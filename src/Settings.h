#pragma once
#include <nlohmann/json.hpp>

class Settings
{
	Settings();
	Settings(const Settings&) = delete;
	Settings& operator=(const Settings&) = delete;
public:
	~Settings();
	static Settings& getSingleton();

	template<typename T>
	static T get(const std::string& key)
	{
		return getSingleton()._data[key].get<T>();
	}

private:
	nlohmann::json _data;
};
