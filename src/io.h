#pragma once

struct Touchpad;
struct TouchData;


namespace IO {

void Init();

void WriteStartup();

void WriteData(
	const Touchpad& pad0, const TouchData& data0,
	const Touchpad& pad1, const TouchData& data1);

}
