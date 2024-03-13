#ifndef PS1CONTROLLER_H
#define PS1CONTROLLER_H

#include "I2CData.h"
#include "IController.h"

class PS1Controller : public IController
{
public:
    PS1Controller(int commandPin, int dataPin, int clockPin, int attentionPin);

    uint16_t translateToFormat(I2CData *data) override;

    void sendToSystem(uint16_t data) override;

private:
    int commandPin;
    int attentionPin;
    int dataPin;
    int clockPin;
};

#endif // PS1CONTROLLER_H