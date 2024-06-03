#include "headers/NESController.h"
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
NESController *NESController::instance = 0;

NESController *NESController::getInstance()
{
    if (instance == 0)
    {
        instance = new NESController();
    }
    return instance;
}

NESController::NESController()
{
    // constructor body
}

void NESController::Setup(int latchPinInput, int clockPinInput, int dataPinInput)
{
    dataPin = dataPinInput;
    clockPin = clockPinInput;
    latchPin = latchPinInput;
    nesState = 0x00;
    gpio_init(dataPin);
    gpio_init(clockPin);

    gpio_set_dir(dataPin, GPIO_OUT);
    gpio_set_dir(clockPin, GPIO_IN);

    // Set up the latch pin as an input and enable the pull-down resistor
    gpio_set_dir(latchPinInput, GPIO_IN);

    gpio_set_irq_enabled_with_callback(latchPin, GPIO_IRQ_EDGE_RISE, true, &NESController::gpioIrqHandler);
}
void NESController::gpioIrqHandler(uint gpio, uint32_t events)
{
    // printf("Latch interrupt hit\n");
    int nesLatchedState = instance->nesState;

    gpio_put(instance->dataPin, ((nesLatchedState & buttonOrder[0]) != 0));
    for (int i = 1; i < 8; i++)
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
            int buttonValue = (nesLatchedState & buttonOrder[i]) != 0;
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
void NESController::sendToSystem(GamepadState data)
{
    GamepadState gamepadState = data;
    if ((gamepadState.dpad & GAMEPAD_MASK_UP) == GAMEPAD_MASK_UP)
    {
        nesState |= NES_BUTTON_UP;
        printf("UP\n");
    }
    if ((gamepadState.dpad & GAMEPAD_MASK_DOWN) == GAMEPAD_MASK_DOWN)
    {
        nesState |= NES_BUTTON_DOWN;
        printf("DOWN\n");
    }
    if ((gamepadState.dpad & GAMEPAD_MASK_LEFT) == GAMEPAD_MASK_LEFT)
    {
        nesState |= NES_BUTTON_LEFT;
        printf("LEFT\n");
    }
    if ((gamepadState.dpad & GAMEPAD_MASK_RIGHT) == GAMEPAD_MASK_RIGHT)
    {
        nesState |= NES_BUTTON_RIGHT;
        printf("RIGHT\n");
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_B1) == GAMEPAD_MASK_B1)
    {
        nesState |= NES_BUTTON_A;
        printf("A\n");
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_B2) == GAMEPAD_MASK_B2)
    {
        nesState |= NES_BUTTON_B;
        printf("B\n");
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_S1) == GAMEPAD_MASK_S1)
    {
        nesState |= NES_BUTTON_SELECT;
        printf("SELECT\n");
    }
    if ((gamepadState.buttons & GAMEPAD_MASK_S2) == GAMEPAD_MASK_S2)
    {
        nesState |= NES_BUTTON_START;
        printf("START\n");
    }
}