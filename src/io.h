#pragma once

#include "pad.h"
#include "abs.h"


namespace io {

void Init();

void WriteStartup();

void WriteData(
	const PadData& pad0, const AbsData& data0,
	const PadData& pad1, const AbsData& data1);

}
