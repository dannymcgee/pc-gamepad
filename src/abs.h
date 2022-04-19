#pragma once

#include "common.h"


struct AbsData {
public:
	u16 X;
	u16 Y;
	u16 Z;
	u8 ButtonFlags;
	bool TouchDown;
	bool Hovering;

private:
	static const u8 sk_ZValueMap[ROWS_Y][COLS_X];

public:
	AbsData();

	/**
	 * Checks for "hover" caused by curved overlays.
	 *
	 * This function identifies when a finger is "hovering" so your system can
	 * choose to ignore it. The sensor detects the finger in the space above the
	 * sensor.
	 *
	 * If the finger is on the surface of the sensor the Z value is highest. If
	 * the finger is a few millimeters above the surface the z value is much
	 * lower.
	 *
	 * Adding a curved overlay will allow the finger to be closer in the middle
	 * (so a higher z value) but farther on the perimeter (so a lower z value).
	 * With a curved overlay you tune the gain of the system to see a finger on
	 * the perimeter of the sensor (the finger is farther away).
	 *
	 * Unfortunately a finger near the center will be detected above the surface.
	 * This code will tell you when to ignore that "hovering" finger.
	 *
	 * `ZVALUE_MAP[][]` stores a lookup table in which you can define the Z-value
	 * and XY position that is considered "hovering". Experimentation/tuning is
	 * required.
	 *
	 * NOTE: Z-value output decreases to 0 as you move your finger away from the
	 * sensor, and it's maximum value is 0x63 (6-bits).
	 */
	void CheckValidTouch();

	/** Scale coordinates to arbitrary X, Y resolution */
	void ScaleData(u16 x_res, u16 y_res);

	/** General print function to display the parameters */
	void DataToString(String& out_str, bool curve) const;

private:
	/**
	 * Clips raw coordinates to "reachable" window of sensor.
	 *
	 * NOTE: Values outside this window can only appear as a result of noise.
	 */
	void clipCoordinates();

	/** Checks touch data to see if it is a z-idle packet (all zeros) */
	bool zIdlePacket() const;
};
