#include "headers/PS1Controller.h"

PS1Controller::PS1Controller(int commandPin, int dataPin, int clockPin, int attentionPin)
    : commandPin(commandPin), dataPin(dataPin), clockPin(clockPin), attentionPin(attentionPin) {}
uint16_t PS1Controller::translateToFormat(I2CData *data)
{
    // Get the GamepadState from the State property of the I2CData object
    GamepadState gamepadState = data->state;

    // Initialize a 16-bit word with all bits set to 1
    // The PS1 controller uses a negative logic, where a pressed button corresponds to a bit set to 0
    uint16_t ps1Buttons = 0xFFFF;

    // Map the GamepadState buttons to the PS1 buttons
    if ((gamepadState.buttons & GAMEPAD_BUTTON_A) == GAMEPAD_BUTTON_A)
    {
        ps1Buttons &= ~PS1_BUTTON_CROSS;
    }
    if ((gamepadState.buttons & GAMEPAD_BUTTON_B) == GAMEPAD_BUTTON_B)
    {
        ps1Buttons &= ~PS1_BUTTON_CIRCLE;
    }
    // Continue for other buttons...

    return ps1Buttons;
}

void PS1Controller::sendToSystem(uint16_t data)
{
    // Implement the PS1 controller communication protocol
    // This will involve setting the attention pin low, sending the command to the controller, clocking the data in and out, and setting the attention pin high again
}