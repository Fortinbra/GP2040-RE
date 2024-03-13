#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "I2CData.h"

class IController
{
public:
    virtual ~IController() {}

    virtual uint16_t translateToFormat(I2CData *data) = 0;

    virtual void sendToSystem(uint16_t data) = 0;
};

#endif // ICONTROLLER_H