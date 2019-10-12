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

	struct Bucket
	{
		int multiplier;
		std::string effect;
		std::string label;
	};
	inline static const std::vector<Bucket>& buckets()
	{
		return getSingleton()._buckets;
	}

private:
	nlohmann::json _data;
	std::vector<Bucket> _buckets;
};
