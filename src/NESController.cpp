#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#include "headers/I2CData.h"
#include "headers/NESController.h"
#include "headers/GamepadState.h"

const uint8_t NES_BUTTON_A = 0x01;
const uint8_t NES_BUTTON_B = 0x02;
const uint8_t NES_BUTTON_SELECT = 0x04;
const uint8_t NES_BUTTON_START = 0x08;
const uint8_t NES_BUTTON_UP = 0x10;
const uint8_t NES_BUTTON_DOWN = 0x20;
const uint8_t NES_BUTTON_LEFT = 0x40;
const uint8_t NES_BUTTON_RIGHT = 0x80;

NESController::NESController(int latchPin, int clockPin, int dataPin)
    : latchPin(latchPin), clockPin(clockPin), dataPin(dataPin)
{
    // printf("Latch Pin: %d\n", latchPin);
    // printf("Clock Pin: %d\n", clockPin);
    // printf("Data Pin: %d\n", dataPin);
}

uint16_t NESController::translateToFormat(GamepadState data)
{

    // Get the GamepadState from the State property of the I2CData object

    GamepadState gamepadState = data;

    // Print the GamepadState
    printf("GamepadState: dpad=%02X, buttons=%02X\n", gamepadState.dpad, gamepadState.buttons);

    // Convert the GamepadState D-pad to NES D-pad
    uint16_t nesPad = 0x00;
    if (gamepadState.dpad & GAMEPAD_MASK_UP)
    {
        nesPad |= NES_BUTTON_UP;
        gpio_put(15, true);
    }
    if (gamepadState.dpad & GAMEPAD_MASK_DOWN)
    {
        nesPad |= NES_BUTTON_DOWN;
        gpio_put(15, true);
    }
    if (gamepadState.dpad & GAMEPAD_MASK_LEFT)
    {
        nesPad |= NES_BUTTON_LEFT;
        gpio_put(15, true);
    }
    if (gamepadState.dpad & GAMEPAD_MASK_RIGHT)
    {
        nesPad |= NES_BUTTON_RIGHT;
        gpio_put(15, true);
    }
    if (gamepadState.buttons & GAMEPAD_MASK_B1)
    {
        nesPad |= NES_BUTTON_A;
        gpio_put(15, true);
    }
    if (gamepadState.buttons & GAMEPAD_MASK_B2)
    {
        nesPad |= NES_BUTTON_B;
        gpio_put(15, true);
    }
    if (gamepadState.buttons & GAMEPAD_MASK_S1)
    {
        nesPad |= NES_BUTTON_SELECT;
        gpio_put(15, true);
    }
    if (gamepadState.buttons & GAMEPAD_MASK_S2)
    {
        nesPad |= NES_BUTTON_START;
        gpio_put(15, true);
    }

    // Combine the NES D-pad and buttons
    uint16_t nesData = nesPad;

    printf("NES Data: %04X\n", nesData);

    // Return the translated NES data
    return nesData;
}

void NESController::sendToSystem(uint16_t data)
{
    // Set the latch pin low to start the transmission
    gpio_put(latchPin, 0);
    // Shift out the data bits
    for (int i = 0; i < 8; i++)
    {
        // Set the clock pin low
        gpio_put(clockPin, 0);
        // Set the data pin to the current bit value
        gpio_put(dataPin, (data >> i) & 1);
        // Set the clock pin high to shift the bit
        gpio_put(clockPin, 1);
    }
    // Set the latch pin high to end the transmission
    gpio_put(latchPin, 1);
}
