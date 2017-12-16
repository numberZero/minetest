#pragma once
#include <atomic>
#include <string>
#include <unordered_map>

struct Setting;

class StaticSettings {
public:
	typedef std::unordered_map<std::string, Setting *> SettingTypes;
	std::string get(const std::string &name) const;
	std::string set(const std::string &name, const std::string &value);

protected:
	StaticSettings(SettingTypes &types);

private:
	const SettingTypes &settings;
};

struct Setting {
	virtual std::string get(const StaticSettings &) = 0;
	virtual void set(StaticSettings &, const std::string &) = 0;
};

struct SettingBool: Setting {
	std::atomic<bool> StaticSettings:: *const setting;

	SettingBool(std::atomic<bool> StaticSettings:: *const _setting);
	std::string get(const StaticSettings &) override;
	void set(StaticSettings &, const std::string &) override;
};

struct SettingInt: Setting {
	std::atomic<int> StaticSettings:: *const setting;
	const int min;
	const int max;

	SettingInt(std::atomic<bool> StaticSettings:: *const _setting, int _min = INT_MIN, int _max = INT_MAX);
	std::string get(const StaticSettings &) override;
	void set(StaticSettings &, const std::string &) override;
};

struct SettingFloat: Setting {
	std::atomic<float> StaticSettings:: *const setting;
	const float min;
	const float max;

	SettingFloat(std::atomic<bool> StaticSettings:: *const _setting, float _min = FLOAt_MIN, float _max = FLOAT_MAX);
	std::string get(const StaticSettings &) override;
	void set(StaticSettings &, const std::string &) override;
};
