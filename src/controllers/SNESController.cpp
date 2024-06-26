#include "headers/SNESController.h"
#include "headers/GamepadState.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include <string>
#include "pico/time.h"
#include <pico/multicore.h>
#include "hardware/regs/sio.h"

// Initialize pointer to zero so that it can be initialized in first call to getInstance
SNESController *SNESController::instance = 0;

SNESController *SNESController::getInstance()
{
    if (instance == 0)
    {
        instance = new SNESController();
    }
    return instance;
}

SNESController::SNESController()
{
    // constructor body
}

void SNESController::Setup(int latchPinInput, int clockPinInput, int dataPinInput)
{
    dataPin = dataPinInput;
    clockPin = clockPinInput;
    latchPin = latchPinInput;
    snesState = 0x00;
    gpio_init(dataPin);
    gpio_init(clockPin);

    gpio_set_dir(dataPin, GPIO_OUT);
    gpio_set_dir(clockPin, GPIO_IN);

    // Set up the latch pin as an input and enable the pull-down resistor
    gpio_set_dir(latchPinInput, GPIO_IN);

    gpio_set_irq_enabled_with_callback(latchPin, GPIO_IRQ_EDGE_RISE, true, &SNESController::gpioIrqHandler);
}
void SNESController::gpioIrqHandler(uint gpio, uint32_t events)
{
    // printf("Latch interrupt hit\n");
    int snesLatchedState = instance->snesState;

    gpio_put(instance->dataPin, ((snesLatchedState & buttonOrder[0]) != 0));
    for (int i = 1; i < 16; i++)
    {
        // Wait for the falling edge of the clock pin
        while (gpio_get(instance->clockPin) == 1)
        {
            tight_loop_contents();
        }
        // Wait for the rising edge of the clock pin
        while (gpio_get(instance->clockPin) == 0)
        {
            tight_loop_contents();
        }
        // Set the data pin to the current bit value
        if (i < 12)
        {
            int buttonValue = (snesLatchedState & buttonOrder[i]) != 0;
            gpio_put(instance->dataPin, buttonValue);
        }
        else
        {
            // Unknown buttons always report high
            gpio_put(instance->dataPin, 1);
        }
    }
    gpio_put(instance->dataPin, 0);
}
void SNESController::sendToSystem(GamepadState data)
{
    GamepadState gamepadState = data;
    if ((gamepadState.dpad & GAMEPAD_MASK_UP) == GAMEPAD_MASK_UP)
    {
        snesState |= SNES_BUTTON_UP;
        printf("UP\n");
    }
    if ((gamepadState.dpad & GAMEPAD_MASK_DOWN) == GAMEPAD_MASK_DOWN)
    {
        snesState |= SNES_BUTTON_DOWN;
        printf("DOWN\n");
    }
    if ((gamepadState.dpad & GAMEPAD_MASK_LEFT) == GAMEPAD_MASK_LEFT)
    {
        snesState |= SNES_BUTTON_LEFT;
        printf("LEFT\n");
    }
    if ((gamepadState.dpad & GAMEPAD_MASK_RIGHT) == GAMEPAD_MASK_RIGHT)
    {
        snesState |= SNES_BUTTON_RIGHT;
        printf("RIGHT\n");
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_B1) == GAMEPAD_MASK_B1)
    {
        snesState |= SNES_BUTTON_A;
        printf("A\n");
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_B2) == GAMEPAD_MASK_B2)
    {
        snesState |= SNES_BUTTON_B;
        printf("B\n");
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_B3) == GAMEPAD_MASK_B3)
    {
        snesState |= SNES_BUTTON_X;
        printf("X\n");
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_B4) == GAMEPAD_MASK_B4)
    {
        snesState |= SNES_BUTTON_Y;
        printf("Y\n");
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_L1) == GAMEPAD_MASK_L1)
    {
        snesState |= SNES_BUTTON_L;
        printf("L\n");
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_R1) == GAMEPAD_MASK_R1)
    {
        snesState |= SNES_BUTTON_R;
        printf("R\n");
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_S1) == GAMEPAD_MASK_S1)
    {
        snesState |= SNES_BUTTON_SELECT;
        printf("SELECT\n");
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_S2) == GAMEPAD_MASK_S2)
    {
        snesState |= SNES_BUTTON_START;
        printf("START\n");
    }
}