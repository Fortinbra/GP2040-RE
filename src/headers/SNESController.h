#ifndef SNESCONTROLLER_H
#define SNESCONTROLLER_H

#include "I2CData.h"
#include "IController.h"

class SNESController : public IController
{
public:
    SNESController(int latchPin, int clockPin, int dataPin);

    void sendToSystem(GamepadState data) override;

private:
    int latchPin;
    int clockPin;
    int dataPin;
    uint16_t translateToFormat(GamepadState data) override;
};

#endif // SNESCONTROLLER_H