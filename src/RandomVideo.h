#pragma once
#include <memory>
#include <random>
#include <string>
#include <vector>

class RandomVideo
{
public:
	RandomVideo();
	const std::string& get();

private:
	std::vector<std::string> _videos;

	std::random_device _rd;
	std::mt19937 _gen;
	std::shared_ptr<std::uniform_int_distribution<>> _dis;
};
