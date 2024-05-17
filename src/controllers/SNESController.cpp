#include "headers/SNESController.h"
#include "headers/GamepadState.h"

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

SNESController::SNESController(int latchPin, int clockPin, int dataPin)
    : latchPin(latchPin), clockPin(clockPin), dataPin(dataPin) {}

uint16_t SNESController::translateToFormat(GamepadState data)

{
    // Get the GamepadState from the State property of the I2CData object
    GamepadState gamepadState = data;
    uint16_t snesPad = 0x00;
    if ((gamepadState.dpad & GAMEPAD_MASK_UP) == GAMEPAD_MASK_UP)
    {
        snesPad |= SNES_BUTTON_UP;
    }
    if ((gamepadState.dpad & GAMEPAD_MASK_DOWN) == GAMEPAD_MASK_DOWN)
    {
        snesPad |= SNES_BUTTON_DOWN;
    }
    if ((gamepadState.dpad & GAMEPAD_MASK_LEFT) == GAMEPAD_MASK_LEFT)
    {
        snesPad |= SNES_BUTTON_LEFT;
    }
    if ((gamepadState.dpad & GAMEPAD_MASK_RIGHT) == GAMEPAD_MASK_RIGHT)
    {
        snesPad |= SNES_BUTTON_RIGHT;
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_B1) == GAMEPAD_MASK_B1)
    {
        snesPad |= SNES_BUTTON_A;
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_B2) == GAMEPAD_MASK_B2)
    {
        snesPad |= SNES_BUTTON_B;
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_B3) == GAMEPAD_MASK_B3)
    {
        snesPad |= SNES_BUTTON_X;
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_B4) == GAMEPAD_MASK_B4)
    {
        snesPad |= SNES_BUTTON_Y;
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_L1) == GAMEPAD_MASK_L1)
    {
        snesPad |= SNES_BUTTON_L;
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_R1) == GAMEPAD_MASK_R1)
    {
        snesPad |= SNES_BUTTON_R;
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_S1) == GAMEPAD_MASK_S1)
    {
        snesPad |= SNES_BUTTON_SELECT;
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_S2) == GAMEPAD_MASK_S2)
    {
        snesPad |= SNES_BUTTON_START;
    }
    return snesPad;
}
void SNESController::sendToSystem(GamepadState data)
{
    uint16_t snesState = translateToFormat(data);

    // Set the latch pin low to start the transmission
    gpio_put(latchPin, 0);
    // Shift out the data bits

    for (int i = 0; i < 16; i++)
    {
        // Set the clock pin low
        gpio_put(clockPin, 0);
        // Set the data pin to the current bit value
        gpio_put(dataPin, (snesState >> i) & 1);
        // Set the clock pin high to shift the bit
        gpio_put(clockPin, 1);
    }
    // Set the latch pin high to end the transmission
    gpio_put(latchPin, 1);
}