#pragma once
#include "Cinnabar/Math/Vector2.h"
#include "Enumerations.h"
#include <string>

struct CoinInsertEvent
{
	CoinType coinType;
};

struct CoinConsumeEvent
{
	CoinType coinType;
};

struct DepartmentSelectEvent
{
	std::string department;
};

struct DepartmentDeselectEvent
{
};

struct GivePointsEvent
{
	Cinnabar::Vector2 position;
	int points;
};

struct PuckBucketEvent
{
	int bucketIndex;
	CoinType coinType;
	Cinnabar::Vector2 puckPosition;
};

struct VideoPlayRequestEvent
{
	std::string filename;
	bool loop = false;
};

struct VideoStopRequestEvent
{
};

struct VideoEndEvent
{
};
