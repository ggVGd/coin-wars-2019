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
		int sizeFactor = 1;
		std::string effect;
		std::string label;
	};
	inline static const std::vector<Bucket>& buckets()
	{
		return getSingleton()._buckets;
	}

	inline static int bucketSizeFactors()
	{
		int result = 0;
		for(const auto& bucket : buckets())
			result += bucket.sizeFactor;
		return result;
	}

	inline static int bucketIndex(int slot)
	{
		std::size_t index = 0;
		for(; slot > 0 && index < buckets().size(); index++)
			slot -= buckets().at(index).sizeFactor;
		return index;
	}

private:
	nlohmann::json _data;
	std::vector<Bucket> _buckets;
};
