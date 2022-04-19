#include "rap.h"


namespace RAP {

void Init(u8 cs_pin)
{
	pinMode(cs_pin, OUTPUT);
	SPI.begin();
}

void Write(byte addr, byte data, u8 cs_pin)
{
	byte cmd_byte = WRITE_MASK | addr;

	SPI.beginTransaction(SPISettings(10'000'000, MSBFIRST, SPI_MODE1));

	digitalWrite(cs_pin, LOW);
	SPI.transfer(cmd_byte);
	SPI.transfer(data);
	digitalWrite(cs_pin, HIGH);

	SPI.endTransaction();
	delayMicroseconds(DEFAULT_WRITE_DELAY);
}

void Read(byte addr, byte* data, byte count, u8 cs_pin)
{
	byte cmd_byte = READ_MASK | addr;

	SPI.beginTransaction(SPISettings(10'000'000, MSBFIRST, SPI_MODE1));

	digitalWrite(cs_pin, LOW);
	SPI.transfer(cmd_byte);
	SPI.transfer(0xFC);
	SPI.transfer(0xFC);

	for (byte i = 0; i < count; ++i)
		data[i] = SPI.transfer(0xFC);

	digitalWrite(cs_pin, HIGH);

	SPI.endTransaction();
}

}
