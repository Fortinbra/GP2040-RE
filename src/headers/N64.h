// #ifndef N64_H
// #define N64_H

// #include "IController.h"
// const uint16_t N64_BUTTON_A = 0x8000;
// const uint16_t N64_BUTTON_B = 0x4000;
// const uint16_t N64_BUTTON_Z = 0x2000;
// const uint16_t N64_BUTTON_START = 0x1000;
// const uint16_t N64_BUTTON_DPAD_UP = 0x0800;
// const uint16_t N64_BUTTON_DPAD_DOWN = 0x0400;
// const uint16_t N64_BUTTON_DPAD_LEFT = 0x0200;
// const uint16_t N64_BUTTON_DPAD_RIGHT = 0x0100;
// const uint16_t N64_BUTTON_RESERVED = 0x0080; // Reserved, always 0
// const uint16_t N64_BUTTON_L = 0x0020;
// const uint16_t N64_BUTTON_R = 0x0010;
// const uint16_t N64_BUTTON_C_UP = 0x0008;
// const uint16_t N64_BUTTON_C_DOWN = 0x0004;
// const uint16_t N64_BUTTON_C_LEFT = 0x0002;
// const uint16_t N64_BUTTON_C_RIGHT = 0x0001;

// #define JOYSTICK_X_MASK 0x00FF // Last 8 bits
// #define JOYSTICK_Y_MASK 0xFF00 // Second to last 8 bits
// #define PULSE_LENGTH_THRESHOLD 4

// enum N64Command
// {
// 	RESETANDINFO = 0xFF,
// 	INFO = 0x00,
// 	CONTROLLER_STATE = 0x01,
// 	READ_CONTROLLER_ACCESSORY = 0x02,
// 	WRITE_CONTROLLER_ACCESSORY = 0x03,
// 	READ_EEPROM = 0x04,
// 	WRITE_EEPROM = 0x05,
// 	RTC_INFO = 0x06,
// 	READ_RTC_BLOCK = 0x07,
// 	WRITE_RTC_BLOCK = 0x08,
// };

// class N64
// {
// public:
// 	N64(int dataPin);

// private:
// 	int dataPin;
// 	uint16_t N64ControllerState;
// 	uint16_t translateToFormat(GamepadState data);
// 	uint32_t recieveFromSystem();
// 	int scaleJoystick(int gamepadValue);
// 	uint8_t reverseBits(uint8_t num);
// 	void listenForCommands();
// 	void resetController();
// 	void sendControllerInfo();
// 	void sendControllerState();
// 	void sendToSystem(uint8_t data);
// 	uint8_t N64::readByte()
// };

// #endif // N64_H