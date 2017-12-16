#include "settings_static.h"
#include "util/string.h"

StaticSettings::StaticSettings(StaticSettings::SettingTypes &types)
		: settings(types)
{
}

std::string StaticSettings::get(const std::string &name) const
{
	auto p = settings.find(name);
	if (p == settings.end())
		return{};
	return p->second->get(*this);
}

std::string StaticSettings::set(const std::string &name, const std::string &value)
{
	auto p = settings.find(name);
	if (p == settings.end())
		return{};
	p->second->set(*this, value);
	return p->second->get(*this);
}

std::string SettingBool::get(const StaticSettings &settings)
{
	return std::to_string(settings.*setting);
}

void SettingBool::set(StaticSettings &settings, const std::string &value)
{
	settings.*setting = is_yes(value);
}

std::string SettingInt::get(const StaticSettings &settings)
{
	return std::to_string(settings.*setting);
}

void SettingInt::set(StaticSettings &settings, const std::string &value)
{
	settings.*setting = mystoi(value, min, max);
}

std::string SettingFloat::get(const StaticSettings &settings)
{
	return std::to_string(settings.*setting);
}

void SettingFloat::set(StaticSettings &settings, const std::string &value)
{
	settings.*setting = mystof(value, min, max);
}
