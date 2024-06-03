#ifndef NESCONTROLLER_H
#define NESCONTROLLER_H

#include "I2CData.h"
#include "IController.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const uint8_t NES_BUTTON_A = 0x01;
const uint8_t NES_BUTTON_B = 0x02;
const uint8_t NES_BUTTON_SELECT = 0x40;
const uint8_t NES_BUTTON_START = 0x80;
const uint8_t NES_BUTTON_UP = 0x100;
const uint8_t NES_BUTTON_DOWN = 0x200;
const uint8_t NES_BUTTON_LEFT = 0x400;
const uint8_t NES_BUTTON_RIGHT = 0x800;
const uint8_t buttonOrder[8] = {
    NES_BUTTON_A,
    NES_BUTTON_B,
    NES_BUTTON_SELECT,
    NES_BUTTON_START,
    NES_BUTTON_UP,
    NES_BUTTON_DOWN,
    NES_BUTTON_LEFT,
    NES_BUTTON_RIGHT};
class NESController
{
public:
    static NESController *getInstance();
    void Setup(int latchPin, int clockPinInput, int dataPinInput);
    void sendToSystem(GamepadState data);
    static void gpioIrqHandler(uint gpio, uint32_t events);

private:
    NESController();
    static NESController *instance;
    uint dataPin;
    uint clockPin;
    uint latchPin;
    uint8_t nesState;
};

#endif // SNESCONTROLLER_H