#pragma once
#include <atomic>
#include <string>
#include <unordered_map>
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
