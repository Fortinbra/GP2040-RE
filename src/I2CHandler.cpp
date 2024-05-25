#include "headers/I2CHandler.h"

I2CHandler::I2CHandler()
{
	// Constructor code here
}

void I2CHandler::setup_slave()
{

	gpio_init(I2C_SLAVE_SDA_PIN);
	gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(I2C_SLAVE_SDA_PIN);

	gpio_init(I2C_SLAVE_SCL_PIN);
	gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(I2C_SLAVE_SCL_PIN);

	i2c_init(i2c0, I2C_BAUDRATE);
	i2c_slave_init(i2c0, I2C_SLAVE_ADDR, &i2c_slave_handler);
	irq_set_priority(I2C0_IRQ, 64);
}

void I2CHandler::clearI2CBus()
{
	// Configure SCL as output
	gpio_set_dir(I2C_SLAVE_SCL_PIN, GPIO_OUT);

	// Send 9 clock pulses
	for (int i = 0; i < 9; i++)
	{
		gpio_put(I2C_SLAVE_SCL_PIN, 0);
		sleep_ms(1);
		gpio_put(I2C_SLAVE_SCL_PIN, 1);
		sleep_ms(1);
	}

	// Configure SCL as input (back to normal state)
	gpio_set_dir(I2C_SLAVE_SCL_PIN, GPIO_IN);
}

bool I2CHandler::isBusStalled()
{
	// Check if SDA and SCL are both low
	if (gpio_get(I2C_SLAVE_SDA_PIN) == 0 && gpio_get(I2C_SLAVE_SCL_PIN) == 0)
	{
		return true;
	}

	// Check if bus is stuck in start or stop condition
	if ((gpio_get(I2C_SLAVE_SDA_PIN) == 0 && gpio_get(I2C_SLAVE_SCL_PIN) == 1) ||
		(gpio_get(I2C_SLAVE_SDA_PIN) == 1 && gpio_get(I2C_SLAVE_SCL_PIN) == 1))
	{
		return true;
	}

	// Check for lack of acknowledgement after sending data
	// This will depend on your specific I2C implementation

	return false;
}

void I2CHandler::i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event)
{
	static uint8_t buf[GPCOMMS_BUFFER_SIZE] = {0};
	static uint8_t receivedIndex = 0;

	switch (event)
	{
	case I2C_SLAVE_RECEIVE:
		buf[receivedIndex] = i2c_read_byte_raw(i2c);
		receivedIndex++;
		break;
	case I2C_SLAVE_REQUEST:
		// TODO: Do something useful?!?
		break;
	case I2C_SLAVE_FINISH:
	{
		uint8_t command = buf[0];
		uint8_t *payload = &buf[1];

		switch (command)
		{
		case GPCMD_STATE:
		{
			static GPComms_State gpState = {};
			memcpy(&gpState, payload, sizeof(GPComms_State));
			GamepadState gamepadState = gpState.gamepadState;
			{
				uint32_t *data = reinterpret_cast<uint32_t *>(&gamepadState);
				for (size_t i = 0; i < sizeof(GamepadState) / sizeof(uint32_t); i++)
				{
					multicore_fifo_push_blocking(data[i]);
				}
			}
			break;
		}
		default:
			break;
		}
		receivedIndex = 0;
		break;
	}
	default:
		break;
	}
}