#ifndef SNESCONTROLLER_H
#define SNESCONTROLLER_H

#include "I2CData.h"
#include "IController.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const uint16_t SNES_BUTTON_A = 0x01;
const uint16_t SNES_BUTTON_B = 0x02;
const uint16_t SNES_BUTTON_X = 0x04;
const uint16_t SNES_BUTTON_Y = 0x08;
const uint16_t SNES_BUTTON_L = 0x10;
const uint16_t SNES_BUTTON_R = 0x20;
const uint16_t SNES_BUTTON_SELECT = 0x40;
const uint16_t SNES_BUTTON_START = 0x80;
const uint16_t SNES_BUTTON_UP = 0x100;
const uint16_t SNES_BUTTON_DOWN = 0x200;
const uint16_t SNES_BUTTON_LEFT = 0x400;
const uint16_t SNES_BUTTON_RIGHT = 0x800;
const uint16_t buttonOrder[16] = {
    SNES_BUTTON_B,
    SNES_BUTTON_Y,
    SNES_BUTTON_SELECT,
    SNES_BUTTON_START,
    SNES_BUTTON_UP,
    SNES_BUTTON_DOWN,
    SNES_BUTTON_LEFT,
    SNES_BUTTON_RIGHT,
    SNES_BUTTON_A,
    SNES_BUTTON_X,
    SNES_BUTTON_L,
    SNES_BUTTON_R,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF // Unknown buttons always report high
};
class SNESController
{
public:
    static SNESController *getInstance();
    void Setup(int latchPin, int clockPinInput, int dataPinInput);
    void sendToSystem(GamepadState data);
    static void gpioIrqHandler(uint gpio, uint32_t events);
    static void clockIRQ(uint gpio, uint32_t events);
    static void latchIRQ(uint gpio, uint32_t events);

private:
    SNESController();
    static SNESController *instance;
    uint dataPin;
    uint clockPin;
    uint latchPin;
    uint16_t snesState;
    int currentBit;
    bool latchPinHigh;
};

#endif // SNESCONTROLLER_H