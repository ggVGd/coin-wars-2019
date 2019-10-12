#pragma once
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

struct PuckBucketEvent
{
	int bucketIndex;
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
