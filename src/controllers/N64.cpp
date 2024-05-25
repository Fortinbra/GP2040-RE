#include "headers/N64.h"
#include <stdio.h>
#include "pico/stdlib.h"

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
	n64Data <<= 8;										   // Shift the button data left by 8 bits to make room for the joystick data
	n64Data |= reverseBits(scaleJoystick(data.lx) & 0xFF); // Add the X-axis joystick data
	n64Data <<= 8;										   // Shift the data left by 8 bits again to make room for more joystick data
	n64Data |= reverseBits(scaleJoystick(data.ly) & 0xFF); // Add the Y-axis joystick data
	N64ControllerState = n64Data;
	return n64Data;
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
uint8_t N64::reverseBits(uint8_t num)
{
	uint8_t NO_OF_BITS = sizeof(num) * 8;
	uint8_t reverse_num = 0;
	int i;
	for (i = 0; i < NO_OF_BITS; i++)
	{
		if ((num & (1 << i)))
			reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
	}
	return reverse_num;
}
void N64::listenForCommands()
{
	// Set the data pin to input mode to listen for commands
	gpio_set_dir(dataPin, GPIO_IN);

	while (true) // Keep listening for commands indefinitely
	{
		// Wait for the start bit
		while (gpio_get(dataPin) == 1)
		{
		}

		// Wait for the line to go high again
		sleep_us(2);
		if (gpio_get(dataPin) == 0)
			continue;

		// Receive the command
		uint8_t command = receiveCommand();

		// Respond to the command
		switch (command)
		{
		case N64Command::RESETANDINFO:
			resetController();
			sendControllerInfo();
			break;
		case N64Command::INFO:
			sendControllerInfo();
			break;
		case N64Command::CONTROLLER_STATE:
			sendControllerState();
			break;
		case N64Command::READ_CONTROLLER_ACCESSORY:
			// handle READ_CONTROLLER_ACCESSORY command
			break;
		case N64Command::WRITE_CONTROLLER_ACCESSORY:
			// handle WRITE_CONTROLLER_ACCESSORY command
			break;
		case N64Command::READ_EEPROM:
			// handle READ_EEPROM command
			break;
		case N64Command::WRITE_EEPROM:
			// handle WRITE_EEPROM command
			break;
		case N64Command::RTC_INFO:
			// handle RTC_INFO command
			break;
		case N64Command::READ_RTC_BLOCK:
			// handle READ_RTC_BLOCK command
			break;
		case N64Command::WRITE_RTC_BLOCK:
			// handle WRITE_RTC_BLOCK command
			break;
		}
	}
}
void N64::sendToSystem(uint8_t data)
{
}
void N64::resetController()
{
	// Reset the joystick to 0,0 position
	N64ControllerState &= ~(JOYSTICK_X_MASK | JOYSTICK_Y_MASK);
}
void N64::sendControllerInfo()
{
	uint8_t info[3];

	// First two bytes for controller
	info[0] = 0x05;
	info[1] = 0x00;

	// Third byte for pack installed
	info[2] = 0x01;

	// Send the info to the system
	for (int i = 0; i < 3; i++)
	{
		sendToSystem(info[i]);
	}
}
void N64::sendControllerState(){
	
}