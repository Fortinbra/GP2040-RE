#ifndef CONFIG_H
#define CONFIG_H

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

//Ethernet Connections
// Orange Strip is the GND connection.
#define ORANGE_ETHERNET_PIN 2
#define GREEN_STRIPE_ETHERNET_PIN 3
#define BLUE_ETHERNET_PIN 4
#define BLUE_STRIPE_ETHERNET_PIN 5
#define GREEN_ETHERNET_PIN 6
#define BROWN_STRIPE_ETHERNET_PIN 7
// Brown is the +V connection.

#define SPI_PORT 1
#define SPI_BAUDRATE 
#define SPI1_RX_MISO 8
// 9 is not used.
#define SPI1_SCK 10
#define SPI1_TX_MOSI 11
#define DATA_COMMAND 12
#define OLED_RESET 13
#define OCS 14
// 15 is not used.
// I2C config (incoming data)
#define I2C_SLAVE_ADDR 0xCE
#define I2C_BAUDRATE 100000
#define I2C_SLAVE_SDA_PIN 16
#define I2C_SLAVE_SCL_PIN 17
#ifndef GPCOMMS_BUFFER_SIZE
#define GPCOMMS_BUFFER_SIZE 100
#endif

// SDIO config
// D1-3 are offsets from D0, and need to be consecutive pins
#define CLK_GPIO 18
#define CMD_GPIO 19
#define D0_GPIO 20
#define D1_GPIO 21
#define D2_GPIO 22
#define D3_GPIO 23
#define CD_GPIO 24

// 25 is not used.

// I2C config (as a host)
#define I2C_BAUDRATE 100000
#define SDA_1_PIN 26
#define SCL_1_PIN 27
// ADC is not used
// 28 and 29 are not used.

#endif // CONFIG_H