#ifndef N64_H
#define N64_H

#include "IController.h"

class N64 : public IController {
public:
	N64(int latchPin, int clockPin, int dataPin);

	uint16_t translateToFormat(I2CData *data) override;

	void sendToSystem(uint16_t data) override;

private:
	int latchPin;
	int clockPin;
	int dataPin;
};

#endif // N64_H