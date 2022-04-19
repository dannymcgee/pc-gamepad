#include "pad.h"
#include "abs.h"
#include "rap.h"


PadData::PadData(u8 cs_pin, u8 dr_pin, u8 led_pin)
	: LedPin(led_pin)
	, m_CsPin(cs_pin)
	, m_DrPin(dr_pin)
{}

void PadData::Init()
{
	RAP::Init(m_CsPin);
	digitalWrite(m_CsPin, HIGH);
	pinMode(m_DrPin, INPUT);

	// Host clears SW_CC flag
	clearFlags();

	// Host configures bits of registers 0x03 and 0x05
	RAP::Write(0x03, SYSCONFIG_1, m_CsPin);
	RAP::Write(0x05, FEEDCONFIG_2, m_CsPin);

	// Host enables preferred output mode (absolute)
	RAP::Write(0x04, FEEDCONFIG_1, m_CsPin);

	// Host sets z-idle packet count to 5 (default is 30)
	RAP::Write(0x0A, Z_IDLE_COUNT, m_CsPin);
	Serial.println("Pinnacle Initialized...");
}

void PadData::clearFlags()
{
	RAP::Write(0x02, 0x00, m_CsPin);
}

void PadData::SetAdcAttenuation(u8 gain)
{
	u8 temp = 0x00;

	Serial.println();
	Serial.println("Setting ADC Gain...");

	read_ERA(0x0187, &temp, 1);

	Serial.print("Current value:\t");
	Serial.println(temp, HEX);

	temp &= 0x3F;
	temp |= gain;
	write_ERA(0x0187, temp);
	read_ERA(0x0187, &temp, 1);

	Serial.print("New value:\t");
	Serial.print(temp, HEX);

	switch (gain) {
		case ADC_ATTENUATE_1X:
			Serial.println(" (X/1)");
			break;
		case ADC_ATTENUATE_2X:
			Serial.println(" (X/2)");
			break;
		case ADC_ATTENUATE_3X:
			Serial.println(" (X/3)");
			break;
		case ADC_ATTENUATE_4X:
			Serial.println(" (X/4)");
			break;
		default:
			break;
	}
}

void PadData::TuneEdgeSensitivity()
{
	u8 temp = 0x00;

	Serial.println();
	Serial.println("Setting xAxis.WideZMin...");

	read_ERA(0x0149, &temp, 1);
	Serial.print("Current value:\t");
	Serial.println(temp, HEX);

	write_ERA(0x0149, 0x04);
	read_ERA(0x0149, &temp, 1);
	Serial.print("New value:\t");
	Serial.println(temp, HEX);

	Serial.println();
	Serial.println("Setting yAxis.WideZMin...");

	read_ERA(0x0168, &temp, 1);
	Serial.print("Current value:\t");
	Serial.println(temp, HEX);

	write_ERA(0x0168, 0x03);
	read_ERA(0x0168, &temp, 1);
	Serial.print("New value:\t");
	Serial.println(temp, HEX);
}

void PadData::ForceCalibration()
{
	u8 cal_config_value = 0x00;

	EnableFeed(false);
	RAP::Read(0x07, &cal_config_value, 1, m_CsPin);
	cal_config_value |= 0x01;
	RAP::Write(0x07, cal_config_value, m_CsPin);

	do {
		RAP::Read(0x07, &cal_config_value, 1, m_CsPin);
	}
	while (cal_config_value & 0x01);

	clearFlags();
}

void PadData::EnableFeed(bool enable)
{
	u8 temp;

	RAP::Read(0x04, &temp, 1, m_CsPin);

	if (enable) {
		temp |= 0x01; // Set Feed Enable bit
		RAP::Write(0x04, temp, m_CsPin);
	}
	else {
		temp &= ~0x01; // Clear Feed Enable bit
		RAP::Write(0x04, temp, m_CsPin);
	}
}

void PadData::GetAbsolute(AbsData& out_result)
{
	u8 data[6] = { 0, 0, 0, 0, 0, 0 };
	RAP::Read(0x12, data, 6, m_CsPin);

	clearFlags();

	out_result.ButtonFlags = data[0] & 0x3F;
	out_result.X = data[2] | ((data[4] & 0x0F) << 8);
	out_result.Y = data[3] | ((data[4] & 0xF0) << 4);
	out_result.Z = data[5] & 0x3F;

	out_result.TouchDown = out_result.X != 0;
}

bool PadData::DrAsserted()
const {
	return digitalRead(m_DrPin);
}

void PadData::read_ERA(u16 addr, u8* data, u16 count)
{
	u8 era_control_value = 0xFF;
	EnableFeed(false);

	RAP::Write(0x1C, (u8) (addr >> 8), m_CsPin);     // Send upper byte of ERA address
	RAP::Write(0x1D, (u8) (addr & 0x00FF), m_CsPin); // Send lower byte of ERA address

	for (u16 i = 0; i < count; ++i) {
		RAP::Write(0x1E, 0x05, m_CsPin); // Signal ERA-read (auto-increment) to Pinnacle

		// Wait for status register 0x1E to clear
		do {
			RAP::Read(0x1E, &era_control_value, 1, m_CsPin);
		}
		while (era_control_value != 0x00);

		RAP::Read(0x1B, data + i, 1, m_CsPin);
		clearFlags();
	}
}

void PadData::write_ERA(u16 addr, u8 data)
{
	u8 era_control_value = 0xFF;
	EnableFeed(false);

	RAP::Write(0x1B, data, m_CsPin);

	RAP::Write(0x1C, (u8) (addr >> 8), m_CsPin);     // Upper byte of ERA address
	RAP::Write(0x1D, (u8) (addr & 0x00FF), m_CsPin); // Lower byte of ERA address

	RAP::Write(0x1E, 0x02, m_CsPin); // Signal an ERA-write to Pinnacle

	// Wait for status register 0x1E to clear
	do {
		RAP::Read(0x1E, &era_control_value, 1, m_CsPin);
	}
	while (era_control_value != 0x00);

	delayMicroseconds(DEFAULT_WRITE_DELAY);
	clearFlags();
}
