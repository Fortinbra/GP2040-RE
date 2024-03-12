#ifndef NESCONTROLLER_H
#define NESCONTROLLER_H

#include "I2CData.h"

class NESController
{
public:
	NESController(int latchPin, int clockPin, int dataPin);

	uint8_t translateToNesFormat(I2CData *data);

	void sendToNesSystem(uint8_t data);

private:
	int latchPin;
	int clockPin;
	int dataPin;
};

#endif // NESCONTROLLER_H