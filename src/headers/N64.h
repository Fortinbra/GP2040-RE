#ifndef N64_H
#define N64_H

#include "IController.h"

class N64 : public IController
{
public:
	N64(int dataPin);

	uint16_t translateToFormat(GamepadState data) override;

	void sendToSystem(uint16_t data) override;

	uint32_t recieveFromSystem();

	int scaleJoystick(int gamepadValue);

private:
	int dataPin;
};

#endif // N64_H