#include "touch-data.h"


const u8 TouchData::sk_ZValueMap[ROWS_Y][COLS_X] {
	{ 0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  2,  3,  5,  5,  3,  2,  0 },
	{ 0,  3,  5, 15, 15,  5,  2,  0 },
	{ 0,  3,  5, 15, 15,  5,  3,  0 },
	{ 0,  2,  3,  5,  5,  3,  2,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0 },
};

u16 Clamp(u16 value, u16 upper, u16 lower)
{
	if (value > upper) return upper;
	if (value < lower) return lower;
	return value;
}

TouchData::TouchData()
	: X(0)
	, Y(0)
	, Z(0)
	, ButtonFlags(0x00)
	, TouchDown(false)
	, Hovering(false)
{}

void TouchData::CheckValidTouch()
{
	u32 zone_x = X / ZONESCALE;
	u32 zone_y = Y / ZONESCALE;

	Hovering = !(Z > sk_ZValueMap[zone_y][zone_x]);
}

void TouchData::ScaleData(u16 x_res, u16 y_res)
{
	u32 x_temp = 0;
	u32 y_temp = 0;

	clipCoordinates();

	x_temp = X;
	y_temp = Y;

	// Translate coordinates to (0, 0) reference by subtracting edge-offset
	x_temp -= PINNACLE_X_LOWER;
	y_temp -= PINNACLE_Y_LOWER;

	// Scale coordinates to (x_res, y_res) range
	X = (u16) (x_temp * x_res / PINNACLE_X_RANGE);
	Y = (u16) (y_temp * y_res / PINNACLE_Y_RANGE);
}

void TouchData::DataToString(String& out_str, bool curve)
const {
	out_str.concat(String(X));
	out_str.concat("\t");
	out_str.concat(String(Y));
	out_str.concat("\t");
	out_str.concat(String(Z));

	if (curve) {
		if (zIdlePacket())
			out_str.concat("-L "); // Append 'Liftoff' code to end of string
		else if (Hovering)
			out_str.concat("-H "); // Append 'Hover' code to end of string
		else
			out_str.concat("-V "); // Append 'Valid' code to end of string
	}
}

void TouchData::clipCoordinates()
{
	X = Clamp(X, PINNACLE_X_UPPER, PINNACLE_X_LOWER);
	Y = Clamp(Y, PINNACLE_Y_UPPER, PINNACLE_Y_LOWER);
}

bool TouchData::zIdlePacket()
const {
	return X == 0 && Y == 0 && Z == 0;
}
