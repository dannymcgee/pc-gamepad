#include "io.h"
#include "touch-data.h"
#include "touchpad.h"

namespace IO {

void Init()
{
#ifdef USB_SERIAL
	Serial.begin(9600);
	while (!Serial);
#endif
#ifdef USB_RAWHID
	// TODO
#endif
}

void WriteStartup()
{
#ifdef USB_SERIAL
	Serial.println();

	String print_str =
		SENSE1_SELECT && SENSE0_SELECT
			? "\tX\tY\tZ\t\t\tX\tY\tZ\tBTN" :
		SENSE1_SELECT
			? "SENSE 1\tX\tY\tZ" :
		SENSE0_SELECT
			? "SENSE 0\tX\tY\tZ\tBTN"
			: "BOTH SENSORS DISABLED .. ENABLE SENSOR SELECT";

	Serial.println(print_str);
#endif
#ifdef USB_RAWHID
	// TODO
#endif
}

void WriteData(
	const Touchpad& pad0, const TouchData& data0,
	const Touchpad& pad1, const TouchData& data1)
{
#ifdef USB_SERIAL
	auto print_str = String("");

	if (pad0.DrAsserted() && SENSE0_SELECT) {
		print_str += "SENSE_0 ";
		data0.DataToString(print_str, SENSE0_OVERLAY_CURVE);

		if (pad1.DrAsserted() && SENSE1_SELECT)
			print_str += "\t";
	}

	if (pad1.DrAsserted() && SENSE1_SELECT) {
		if (print_str.length() == 0)
			print_str += "\t\t\t\t\t";

		print_str += "SENSE_1 ";
		data1.DataToString(print_str, SENSE1_OVERLAY_CURVE);
	}

	if (print_str.length() > 0) {
		print_str += "\n";
		Serial.print(print_str);
	}
#endif
#ifdef USB_RAWHID
	// TODO
#endif
}

}
