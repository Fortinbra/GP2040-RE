#include "GamepadState.h"
#include <pico/stdlib.h>

#ifndef I2CDATA_H
#define I2CDATA_H

struct I2CData
{
	GamepadState state;
	uint64_t timestamp = 0;
};
#endif