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

#define NOW() to_us_since_boot(get_absolute_time())
#define LOG(start) printf("%llu\n", NOW() - start);
#define LOGD(size, diff) printf("%i, %llu\n", diff, size)
#define LOGD_H() printf("size, micros\n");

#define I2C_SLAVE_ADDR 0x17
#define I2C_BAUDRATE 1000000 // 1 MHz
#define I2C_SLAVE_SDA_PIN 20
#define I2C_SLAVE_SCL_PIN 21

void processI2CData(I2CData *data)
{
    printf("Processing I2CData with timestamp: %llu\n", data->timestamp);
    bool isPin6High = gpio_get(6);
    bool isPin7High = gpio_get(7);

    if (isPin6High && isPin7High)
    {
        // Both pin 6 and pin 7 are high
        // Add your code here for this case
    }
    else if (isPin6High)
    {
        // Only pin 6 is high
        // Add your code here for this case
    }
    else if (isPin7High)
    {
        SNESController snesController(2, 1, 3);
        uint8_t snesData = snesController.translateToFormat(data);
        snesController.sendToSystem(snesData);
    }
    else
    {
        // Both pins are low, enable NES mode
        NESController nesController(2, 1, 3); // pin0 as latch, pin1 as clock, pin2 as data

        // Translate the gamepad data into NES format
        uint8_t nesData = nesController.translateToFormat(data);

        // Send the data to the NES system
        nesController.sendToSystem(nesData);
    }
}
static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event)
{
    static uint8_t buf[256] = {0};
    static int32_t receivedIndex = 0;

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
        I2CData *data = reinterpret_cast<I2CData *>(buf);
        uint64_t diff = NOW() - data->timestamp;
        LOGD(diff, receivedIndex);
        receivedIndex = 0;
        memcpy(buf, 0, 256);
        processI2CData(data);
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
    printf("I2C Master/Slave example\n");
    printf("I2C rate set to %u KHz\n", I2C_BAUDRATE / 1000);
    printf("Starting I2C transfer\n");
    sleep_ms(1000);
}