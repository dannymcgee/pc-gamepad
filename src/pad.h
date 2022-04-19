#pragma once

#include "common.h"

struct AbsData;


struct PadData {
public:
	u8 LedPin;

private:
	u8 m_CsPin;
	u8 m_DrPin;

public:
	PadData(u8 cs_pin, u8 dr_pin, u8 led_pin);

	void Init();

	/**
	 * Adjusts the feedback in the ADC, effectively attenuating the finger
	 * signal. By default, the the signal is maximally attenuated
	 * (ADC_ATTENUATE_4X) for use with thin, flat overlays.
	 */
	void SetAdcAttenuation(u8 gain);

	/** Changes thresholds to improve detection of fingers */
	void TuneEdgeSensitivity();

	/**
	 * Forces Pinnacle to re-calibrate. If the touchpad is reporting touches when
	 * no fingers are on the pad then calibration (compensation) is wrong.
	 * Calling this function will fix the problem.
	 *
	 * WARNING: Re-enable the feed after calling this
	 */
	void ForceCalibration();

	/** Enables/disables the feed */
	void EnableFeed(bool enable);

	bool DrAsserted() const;

	/**
	 * Reads XYZ data from Pinnacle registers 0x14 through 0x17 and writes the
	 * result to `out_result`.
	 */
	void GetAbsolute(AbsData& out_result);

private:
	void clearFlags();

	/**
	 * Reads `count` bytes from an extended register at `addr` (16-bit address),
	 * stores values in `data`.
	 */
	void read_ERA(u16 addr, u8* data, u16 count);

	/**
	 * Writes a byte, `data`, to an extended register at `addr` (16-bit address).
	 */
	void write_ERA(u16 addr, u8 data);
};
