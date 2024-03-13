#ifndef GAMEPADSTATE_H
#define GAMEPADSTATE_H

#include <pico/stdlib.h>
#define GAMEPAD_MASK_DPAD (GAMEPAD_MASK_UP | GAMEPAD_MASK_DOWN | GAMEPAD_MASK_LEFT | GAMEPAD_MASK_RIGHT)

#define GAMEPAD_JOYSTICK_MIN 0
#define GAMEPAD_JOYSTICK_MID 0x7FFF
#define GAMEPAD_JOYSTICK_MAX 0xFFFF

#define GAMEPAD_MASK_UP (1U << 0)
#define GAMEPAD_MASK_DOWN (1U << 1)
#define GAMEPAD_MASK_LEFT (1U << 2)
#define GAMEPAD_MASK_RIGHT (1U << 3)

#define GAMEPAD_MASK_B1 (1U << 0)
#define GAMEPAD_MASK_B2 (1U << 1)
#define GAMEPAD_MASK_B3 (1U << 2)
#define GAMEPAD_MASK_B4 (1U << 3)
#define GAMEPAD_MASK_L1 (1U << 4)
#define GAMEPAD_MASK_R1 (1U << 5)
#define GAMEPAD_MASK_L2 (1U << 6)
#define GAMEPAD_MASK_R2 (1U << 7)
#define GAMEPAD_MASK_S1 (1U << 8)
#define GAMEPAD_MASK_S2 (1U << 9)
#define GAMEPAD_MASK_L3 (1U << 10)
#define GAMEPAD_MASK_R3 (1U << 11)
#define GAMEPAD_MASK_A1 (1U << 12)
#define GAMEPAD_MASK_A2 (1U << 13)

const uint8_t dpadMasks[] =
	{
		GAMEPAD_MASK_UP,
		GAMEPAD_MASK_DOWN,
		GAMEPAD_MASK_LEFT,
		GAMEPAD_MASK_RIGHT,
};

const uint16_t buttonMasks[] =
	{
		GAMEPAD_MASK_B1,
		GAMEPAD_MASK_B2,
		GAMEPAD_MASK_B3,
		GAMEPAD_MASK_B4,
		GAMEPAD_MASK_L1,
		GAMEPAD_MASK_R1,
		GAMEPAD_MASK_L2,
		GAMEPAD_MASK_R2,
		GAMEPAD_MASK_S1,
		GAMEPAD_MASK_S2,
		GAMEPAD_MASK_L3,
		GAMEPAD_MASK_R3,
		GAMEPAD_MASK_A1,
		GAMEPAD_MASK_A2,
};

struct GamepadState
{
	uint8_t dpad{0};
	uint16_t buttons{0};
	uint16_t aux{0};
	uint16_t lx{GAMEPAD_JOYSTICK_MID};
	uint16_t ly{GAMEPAD_JOYSTICK_MID};
	uint16_t rx{GAMEPAD_JOYSTICK_MID};
	uint16_t ry{GAMEPAD_JOYSTICK_MID};
	uint8_t lt{0};
	uint8_t rt{0};
};
#endif