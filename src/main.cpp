#include <stdio.h>
#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include <pico/stdlib.h>
#include <string.h>
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#include "headers/GamepadState.h"
#include "headers/I2CData.h"
#include "headers/NESController.h"
#include "headers/SNESController.h"
#include "headers/N64.h"
#include <pico/multicore.h>
#include "hardware/uart.h"
#include <cstdlib>
#include "headers/config.h"
#include "headers/Console.h"

Console detectedConsole;
SNESController *snes = SNESController::getInstance();

void processI2CData()
{
    GamepadState gpState;
    uint32_t *data = reinterpret_cast<uint32_t *>(&gpState);
    for (size_t i = 0; i < sizeof(GamepadState) / sizeof(uint32_t); i++)
    {
        data[i] = multicore_fifo_pop_blocking();
    }
    switch (detectedConsole)
    {
    case Console::CONSOLE_SNES:
        snes->sendToSystem(gpState);
        printf("AFTER SNES\n");
        break;
    default:
        // Do nothing, we don't know what console we're dealing with.
        break;
    }
}

void handleStatus(uint8_t *payload)
{
    (void)0;
}

void handleMessage(uint8_t *payload)
{
    (void)0;
}

static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event)
{
    static uint8_t buf[GPCOMMS_BUFFER_SIZE] = {0};
    static uint8_t receivedIndex = 0;

    switch (event)
    {
    case I2C_SLAVE_RECEIVE:
        buf[receivedIndex] = i2c_read_byte_raw(i2c);
        receivedIndex++;
        break;
    case I2C_SLAVE_REQUEST:
        // TODO: Do something useful?!?
        break;
    case I2C_SLAVE_FINISH:
    {
        uint8_t command = buf[0];
        uint8_t *payload = &buf[1];

        switch (command)
        {
        case GPCMD_STATE:
        {
            static GPComms_State gpState = {};
            memcpy(&gpState, payload, sizeof(GPComms_State));
            GamepadState gamepadState = gpState.gamepadState;
            {
                uint32_t *data = reinterpret_cast<uint32_t *>(&gamepadState);
                for (size_t i = 0; i < sizeof(GamepadState) / sizeof(uint32_t); i++)
                {
                    multicore_fifo_push_blocking(data[i]);
                }
            }
            break;
        }
        default:
            break;
        }
        receivedIndex = 0;
        break;
    }
    default:
        break;
    }
}
void setup_slave()
{
    gpio_init(I2C_SLAVE_SDA_PIN);
    gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SDA_PIN);

    gpio_init(I2C_SLAVE_SCL_PIN);
    gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SCL_PIN);

    i2c_init(i2c0, I2C_BAUDRATE);
    i2c_slave_init(i2c0, I2C_SLAVE_ADDR, &i2c_slave_handler);
}
void core1()
{
    multicore_lockout_victim_init(); // block core 1
    setup_slave();
    while (true)
    {
    };
}
const char *consoleToString(Console console)
{
    switch (console)
    {
    case Console::CONSOLE_NES:
        return "NES";
    case Console::CONSOLE_SNES:
        return "SNES";
    case Console::CONSOLE_N64:
        return "N64";
    case Console::CONSOLE_GC:
        return "GameCube";
    case Console::CONSOLE_PS1:
        return "PS1";
    case Console::CONSOLE_PS2:
        return "PS2";
    case Console::CONSOLE_DC:
        return "Dreamcast";
    default:
        return "Unknown";
    }
}
// int measureFrequency(int pin)
// {
//     gpio_init(pin);
//     gpio_set_dir(pin, GPIO_IN);
//     // Count the number of rising edges over a period of one second
//     int count = 0;
//     bool lastState = gpio_get(pin);
//     absolute_time_t endTime = make_timeout_time_ms(1000);
//     while (!time_reached(endTime))
//     {
//         bool state = gpio_get(pin);
//         if (state && !lastState)
//         {
//             count++;
//         }
//         lastState = state;
//     }
//     // The count is the frequency in Hz
//     return count;
// }

Console detectConsole()
{
    // Measure the frequency of the clock signal
    // int frequency = measureFrequency(BROWN_ETHERNET_PIN);

    // // Print the measured frequency to the UART
    // printf("Measured frequency: %d Hz\n", frequency);

    // // Compare the measured frequency to the lookup table
    // for (int i = 0; i < sizeof(consoleLookupTable) / sizeof(ConsoleLookup); i++)
    // {
    //     if (consoleLookupTable[i].frequency == frequency)
    //     {
    //         return consoleLookupTable[i].console;
    //     }
    // }

    // If no match was found in the lookup table, return unknown
    return Console::CONSOLE_SNES;
}
int main()
{
    stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    multicore_launch_core1(core1);

    // Detect the console
    detectedConsole = detectConsole();
    printf("Detected console: %s\n", consoleToString(detectedConsole));
    switch (detectedConsole)
    {
    case Console::CONSOLE_SNES:
        snes->Setup(GREEN_ETHERNET_PIN, BROWN_ETHERNET_PIN, BLUE_ETHERNET_PIN);
        break;
    default:
        // Do nothing, we don't know what console we're dealing with.
        break;
    }

    while (true)
    {
        processI2CData();
    };
}