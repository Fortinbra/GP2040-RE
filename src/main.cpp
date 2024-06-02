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
#include "headers/I2CHandler.h"

SNESController *snes = SNESController::getInstance();
I2CHandler i2cHandler;

void processI2CData()
{
    GamepadState gpState;
    uint32_t *data = reinterpret_cast<uint32_t *>(&gpState);
    for (size_t i = 0; i < sizeof(GamepadState) / sizeof(uint32_t); i++)
    {
        data[i] = multicore_fifo_pop_blocking();
    }
    snes->sendToSystem(gpState);
}

void core1()
{
    multicore_lockout_victim_init(); // block core 1
    i2cHandler.setup_slave();
    while (true)
    {
        tight_loop_contents();
    };
}

int main()
                  {
    stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    multicore_launch_core1(core1);
    snes->Setup(GREEN_ETHERNET_PIN, BROWN_ETHERNET_PIN, BLUE_ETHERNET_PIN);
    printf("Hello!\n");
    while (true)
    {
        processI2CData();
        // if (i2cHandler.isBusStalled())
        // {
        //     i2cHandler.clearI2CBus();
        // }
    };
}