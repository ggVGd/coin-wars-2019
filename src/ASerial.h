#pragma once

#include <string>
#include "Enumerations.h"

class ASerial
{
public: 
	ASerial(std::string deviceName);
	~ASerial();
	bool canRead();
	CoinType getCommand();
	void openPort();
	void closePort();


private: 
	std::string _deviceName;
	int _fileDescriptor = -1; // indicate unopened state (will be non-negative if opened)
};

