#pragma once
#include <cassert>
#include <atomic>
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "util/string.h"

struct Setting;

class StaticSettingsManager {
public:
	typedef std::unordered_map<std::string, Setting *> SettingTypes;
	explicit StaticSettingsManager(SettingTypes &_types);
	std::string get(const std::string &name) const;
	void set(const std::string &name, const std::string &value);
	bool update(const std::string &name, std::string &value);

private:
	const SettingTypes &types;
};

struct Setting {
	virtual std::string get() = 0;
	virtual void set(const std::string &) = 0;
};

template <typename Type>
struct SettingStd: Setting {
	std::atomic<Type> *const setting;

	SettingStd(std::atomic<Type> *_setting) : setting(_setting)
	{
	}

	std::string get() override
	{
		return std::to_string(*setting);
	}
};

struct SettingBool: SettingStd<bool> {
	using SettingStd<bool>::SettingStd;

	void set(const std::string &value) override
	{
		*setting = is_yes(value);
	}
};

struct SettingInt: SettingStd<int> {
	const int min;
	const int max;

	SettingInt(std::atomic<int> *_setting, int _min = INT_MIN, int _max = INT_MAX)
			: SettingStd<int>(_setting), min(_min), max(_max)
	{
	}

	void set(const std::string &value) override
	{
		*setting = mystoi(value, min, max);
	}
};

struct SettingFloat: SettingStd<float> {
	const float min;
	const float max;

	SettingFloat(std::atomic<float> *_setting, float _min = INT_MIN, float _max = INT_MAX)
			: SettingStd<float>(_setting), min(_min), max(_max)
	{
	}

	void set(const std::string &value) override
	{
		*setting = mystof(value, min, max);
	}
};

template <typename Enum>
struct SettingEnum: Setting {
	std::atomic<Enum> *const setting;

	// Enumerators must be assigned sequentially, starting from 0 (as by default).
	SettingEnum(std::atomic<Enum> *_setting, std::initializer_list<std::pair<Enum, std::string>> _labels)
			: setting(_setting)
	{
		labels.resize(_labels.size());
		int k = 0;
		for (const auto &p: _labels) {
			assert(static_cast<int>(p.first) == k);
			labels[k++] = std::move(p.second);
		}
	}

	std::string get() override
	{
		return labels.at(static_cast<int>(setting->load()));
	}

	void set(const std::string &value) override
	{
		for (int k = 0; k < labels.size(); k++) {
			if (strcasecmp(labels[k].c_str(), value.c_str()) != 0)
				continue;
			setting->store(static_cast<Enum>(k));
			break;
		}
	}

private:
	std::vector<std::string> labels;
};

StaticSettingsManager::StaticSettingsManager(SettingTypes &_types)
		: types(_types)
{
}

std::string StaticSettingsManager::get(const std::string &name) const
{
	auto p = types.find(name);
	if (p == types.end())
		return{};
	return p->second->get();
}

std::string StaticSettingsManager::set(const std::string &name, const std::string &value)
{
	auto p = types.find(name);
	if (p == types.end())
		return{};
	p->second->set(value);
	return p->second->get();
}

bool StaticSettingsManager::update(const std::string &name, std::string &value)
{
	auto p = types.find(name);
	if (p == types.end())
		return false;
	p->second->set(value);
	value = p->second->get();
	return true;
}
