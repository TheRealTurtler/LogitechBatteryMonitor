#pragma once

#include <string>

#include "hidapi.h"


class BatteryMonitor
{
public:
	BatteryMonitor();
	~BatteryMonitor();

	void start();
	void stop();
	
	void getBatteryStatus();

private:
	enum BATTERY_POWER_STATUS
	{
		BATTERY_STATUS_UNKNOWN,

		BATTERY_CHARGING,
		BATTERY_DISCHARGING,
		BATTERY_FULL,
		BATTERY_NOT_CHARGING
	};

	struct BatteryStatus
	{
		unsigned short voltage = 0;
		BATTERY_POWER_STATUS powerStatus = BATTERY_STATUS_UNKNOWN;
	};

	hid_device* m_deviceHandle = nullptr;

	static bool checkValidReply(const unsigned char* bufferWrite, size_t sizeWrite, const unsigned char* bufferRead, size_t sizeRead);
	static BatteryStatus decodeBatteryStatus(const unsigned char* bufferRead, size_t sizeRead);

	static std::string getStatusStringFromEnum(BATTERY_POWER_STATUS status);
};
