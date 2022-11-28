#include "BatteryMonitor.h"
#include "hidapi.h"

int main()
{
	// Initialize HID library
	hid_init();

	// Create battery monitor
	BatteryMonitor batMon;
	
	// Start battery monitor
	batMon.start();

	batMon.getBatteryStatus();
	
	// Stop battery monitor
	batMon.stop();

	// Close HID library
	hid_exit();

	return 0;
}