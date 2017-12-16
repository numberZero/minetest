#pragma once
#include <cassert>
#include <atomic>
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "util/string.h"

template <typename Settings>
struct Setting;

template <typename Settings>
class StaticSettingsManager {
public:
	typedef std::unordered_map<std::string, Setting<Settings> *> SettingTypes;
	StaticSettingsManager(SettingTypes &_types, Settings &_settings);
	std::string get(const std::string &name) const;
	std::string set(const std::string &name, const std::string &value);

private:
	const SettingTypes &types;
	Settings &settings;
};

template <typename Settings>
struct Setting {
	virtual std::string get(const Settings &) = 0;
	virtual void set(Settings &, const std::string &) = 0;
};

template <typename Settings, typename Type>
struct SettingStd: Setting<Settings> {
	typedef std::atomic<Type> Settings:: *PSetting;
	const PSetting setting;

	SettingStd(PSetting _setting) : setting(_setting)
	{
	}

	std::string get(const Settings &settings) override
	{
		return std::to_string(settings.*setting);
	}
};

template <typename Settings>
struct SettingBool: SettingStd<Settings, bool> {
	using typename SettingStd<Settings, bool>::PSetting;
	using SettingStd<Settings, bool>::setting;
	using SettingStd<Settings, bool>::SettingStd;

	void set(Settings &settings, const std::string &value) override
	{
		settings.*setting = is_yes(value);
	}
};

template <typename Settings>
struct SettingInt: SettingStd<Settings, int> {
	using typename SettingStd<Settings, int>::PSetting;
	using SettingStd<Settings, int>::setting;
	const int min;
	const int max;

	SettingInt(PSetting _setting, int _min = INT_MIN, int _max = INT_MAX)
			: SettingStd<Settings, int>(_setting), min(_min), max(_max)
	{
	}

	void set(Settings &settings, const std::string &value) override
	{
		settings.*setting = mystoi(value, min, max);
	}
};

template <typename Settings>
struct SettingFloat: SettingStd<Settings, float> {
	using typename SettingStd<Settings, float>::PSetting;
	using SettingStd<Settings, float>::setting;
	const float min;
	const float max;

	SettingFloat(PSetting _setting, float _min = INT_MIN, float _max = INT_MAX)
			: SettingStd<Settings, float>(_setting), min(_min), max(_max)
	{
	}

	void set(Settings &settings, const std::string &value) override
	{
		settings.*setting = mystof(value, min, max);
	}
};

template <typename Settings, typename Enum>
struct SettingEnum: Setting<Settings> {
	typedef std::atomic<Enum> Settings:: *PSetting;
	const PSetting setting;

	// Enumerators should be assigned sequentially, starting from 0 (as by default).
	// This will throw otherwise.
	SettingEnum(PSetting _setting, std::initializer_list<std::pair<Enum, std::string>> _labels)
			: setting(_setting)
	{
		labels.resize(_labels.size());
		int k = 0;
		for (const auto &p: _labels) {
			assert(static_cast<int>(p.first) == k);
			labels[k++] = std::move(p.second);
		}
	}

	std::string get(const Settings &settings) override
	{
		return labels.at(static_cast<int>((settings.*setting).load()));
	}

	void set(Settings &settings, const std::string &value) override
	{
		for (int k = 0; k < labels.size(); k++) {
			if (strcasecmp(labels[k].c_str(), value.c_str()) != 0)
				continue;
			(settings.*setting).store(static_cast<Enum>(k));
			break;
		}
	}

private:
	std::vector<std::string> labels;
};

template <typename Settings>
StaticSettingsManager<Settings>::StaticSettingsManager(SettingTypes &_types, Settings &_settings)
		: types(_types), settings(_settings)
{
}

template <typename Settings>
std::string StaticSettingsManager<Settings>::get(const std::string &name) const
{
	auto p = types.find(name);
	if (p == types.end())
		return{};
	return p->second->get(settings);
}

template <typename Settings>
std::string StaticSettingsManager<Settings>::set(const std::string &name, const std::string &value)
{
	auto p = types.find(name);
	if (p == types.end())
		return{};
	p->second->set(settings, value);
	return p->second->get(settings);
}
