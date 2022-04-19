#pragma once

#include "common.h"


namespace RAP {

	void Init(u8 cs_pin);

	/** Writes single-byte `data` to `address` */
	void Write(byte addr, byte data, u8 cs_pin);

	/** Reads `count` Pinnacle registers starting at `addr` */
	void Read(byte addr, byte* data, byte count, u8 cs_pin);

}
