#include "common.h"
#include "touch-data.h"
#include "touchpad.h"
#include "io.h"


// ___ Using a Cirque TM0XX0XX and TM0XX0XX with a DK-000013-0x and Arduino ___
//
// This demonstration application is built to work with a Teensy 3.1/3.2, but it
// can easily be adapted to work with Arduino-based systems. This application
// connects to a TM0XX0XX (Sensor0) and TM0XX0XX (Sensor1) circular touch pad
// via SPI.
//
// To verify that your touch pad is configured for SPI-mode, make sure that R1
// is populated with a 470k resistor (or whichever resistor connects pins 24 &
// 25 of the 1CA027 IC).
//
// The pad is configured for Absolute mode tracking. Touch data is sent in text
// format over USB CDC to the host PC.
//
// You can open a terminal window on the PC to the USB CDC port and see X, Y,
// and Z data fill the window when you touch the sensor. In the Arduino IDE use
// Tools->Serial Monitor to view touch data.
//
// This demo can use both sensor ports on the 02-000620-00REVA00 development
// board.
//
// You can configure which sensors are active using the SENSE0_SELECT and
// SENSE0_SELECT in "common.h".
//
// You can configure curved overlay or flat overlay using SENSE0_OVERLAY_CURVE
// and SENSE1_OVERLAY_CURVE in "common.h".
//
// NOTE: all config values applied in this sample are meant for a module using
// REXT = 976kOhm


TouchData g_TouchData0 {};
TouchData g_TouchData1 {};

Touchpad g_Touchpad0 { CS0_PIN, DR0_PIN, LED_0 };
Touchpad g_Touchpad1 { CS1_PIN, DR1_PIN, LED_1 };


void setup()
{
	IO::Init();

	pinMode(LED_0, OUTPUT);
	pinMode(LED_1, OUTPUT);

	if (SENSE0_SELECT) g_Touchpad0.Init();
	if (SENSE1_SELECT) g_Touchpad1.Init();

	// These functions are required for use with thick overlays (curved)
	if (SENSE0_OVERLAY_CURVE) {
		g_Touchpad0.SetAdcAttenuation(ADC_ATTENUATE_2X);
		g_Touchpad0.TuneEdgeSensitivity();
		g_Touchpad0.ForceCalibration();
	}

	if (SENSE1_OVERLAY_CURVE) {
		g_Touchpad1.SetAdcAttenuation(ADC_ATTENUATE_2X);
		g_Touchpad1.TuneEdgeSensitivity();
		g_Touchpad1.ForceCalibration();
	}

	IO::WriteStartup();

	g_Touchpad0.EnableFeed(true);
	g_Touchpad1.EnableFeed(true);
}

void loop()
{
	// Note: the two Pinnacles are not synchronized. In a polling loop like this
	// you may get one or both of the sensors reporting new data. We just grab
	// what data there is and write it.
	if (g_Touchpad0.DrAsserted() && SENSE0_SELECT) {
		g_Touchpad0.GetAbsolute(g_TouchData0);
		g_TouchData0.CheckValidTouch();
		g_TouchData0.ScaleData(1024, 1024);
	}

	if (g_Touchpad1.DrAsserted() && SENSE1_SELECT) {
		g_Touchpad1.GetAbsolute(g_TouchData1);
		g_TouchData1.CheckValidTouch();
		g_TouchData1.ScaleData(1024, 1024);
	}

	IO::WriteData(g_Touchpad0, g_TouchData0, g_Touchpad1, g_TouchData1);

	digitalWrite(g_Touchpad0.LedPin, !g_TouchData0.TouchDown);
	digitalWrite(g_Touchpad1.LedPin, !g_TouchData1.TouchDown);
}
