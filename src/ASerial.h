#pragma once

#include <string>
#include "Enumerations.h"

class ASerial
{
	ASerial(const ASerial&) = delete;
	ASerial& operator=(const ASerial&) = delete;
public: 
	ASerial() = default;
	~ASerial() = default;

	bool canRead();
	CoinType getCommand();
	bool openPort(const std::string& deviceName);
	void closePort();


private: 
	std::string _deviceName;
	int _fileDescriptor = -1; // indicate unopened state (will be non-negative if opened)
};

