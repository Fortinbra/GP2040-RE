#ifndef N64_H
#define N64_H

#include "IController.h"

class N64 : public IController
{
public:
	N64(int dataPin);

	void sendToSystem(GamepadState data) override;

private:
	int dataPin;
	uint16_t translateToFormat(GamepadState data) override;
	uint32_t recieveFromSystem();
	int scaleJoystick(int gamepadValue);
};

#endif // N64_H