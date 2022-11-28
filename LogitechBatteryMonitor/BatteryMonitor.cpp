#include <iostream>
#include <string>

#include "hidapi.h"
#include "Debug.h"
#include "BatteryMonitor.h"
#include "G502.h"


constexpr size_t LENGTH_HIDPP_LONG = 20;
constexpr size_t LENGTH_HIDPP_SHORT = 7;


BatteryMonitor::BatteryMonitor()
{
   
}

BatteryMonitor::~BatteryMonitor()
{
	
}

void BatteryMonitor::start()
{
	if (m_deviceHandle)
		return;

	// Get a list of devices with matching VID/PID
	hid_device_info* devices = hid_enumerate(G502::VID, G502::PID);

	hid_device_info* dev = devices;
	std::string devPath = "";

	while (dev)
	{
		if (dev->usage_page != G502::USAGE_PAGE || dev->usage != G502::USAGE)
		{
			dev = dev->next;
			continue;
		}

		devPath = std::string(dev->path);
		break;
	}

	dev = nullptr;

	// Free device list
	hid_free_enumeration(devices);

	std::cout << "Device path: " << devPath << std::endl;

	if (devPath == "")
		return;

	m_deviceHandle = hid_open_path(devPath.data());

	if (!m_deviceHandle)
		std::cout << "Error opening device!" << std::endl;
}

void BatteryMonitor::stop()
{
	// Close device handle
	if (m_deviceHandle)
		hid_close(m_deviceHandle);
}

void BatteryMonitor::getBatteryStatus()
{
	if (!m_deviceHandle)
		return;

	unsigned char bufferWrite[LENGTH_HIDPP_LONG] = { 0 };
	bufferWrite[0] = 0x11;
	bufferWrite[1] = 0x01;
	bufferWrite[2] = 0x06;
	bufferWrite[3] = 0x0A;

	int res = hid_write(m_deviceHandle, bufferWrite, LENGTH_HIDPP_LONG);

	std::cout << "Bytes written: " << res << std::endl;
	Debug::printHexBuffer(bufferWrite, LENGTH_HIDPP_LONG);

	if (res == -1)
		return;

	unsigned char bufferRead[LENGTH_HIDPP_LONG]{ 0 };

	res = hid_read(m_deviceHandle, bufferRead, LENGTH_HIDPP_LONG);

	std::cout << "Bytes read: " << res << std::endl;
	Debug::printHexBuffer(bufferRead, LENGTH_HIDPP_LONG);

	if (res < LENGTH_HIDPP_LONG)
		return;

	if (!checkValidReply(bufferWrite, LENGTH_HIDPP_LONG, bufferRead, LENGTH_HIDPP_LONG))
		return;

	BatteryStatus batStatus = decodeBatteryStatus(bufferRead, LENGTH_HIDPP_LONG);

	std::cout << "Voltage: " << batStatus.voltage << " mV" << std::endl;
	std::cout << "Percentage: " << G502::getPercentFromVoltage(batStatus.voltage) << " %" << std::endl;
	std::cout << "Status: " << getStatusStringFromEnum(batStatus.powerStatus) << std::endl;
}

bool BatteryMonitor::checkValidReply(const unsigned char* bufferWrite, size_t sizeWrite, const unsigned char* bufferRead, size_t sizeRead)
{
	if (sizeWrite < 4 ||sizeRead < 4)
		return false;

	bool result = true;

	for (size_t idx = 0; idx < 4; ++idx)
	{
		if (bufferWrite[idx] != bufferRead[idx])
			result = false;
	}

	return result;
}

BatteryMonitor::BatteryStatus BatteryMonitor::decodeBatteryStatus(const unsigned char* bufferRead, size_t sizeRead)
{
	BatteryStatus result;

	result.voltage = (bufferRead[4] << 8);
	result.voltage |= bufferRead[5];

	if (bufferRead[6] & 0x80)
	{
		switch (bufferRead[6] & 0x07)
		{
		case 0:
			result.powerStatus = BATTERY_CHARGING;
			break;
		case 1:
			result.powerStatus = BATTERY_FULL;
			break;
		case 2:
			result.powerStatus = BATTERY_NOT_CHARGING;
			break;
		default:
			result.powerStatus = BATTERY_STATUS_UNKNOWN;
			break;
		}
	}
	else
		result.powerStatus = BATTERY_DISCHARGING;

	return result;
}

std::string BatteryMonitor::getStatusStringFromEnum(BATTERY_POWER_STATUS status)
{
	switch (status)
	{
	case BATTERY_STATUS_UNKNOWN:
		return "unknown";
	case BATTERY_CHARGING:
		return "charging";
	case BATTERY_DISCHARGING:
		return "discharging";
	case BATTERY_FULL:
		return "full";
	case BATTERY_NOT_CHARGING:
		return "not charging";
	}

	return "ERROR";
}
