#include "GamepadState.h"
#include "Types.h"
#include <pico/stdlib.h>

#ifndef I2CDATA_H
#define I2CDATA_H

enum InputMode
{
	INPUT_MODE_XINPUT,
	INPUT_MODE_SWITCH,
	INPUT_MODE_HID,
	INPUT_MODE_KEYBOARD,
	INPUT_MODE_PS4,
	INPUT_MODE_CONFIG = 255,
};

enum DpadMode
{
	DPAD_MODE_DIGITAL,
	DPAD_MODE_LEFT_ANALOG,
	DPAD_MODE_RIGHT_ANALOG,
};

enum SOCDMode
{
	SOCD_MODE_UP_PRIORITY,
	SOCD_MODE_NEUTRAL,
	SOCD_MODE_SECOND_INPUT_PRIORITY,
	SOCD_MODE_FIRST_INPUT_PRIORITY,
	SOCD_MODE_BYPASS,
};

struct GPComms_Status
{
	InputMode inputMode;
	int8_t turboRate; // Negative value indicate off

	bool macroEnabled : 1;
	DpadMode dpadMode : 2;
	SOCDMode socdMode : 3;
	// 2 bits left
};

struct GPComms_State
{
	GamepadState gamepadState;
	Mask_t gpioState = 0;
};

struct GPComms_Message
{
	uint16_t length = 0;
	char *message;
};

enum GPComms_Command
{
	GPCMD_UNKNOWN = 0x00,
	GPCMD_STATE = 0x01,
	GPCMD_STATUS = 0x02,
	GPCMD_MESSAGE = 0x03,
	GPCMD_ACK = 0xFF,
};
#endif