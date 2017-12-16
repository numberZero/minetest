DIR_DELIM = "/"

function get_mods() return {} end

core = {}
function core.get_builtin_path() return "builtin/" end
core.log = print

dofile("builtin/common/misc_helpers.lua")
local parse_config_file = dofile("builtin/common/parse_config_file.lua")
local settings = parse_config_file(true, false)

local concat = table.concat
local insert = table.insert
local sprintf = string.format
local rep = string.rep

local minetest_example_header = [[
#    This file contains a list of all available settings and their default value for minetest.conf

#    By default, all the settings are commented and not functional.
#    Uncomment settings by removing the preceding #.

#    minetest.conf is read by default from:
#    ../minetest.conf
#    ../../minetest.conf
#    Any other path can be chosen by passing the path as a parameter
#    to the program, eg. "minetest.exe --config ../minetest.conf.example".

#    Further documentation:
#    http://wiki.minetest.net/

]]

local group_format_template = [[
# %s = {
#    offset      = %s,
#    scale       = %s,
#    spread      = (%s, %s, %s),
#    seed        = %s,
#    octaves     = %s,
#    persistence = %s,
#    lacunarity  = %s,
#    flags       = "%s"
# }

]]

local function create_minetest_conf_example()
	local result = { minetest_example_header }
	for _, entry in ipairs(settings) do
		if entry.type == "category" then
			if entry.level == 0 then
				insert(result, "#\n# " .. entry.name .. "\n#\n\n")
			else
				insert(result, rep("#", entry.level))
				insert(result, "# " .. entry.name .. "\n\n")
			end
		else
			local group_format = false
			if entry.noise_params and entry.values then
				if entry.type == "noise_params_2d" or entry.type == "noise_params_3d" then
					group_format = true
				end
			end
			if entry.comment ~= "" then
				for _, comment_line in ipairs(entry.comment:split("\n", true)) do
					insert(result, "#    " .. comment_line .. "\n")
				end
			end
			insert(result, "#    type: " .. entry.type)
			if entry.min then
				insert(result, " min: " .. entry.min)
			end
			if entry.max then
				insert(result, " max: " .. entry.max)
			end
			if entry.values and entry.noise_params == nil then
				insert(result, " values: " .. concat(entry.values, ", "))
			end
			if entry.possible then
				insert(result, " possible values: " .. concat(entry.possible, ", "))
			end
			insert(result, "\n")
			if group_format == true then
				insert(result, sprintf(group_format_template, entry.name, entry.values[1],
						entry.values[2], entry.values[3], entry.values[4], entry.values[5],
						entry.values[6], entry.values[7], entry.values[8], entry.values[9],
						entry.values[10]))
			else
				local append
				if entry.default ~= "" then
					append = " " .. entry.default
				end
				insert(result, sprintf("# %s =%s\n\n", entry.name, append or ""))
			end
		end
	end
	return concat(result)
end

local translation_file_header = [[
// This file is automatically generated
// It conatins a bunch of fake gettext calls, to tell xgettext about the strings in config files
// To update it, refer to the bottom of builtin/mainmenu/dlg_settings_advanced.lua

fake_function() {]]

local function create_translation_file()
	local result = { translation_file_header }
	for _, entry in ipairs(settings) do
		if entry.type == "category" then
			insert(result, sprintf("\tgettext(%q);", entry.name))
		else
			if entry.readable_name then
				insert(result, sprintf("\tgettext(%q);", entry.readable_name))
			end
			if entry.comment ~= "" then
				local comment_escaped = entry.comment:gsub("\n", "\\n")
				comment_escaped = comment_escaped:gsub("\"", "\\\"")
				insert(result, "\tgettext(\"" .. comment_escaped .. "\");")
			end
		end
	end
	insert(result, "}\n")
	return concat(result, "\n")
end

local function escape_id(name)
	name = name:gsub("[.-]", "_")
	if name:match("^%d") then
		return "_" .. name
	end
	assert(name:match("^[%w_]*$"))
	return name
end

local function capitalize_id(id)
	if id:match("^%s*$") then
		return "_"
	end
	return id:gsub("^(%a)", string.upper):gsub("_(%a)", string.upper)
end

local function escape_str(text)
	return text:gsub("[\"\\]", "\\%1"):gsub("\n", "\\n")
end

local function is_numerical(values)
	for _, v in ipairs(values) do
		if not v:match("^%d+$") then
			return false
		end
	end
	return true
end

local settings_builtin_header = [[
// This file is automatically generated

#pragma once
#include <atomic>
#include "settings_static.h"

struct BuiltinSettings: StaticSettings {
	BuiltinSettings();
]]

local settings_builtin_footer = [[
};

extern const BuiltinSettings builtin_settings;
]]

local settings_builtin_impl = [[
// This file is automatically generated

#include "settings_builtin.h"

// Enum settings
%s
// Definitions
%s
// Map
StaticSettings::SettingTypes types = {
%s};

BuiltinSettings::BuiltinSettings() : StaticSettings(types)
{
}
]]

local function insertf(table, template, ...)
	insert(table, template:format(...))
end

local function create_settings_builtin()
	local result = { settings_builtin_header }
	local types = {}
	local defs = {}
	local map = {}
	local function append(template, ...)
		insert(result, template:format(...))
	end
	for _, entry in ipairs(settings) do
		local t = entry.type
		if t == "category" then
			if entry.level == 0 then
				append("\n/*\n * %s\n */\n", entry.name)
			else
				append("\n/%s %s */\n", rep("*", entry.level), entry.name)
			end
		else
			local name = escape_id(entry.name)
			if t == "int" or t == "float" or t == "bool" then
				append("\tstd::atomic<%s> %s{%s};\n", t, name, entry.default)
				insertf(defs, "static Setting%s setting_%s{&BuiltinSettings::%s};\n", capitalize_id(t), name, name)
				insertf(map, "\t{\"%s\", &setting_%s},\n", entry.name, name)
			elseif t == "enum" then
				if is_numerical(entry.values) then
					append("\tstd::atomic<int> %s{%s};\n", name, entry.default)
				else
					local enum = capitalize_id(name)
					append("\tenum class %s {\n", enum)
					for _, v in ipairs(entry.values) do
						append("\t\t%s,\n", capitalize_id(v))
					end
					append("\t};\n")
					append("\tstd::atomic<%s> %s{%s::%s};\n", enum, name, enum, capitalize_id(entry.default))
				end
			end
		end
	end
	insert(result, settings_builtin_footer);
	return concat(result), settings_builtin_impl:format(concat(types), concat(defs), concat(map))
end

local file = assert(io.open("minetest.conf.example", "w"))
file:write(create_minetest_conf_example())
file:close()

file = assert(io.open("src/settings_translation_file.cpp", "w"))
file:write(create_translation_file())
file:close()

local h, cpp = create_settings_builtin()
file = assert(io.open("src/settings_builtin.h", "w"))
file:write(h)
file:close()

file = assert(io.open("src/settings_builtin.cpp", "w"))
file:write(cpp)
file:close()
