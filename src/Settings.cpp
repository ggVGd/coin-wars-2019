#include "Settings.h"
#include <fstream>

using namespace nlohmann;

Settings::Settings()
{
	std::ifstream file("settings.json");
	_data = json::parse(file);
	file.close();
}
Settings::~Settings()
{
}
Settings& Settings::getSingleton()
{
	static Settings instance;
	return instance;
}
