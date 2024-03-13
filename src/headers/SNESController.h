#ifndef SNESCONTROLLER_H
#define SNESCONTROLLER_H

#include "I2CData.h"
#include "IController.h"

class SNESController : public IController
{
public:
    SNESController(int latchPin, int clockPin, int dataPin);

    uint16_t translateToFormat(I2CData *data) override;

    void sendToSystem(uint16_t data) override;

private:
    int latchPin;
    int clockPin;
    int dataPin;
};

#endif // SNESCONTROLLER_H