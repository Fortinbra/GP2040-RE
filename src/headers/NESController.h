#ifndef NESCONTROLLER_H
#define NESCONTROLLER_H

#include "I2CData.h"
#include "IController.h"

class NESController : public IController
{
public:
	NESController(int latchPin, int clockPin, int dataPin);

	uint16_t translateToFormat(I2CData *data) override;

	void sendToSystem(uint16_t data) override;

private:
	int latchPin;
	int clockPin;
	int dataPin;
};

#endif // NESCONTROLLER_H