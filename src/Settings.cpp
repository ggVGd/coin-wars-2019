#include "Settings.h"
#include <fstream>

using namespace nlohmann;

static void from_json(const json& json, Settings::Bucket& bucket)
{
	if(json.contains("multiplier"))
		bucket.multiplier = json["multiplier"].get<int>();
	if(json.contains("effect"))
		bucket.effect = json["effect"].get<std::string>();
	if(json.contains("label"))
		bucket.label = json["label"].get<std::string>();
};

Settings::Settings()
{
	std::ifstream file("settings.json");
	_data = json::parse(file);
	file.close();

	_buckets = _data["buckets"].get<std::vector<Bucket>>();
}
Settings::~Settings()
{
}
Settings& Settings::getSingleton()
{
	static Settings instance;
	return instance;
}
