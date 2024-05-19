#ifndef NESCONTROLLER_H
#define NESCONTROLLER_H

#include "I2CData.h"
#include "IController.h"

class NESController : public IController
{
public:
	NESController(int latchPin, int clockPin, int dataPin);

	void sendToSystem(GamepadState data) override;

private:
	int latchPin;
	int clockPin;
	int dataPin;
	
	uint16_t translateToFormat(GamepadState data) override;
};

#endif // NESCONTROLLER_H