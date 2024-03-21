#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#include "headers/GamepadState.h"
#include "headers/I2CData.h"
#include "headers/NESController.h"
#include "headers/SNESController.h"
#include <pico/multicore.h>

#define I2C_SLAVE_ADDR 0x17
#define I2C_BAUDRATE 1000000 // 1 MHz
#define I2C_SLAVE_SDA_PIN 0
#define I2C_SLAVE_SCL_PIN 1
#ifndef GPCOMMS_BUFFER_SIZE
#define GPCOMMS_BUFFER_SIZE 100
#endif
GamepadState gamepadState;
Mask_t gpioState = 0;

void processI2CData(GamepadState data)
{
    // printf("Processing I2CData with timestamp: %llu\n", data->timestamp);
    NESController nesController(2, 3, 4);

    // Translate the gamepad data into NES format
    uint16_t nesData = nesController.translateToFormat(data);

    // Send the data to the NES system
    nesController.sendToSystem(nesData);
}

void handleStatus(uint8_t *payload)
{
    printf("Received message payload value: %s\n", payload);
    (void)0;
}

void handleState(uint8_t *payload)
{
    static GPComms_State gpState;
    memcpy(&gpState, payload, sizeof(GPComms_State));
    gamepadState = gpState.gamepadState;
    gpioState = gpState.gpioState;
    processI2CData(gamepadState);
}

void handleMessage(uint8_t *payload)
{
    printf("Received message payload value: %s\n", payload);
    (void)0;
}

void handleBuffer(uint8_t *buf, int size)
{
    uint8_t command = buf[0];
    uint8_t *payload = &buf[1];

    switch (command)
    {
    case GPCMD_STATE:
        handleState(payload);
        break;

    case GPCMD_STATUS:
        handleStatus(payload);
        break;

    case GPCMD_MESSAGE:
        handleMessage(payload);
        break;

    case GPCMD_ACK:
        break;

    case GPCMD_UNKNOWN:
    default:
        break;
    }
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
        handleBuffer(buf, receivedIndex);
        receivedIndex = 0;
        break;
    }
    break;
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

int main()
{
    stdio_init_all();
    sleep_ms(3000);
    setup_slave();
    sleep_ms(2000);
    printf("I2C Retro PoC\n");
    printf("I2C rate set to %u KHz\n", I2C_BAUDRATE / 1000);
    printf("Starting I2C transfer\n");
    sleep_ms(1000);
}