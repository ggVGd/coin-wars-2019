#pragma once
#include "Enumerations.h"
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
		while(slot >= 0 && index < buckets().size())
		{
			slot -= buckets().at(index).sizeFactor;
			if(slot < 0)
				return index;
			index++;
		}
		return -1;
	}

	inline static std::string coinText(CoinType coinType)
	{
		int n = static_cast<std::underlying_type<CoinType>::type>(coinType);
		return getSingleton()._data["coin_" + std::to_string(n) + "_text"].get<std::string>();
	}

	inline static int coinValue(CoinType coinType)
	{
		int n = static_cast<std::underlying_type<CoinType>::type>(coinType);
		return getSingleton()._data["coin_" + std::to_string(n) + "_value"].get<int>();
	}

private:
	nlohmann::json _data;
	std::vector<Bucket> _buckets;
};
