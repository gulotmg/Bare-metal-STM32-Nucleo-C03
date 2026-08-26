# Bare-metal STM32 Nucleo-C03

Register-level embedded C examples for the STM32 Nucleo-C031C6 board: small, independent experiments useful as a starting point for more complex firmware.

## Target

- **Board:** STMicroelectronics NUCLEO-C031C6
- **MCU:** STM32C031C6Tx, Arm Cortex-M0+
- **Peripheral access:** CMSIS device headers and direct register manipulation

## Examples

| Directory | Demonstrates | Main observable behaviour |
|---|---|---|
| `Drivers/UART` | USART2 transmit driver | Continuously prints in serial terminal |
| `Drivers/ADC` | ADC1 single-channel setup on PA1 | Continuously samples channel 1 and prints the result over UART |
| `Drivers/EXTI` | GPIO and external interrupt | The rising edge of the PC13 user button triggers `EXTI4_15_IRQHandler`; PA5 drives the LED |
| `Drivers/IWDG` | Independent watchdog and EXTI-driven state machine | The button toggles between feeding the watchdog and starving it to force a reset |
| `Drivers/RTC` | LSE-backed RTC calendar | Initialises a fixed date and time, then continuously prints the decoded time, date and weekday |
| `Drivers/I2C_mpu_6500` | I²C1 master and MPU-6500 | Reads the device ID and the accelerometer registers at address `0x68`, then prints X/Y/Z in g |
| `Drivers/SPI_mpu_6500` | SPI1 master and MPU-6500 | Configures the SPI mode, reads the accelerometer registers and prints X/Y/Z in g |

## I²C and SPI pinout

Both MPU-6500 examples configure the accelerometer for ±4 g (8192 LSB/g) with a 41 Hz digital low-pass filter. I²C1 is mapped to PA9 and PA10, while SPI1 uses PA5, PA6 and PA7 with PA4 as a manually controlled chip select.

| Interface | Signal | Pin | Configuration in code |
|---|---|---:|---|
| I²C1 | SCL | **PA9** | Alternate function 6, open-drain, pull-up |
| I²C1 | SDA | **PA10** | Alternate function 6, open-drain, pull-up |
| SPI1 | SCK | **PA5** | Alternate function 0 |
| SPI1 | MISO | **PA6** | Alternate function 0 |
| SPI1 | MOSI | **PA7** | Alternate function 0 |
| SPI1 | CS/NSS | **PA4** | GPIO output, manual active-low chip select |

Logic analyser captures of the I²C and SPI transactions will be added to this section.

## Repository layout

```text
Drivers/
  ADC/
  EXTI/
  I2C_mpu_6500/
  IWDG/
  RTC/
  SPI_mpu_6500/
  UART/
```

## Flashing

Use the on-board ST-LINK or any compatible programmer to flash the generated `.elf`, `.hex` or `.bin` image.

## Notes

- Peripheral initialisation is explicit and register-oriented: no STM32 HAL or LL layers.
- Timing relies on busy-wait loops and fixed clock assumptions; the examples are not optimised for production use, as already said, they provide a good starting point.
- The code is hardware-specific: on different boards, check alternate functions, pull-ups, oscillators and clock values and equivalent (or not) registers.

## License

Released under the [MIT License](LICENSE). The bundled ST CMSIS headers retain their own STMicroelectronics copyright and license.
