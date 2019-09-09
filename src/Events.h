#pragma once
#include "Enumerations.h"
#include <string>

struct CoinInsertEvent
{
	CoinType coinType;
};

struct PuckBucketEvent
{
	int bucketValue;
	CoinType coinType;
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
