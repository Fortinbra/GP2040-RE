#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "I2CData.h"

class IController
{
public:
    virtual ~IController() {}

    virtual void sendToSystem(GamepadState data) = 0;

private:
    virtual uint16_t translateToFormat(GamepadState data) = 0;
};

#endif // ICONTROLLER_H