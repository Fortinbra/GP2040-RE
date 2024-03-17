#include "headers/N64.h"

N64::N64(int latchPin, int clockPin, int dataPin)
	: latchPin(latchPin), clockPin(clockPin), dataPin(dataPin)
{
	// Initialization code here
}

uint16_t N64::translateToFormat(I2CData *data)
{
	// Translate the gamepad data into N64 format. This is just a placeholder.
	// Replace this with your actual translation logic.
	return 0;
}

void N64::sendToSystem(uint16_t data)
{
	// Set latch pin low to start the transmission
	gpio_put(latchPin, 0);

	// Send each bit of the data starting from the most significant bit
	for (int i = 15; i >= 0; i--)
	{
		// Set clock pin low
		gpio_put(clockPin, 0);

		// Set data pin according to the current bit
		gpio_put(dataPin, (data >> i) & 1);

		// Set clock pin high to send the bit
		gpio_put(clockPin, 1);
	}

	// Set latch pin high to end the transmission
	gpio_put(latchPin, 1);
}