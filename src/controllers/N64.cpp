#include "headers/N64.h"
#include <SPI.h>

const uint16_t N64_BUTTON_A = 0x8000;
const uint16_t N64_BUTTON_B = 0x4000;
const uint16_t N64_BUTTON_Z = 0x2000;
const uint16_t N64_BUTTON_START = 0x1000;
const uint16_t N64_BUTTON_DPAD_UP = 0x0800;
const uint16_t N64_BUTTON_DPAD_DOWN = 0x0400;
const uint16_t N64_BUTTON_DPAD_LEFT = 0x0200;
const uint16_t N64_BUTTON_DPAD_RIGHT = 0x0100;
const uint16_t N64_BUTTON_RESERVED = 0x0080; // Reserved, always 0
const uint16_t N64_BUTTON_L = 0x0020;
const uint16_t N64_BUTTON_R = 0x0010;
const uint16_t N64_BUTTON_C_UP = 0x0008;
const uint16_t N64_BUTTON_C_DOWN = 0x0004;
const uint16_t N64_BUTTON_C_LEFT = 0x0002;
const uint16_t N64_BUTTON_C_RIGHT = 0x0001;

N64::N64(int dataPin)
	: dataPin(dataPin)
{
	gpio_init(dataPin);
	gpio_set_dir(dataPin, GPIO_IN);
}

uint16_t N64::translateToFormat(GamepadState data)
{
	uint16_t n64Data = 0;

	// Map each button in the GamepadState to the corresponding bit in the N64 format
	if (data.buttons & GAMEPAD_MASK_B1)
	{
		n64Data |= N64_BUTTON_A;
	}
	if (data.buttons & GAMEPAD_MASK_B2)
	{
		n64Data |= N64_BUTTON_B;
	}
	if (data.buttons & GAMEPAD_MASK_B3)
	{
		n64Data |= N64_BUTTON_Z;
	}
	if (data.buttons & GAMEPAD_MASK_L1)
	{
		n64Data |= N64_BUTTON_L;
	}
	if (data.buttons & GAMEPAD_MASK_R1)
	{
		n64Data |= N64_BUTTON_R;
	}
	if (data.buttons & GAMEPAD_MASK_S1)
	{
		n64Data |= N64_BUTTON_START;
	}
	if (data.dpad & GAMEPAD_MASK_DOWN)
	{
		n64Data |= N64_BUTTON_DPAD_DOWN;
	}
	if (data.dpad & GAMEPAD_MASK_UP)
	{
		n64Data |= N64_BUTTON_DPAD_UP;
	}
	if (data.dpad & GAMEPAD_MASK_RIGHT)
	{
		n64Data |= N64_BUTTON_DPAD_RIGHT;
	}
	if (data.dpad & GAMEPAD_MASK_LEFT)
	{
		n64Data |= N64_BUTTON_DPAD_LEFT;
	}

	// Add the joystick data
	n64Data <<= 8;								// Shift the button data left by 8 bits to make room for the joystick data
	n64Data |= (scaleJoystick(data.lx) & 0xFF); // Add the X-axis joystick data
	n64Data <<= 8;								// Shift the data left by 8 bits again to make room for more joystick data
	n64Data |= (scaleJoystick(data.ly) & 0xFF); // Add the Y-axis joystick data

	return n64Data;
}

void N64::sendToSystem(uint16_t data)
{
	gpio_set_dir(dataPin, GPIO_OUT);
	for (int i = 15; i >= 0; i--)
	{
		gpio_put(dataPin, (data >> i) & 1);
		sleep_us(4);
	}
	gpio_set_dir(dataPin, GPIO_IN);
}

int N64::scaleJoystick(int gamepadValue)
{
	// Subtract the midpoint of the gamepad range to center the value around 0
	int centeredValue = gamepadValue - GAMEPAD_JOYSTICK_MID;

	// Scale the centered value to the N64 range
	int n64Value = centeredValue * 80 / GAMEPAD_JOYSTICK_MID;

	// Clamp the value to the N64 range
	if (n64Value < -80)
	{
		n64Value = -80;
	}
	else if (n64Value > 80)
	{
		n64Value = 80;
	}

	return n64Value;
}

uint32_t N64::recieveFromSystem()
{
	uint32_t data = 0;

	// Set the data pin to input
	gpio_set_dir(dataPin, GPIO_IN);

	// Read each bit of the data
	for (int i = 31; i >= 0; i--)
	{
		// Wait for the appropriate amount of time
		sleep_us(4); // The N64 controller protocol uses a bit rate of 250 kbit/s, which corresponds to 4 microseconds per bit

		// Read the bit from the data pin
		uint32_t bit = gpio_get(dataPin);

		// Add the bit to the data
		data |= (bit << i);
	}
	// If the data is memory card data, write it to the SD card
	if (isMemoryCardData(data))
	{
		// Open the file on the SD card
		// File file = SD.open("memoryCardData.bin", FILE_WRITE);

		// // Write the data to the file
		// file.write((uint8_t *)&data, sizeof(data));

		// // Close the file
		// file.close();
	}

	return data;
}
bool isMemoryCardData(uint32_t data)
{
	// Check the command byte to determine if the data is memory card data
	// This is a placeholder - replace with the actual check
	// return (data & 0xFF000000) == MEMORY_CARD_COMMAND;
}