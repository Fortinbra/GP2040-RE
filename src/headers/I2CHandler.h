#ifndef I2CHANDLER_H
#define I2CHANDLER_H

#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include "headers/config.h"
#include "hardware/gpio.h"
#include "headers/I2CData.h"
#include <string.h>
#include <pico/multicore.h>
#include "hardware/irq.h"

class I2CHandler
{
public:
	I2CHandler();
	void setup_slave();
	void clearI2CBus();
	bool isBusStalled();
	static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event);
};

#endif // I2CHANDLER_H