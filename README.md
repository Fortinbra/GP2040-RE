# GP2040-RE

GP2040-Retro Edition

## What this is

The purpose here is to add the ability to use your GP2040-CE controller with older consoles that do not have USB support. So this project is not a contoller on it's own, and requires the use of another gamepad for the actual controller functions. This is just the transport level to connect to the consoles themselves.

## Getting Started

### GP2040-CE

The features required on the GP2040-CE side of this have not been released yet, and this will be updated with the version number as soon as they are.

### Hardware

This project operates as a target I2C device. (Formerly called a slave device). At minimum a Raspberry Pi Pico is required. Pico W will not be supported, as wireless is not on the roadmap here, and we will be utilizing all the RP2040 pins eventually, not leaving any for the SPI bus required for wireless functions.
A level shifter is used to bring the 3.3V logic of the Pico up to the 5V logic of the SNES (the only suppored retro console so far). A diode is necessary so that we can use the 5V line from the SNES as the reference voltage for the high side of the level shifter, and not accidentally backfeed anyting to the SNES. A pull up resistor is required on the Data line, as it's an open-drain, and should be normallly high. The SNES does have a 5V line that is documented to provide up to 25mA across both controller ports, and there are examples of devices utilizing that as a power source for similar projects to this. To be safe with the geriatric console, I'm opting for external power.

### Firmware

This document assumes you know how to build and flash your own firmware. All pin configurations are in [config.h](src/headers/config.h).

### Building your own

So this will be a breadboard edition.

#### Required Materials

- Raspberry Pi Pico
- 3.3v-> 5v level shifter
  - must support open-drain signals
  - must be bi-directional
- SNES controller cable
- Powersource for the Pico
  - can be USB or a battery bank
  - can be powered directly from GP2040-CE controller
- GP2040-CE controller with available I2C connection
  - does not work with the OLED

  Starting with the SNES controller cable, you'll need to identify which wires are attached to which pins. Official nintendo cables do follow a specific wiring color scheme but are increasingly hard to acquire.

SNES Controller Connector:

        ----------------------------- ---------------------
      |                             |                      \
      | (1)     (2)     (3)     (4) |   (5)     (6)     (7) |
      |                             |                      /
       ----------------------------- ---------------------

1 Vcc (+5V)
2 Clock
3 Latch
4 Data
5 Not Connected
6 Not Connected
7 Ground

#### Breadboarding at it's best

With the Pico attached to your breadboard, you'll want to add your level shifter, and wire it to the Pico with jumpers. If using the TXS0108 from Texas instruments, you would need GPIO pins 2-7 on the Pico to connect to pins A2-A7 on the level shifter. You'll want to connect the level shifter's ground to the Pico's ground (the entire project will have common ground, so make sure everything is connected). We also want to connect the level shifter's VCCA pin to the Pico's 3v3 pin. If your OE pin does not have a resisotor in it' breakout board, you'll want to use one to pull it to ground.

On the other side, let's connect the SNES controller to the level shifter. Pin 1 will go to a diode first, then the diode will connect to the VCCB pin on the level shifter.  Ground will connect to our common ground. Pin 4 (Data) will connect to B5 on the shifter. Pin 3 (Latch), will connect to B3, and Pin 2 (Clock), will connect to B1.
(Diagrams to come soon)

Next lets connect to the GP2040-CE controller. This is I2C, so we'll have four pins to deal with. The 3v3 pin from the I2C should connect to the VSYS pin on our Pico. The ground pin should be connected to our common ground setup. And finally, the SDA and SCL pins should be connected to GPIO 16 (SDA), and GPIO 17 (SCL) which is part of I2C0 on the Pico.

If you'd like to do UART monitoring, the button outputs can be seen using the UART on GPIO 0 and 1.

## The Future

- [ ] NES Support
  - Need a console to test against
  - Same protocol as SNES
- [ ] SNES Support
  - In progress
- [ ] N64
  - Uses JoyBus protocol
  - Memory card support
- [ ] GameCube
  - Uses JoyBus protocal
- [ ] Playstation 1 & 2
  - No Analog face button support at this time (PS2).
- [ ] Dreamcast
  - VMU suppport

## Would be nice

- Readily available connector with more than 8 pins, to support Sega, Atari, and NEOGEO consoles too.

## Deeper Dive - SNES

### Communication protocol

The communication protocol on the SNES is based on a 4021 PISO shift register, and depending the manufacture date of the controller, it may physically have two shift registers in a chain, or one 16bit register. Ether way, the output is the same.

When the Latch pin is high, it signals the shift register to start sending data according to the clock pulses. What is commonly documented incorrectly, is regarding the first data pulse. The data is sent as a low signal on the rising edge of the clock pin. However, when the latch is high at the beginning, the clock is also already high, so the first data pulse needs to begin immediatly. Overall, there will be 16 data pulses sent, the first twelve will be active low pulses representing the button state of the 12 buttons on the controller. The last 4 will be always high. On the trailing edge of the last clock pulse, the data will be pulled low one last time, before going back to it's default high state, and the cycle starts all over again. The entire report is done in 16.67ms. The button reports are always in the same order: B, Y, SELECT, START, UP, DOWN, LEFT, RIGHT, A, X, L, and R. The NES uses this same protocol, with a slightly different button order. A, B, SELECT, START, UP, DOWN, LEFT, and RIGHT
