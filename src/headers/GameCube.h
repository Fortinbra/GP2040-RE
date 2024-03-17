#ifndef GAMECUBE_H
#define GAMECUBE_H

#include "IController.h"

class GameCube : public IController {
public:
    GameCube(int latchPin, int clockPin, int dataPin);

    uint16_t translateToFormat(I2CData *data) override;

    void sendToSystem(uint16_t data) override;

private:
    int latchPin;
    int clockPin;
    int dataPin;
};

#endif // GAMECUBE_H