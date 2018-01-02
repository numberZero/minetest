#pragma once
#include <cassert>
#include <cfloat>
#include <atomic>
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "util/string.h"

struct Setting
{
	virtual std::string get() = 0;
	virtual void set(const std::string &) = 0;
};

struct SettingBool : Setting
{
	std::atomic<bool> *const setting;

	SettingBool(std::atomic<bool> *_setting) : setting(_setting)
	{
	}

	std::string get() override
	{
		return std::to_string(*setting);
	}

	void set(const std::string &value) override
	{
		*setting = is_yes(value);
	}
};

struct SettingInt : Setting
{
	std::atomic<int> *const setting;
	const int min;
	const int max;

	SettingInt(std::atomic<int> *_setting, int _min = INT_MIN, int _max = INT_MAX) :
			setting(_setting), min(_min), max(_max)
	{
	}

	std::string get() override
	{
		return std::to_string(*setting);
	}

	void set(const std::string &value) override
	{
		*setting = mystoi(value, min, max);
	}
};

struct SettingFloat : Setting
{
	std::atomic<float> *const setting;
	const float min;
	const float max;

	SettingFloat(std::atomic<float> *_setting, float _min = -HUGE_VAL,
			float _max = HUGE_VAL) :
			setting(_setting),
			min(_min), max(_max)
	{
	}

	std::string get() override
	{
		return std::to_string(*setting);
	}

	void set(const std::string &value) override
	{
		*setting = mystof(value, min, max);
	}
};

template <typename Enum> struct SettingEnum : Setting
{
	std::atomic<Enum> *const setting;

	// Enumerators must be assigned sequentially, starting from 0 (as by default).
	SettingEnum(std::atomic<Enum> *_setting,
			std::initializer_list<std::pair<Enum, std::string>> _labels) :
			setting(_setting)
	{
		labels.resize(_labels.size());
		std::size_t k = 0;
		for (const auto &p : _labels) {
			assert(static_cast<std::size_t>(p.first) == k);
			labels[k++] = std::move(p.second);
		}
	}

	std::string get() override
	{
		return labels.at(static_cast<std::size_t>(setting->load()));
	}

	void set(const std::string &value) override
	{
		for (std::size_t k = 0; k < labels.size(); k++) {
			if (strcasecmp(labels[k].c_str(), value.c_str()) != 0)
				continue;
			setting->store(static_cast<Enum>(k));
			break;
		}
	}

private:
	std::vector<std::string> labels;
};

class StaticSettingsManager
{
public:
	typedef std::unordered_map<std::string, Setting *> SettingTypes;

	explicit StaticSettingsManager(SettingTypes &_types) : types(_types)
	{
	}

	bool update(const std::string &name, std::string &value)
	{
		auto p = types.find(name);
		if (p == types.end())
			return false;
		p->second->set(value);
		value = p->second->get();
		return true;
	}

private:
	const SettingTypes &types;
};
