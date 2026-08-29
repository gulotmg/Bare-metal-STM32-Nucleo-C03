# Bare-metal STM32 Nucleo-C03

Register-level embedded C examples for the STM32 Nucleo-C031C6 board: small, independent experiments useful as a starting point for more complex firmware.

## Target

- **Board:** STMicroelectronics NUCLEO-C031C6
- **MCU:** STM32C031C6Tx, Arm Cortex-M0+
- **Peripheral access:** CMSIS device headers and direct register manipulation

## Repository layout

```text
Drivers/
  ADC/
  DMA/
  EXTI/
  I2C_mpu_6500/
  IWDG/
  PWM/
  RTC/
  SPI_mpu_6500/
  UART/
```

## Examples

| Directory | Demonstrates | Main observable behaviour |
|---|---|---|
| `Drivers/UART` | USART2 transmit driver | Continuously prints in serial terminal |
| `Drivers/ADC` | ADC1 single-channel setup on PA1 | Continuously samples channel 1 and prints the result over UART |
| `Drivers/PWM` | TIM1 PWM generation on PA8 | Configures TIM1 Channel 1 to output a PWM signal (ARR = 12000, 50% default duty) |
| `Drivers/DMA` | DMAMUX-routed DMA circular transfer | Autonomously copies ADC1 conversions into TIM1 CCR1, closing a zero-CPU control loop |
| `Drivers/EXTI` | GPIO and external interrupt | The rising edge of the PC13 user button triggers `EXTI4_15_IRQHandler`; PA5 drives the LED |
| `Drivers/IWDG` | Independent watchdog and EXTI-driven state machine | The button toggles between feeding the watchdog and starving it to force a reset |
| `Drivers/RTC` | LSE-backed RTC calendar | Initialises a fixed date and time, then continuously prints the decoded time, date and weekday |
| `Drivers/I2C_mpu_6500` | I²C1 master and MPU-6500 | Reads the device ID and the accelerometer registers at address `0x68`, then prints X/Y/Z in g |
| `Drivers/SPI_mpu_6500` | SPI1 master and MPU-6500 | Configures the SPI mode, reads the accelerometer registers and prints X/Y/Z in g |

## PWM and ADC pinout (Potentiometer setup)

The `PWM` and `DMA` examples can be combined to build a fully autonomous control loop: the ADC continuously samples an analog voltage, and the DMA peripheral writes each converted value straight into the TIM1 capture/compare register, adjusting the PWM duty cycle in real time with **zero CPU intervention**.

### Hardware wiring

Connect a standard 10 kΩ (or similar) potentiometer to the Nucleo Arduino headers:

| Potentiometer terminal | Nucleo pin | Header | Notes |
|---|---|---|---|
| **VCC** | 3.3 V | CN6 pin 4 or CN5 pin 8 | Do **not** use 5 V — PA1 is not 5 V-tolerant |
| **GND** | GND | CN6 pin 6 | Common ground with the board |
| **Wiper** | **PA1** (A1) | CN8 pin 2 | ADC1_IN1 analog input |

The resulting PWM waveform is available on **PA8** (CN10 pin 1). Probe it with an oscilloscope or connect an LED (with a 220–330 Ω series resistor) to visually verify the duty cycle change as you turn the knob.

### Pin summary

| Peripheral | Signal | Pin | Configuration in code |
|---|---|---|---|
| ADC1 | IN1 (Analog Input) | **PA1** | Analog mode, 12.5-cycle sampling, continuous + DMA circular |
| TIM1 | CH1 (PWM Output) | **PA8** | Alternate Function 2 (AF2), push-pull, PWM Mode 1 |
| DMA1 | Channel 1 | *Internal* | Peripheral-to-peripheral, `ADC1->DR` → `TIM1->CCR1`, circular, via DMAMUX request 5 |

## I²C and SPI pinout

Both MPU-6500 examples configure the accelerometer for ±4 g (8192 LSB/g) with a 41 Hz digital low-pass filter. I²C1 is mapped to PA9 and PA10, while SPI1 uses PA5, PA6 and PA7 with PA4 as a manually controlled chip select.

| Interface | Signal | Pin | Configuration in code |
|---|---|---|---|
| I²C1 | SCL | **PA9** | Alternate function 6, open-drain, pull-up |
| I²C1 | SDA | **PA10** | Alternate function 6, open-drain, pull-up |
| SPI1 | SCK | **PA5** | Alternate function 0 |
| SPI1 | MISO | **PA6** | Alternate function 0 |
| SPI1 | MOSI | **PA7** | Alternate function 0 |
| SPI1 | CS/NSS | **PA4** | GPIO output, manual active-low chip select |

## Logic analyzer captures

SPI1 and I²C1 traffic while talking to the MPU-6500, captured with a logic analyzer.

**SPI1 – initialization**
<img src="https://github.com/user-attachments/assets/7fdc65d5-7fd0-4443-9f97-b524d37d85ee" alt="SPI1 initialization" width="900">
- *Configuration phase: register writes that initialise the MPU-6500*

**SPI1 – steady state**
<img src="https://github.com/user-attachments/assets/1e38813f-1c0a-453e-95e7-22a6c8aa29a3" alt="SPI1 steady state" width="900">
- *Runtime: the sensor continuously streaming the acquired X/Y/Z samples.*

---

**I²C1 – initialization**
<img src="https://github.com/user-attachments/assets/01f68828-3d89-4b8b-8401-cd6bfeb68aea" alt="I2C1 steady state" width="900">
- *Configuration phase: WHO_AM_I and setup writes to the MPU-6500.*

**I²C1 – steady state**
<img src="https://github.com/user-attachments/assets/9b3010cc-b26c-448b-b68f-0fadb5425f91" alt="I2C1 initialization" width="900">
- *Runtime: repeated accelerometer reads.*

## Flashing

Use the on-board ST-LINK or any compatible programmer to flash the generated `.elf`, `.hex` or `.bin` image.

## Notes

- Peripheral initialisation is explicit and register-oriented: no STM32 HAL or LL layers.
- Timing relies on busy-wait loops and fixed clock assumptions; the examples are not optimised for production use — they provide a good starting point.
- The code is hardware-specific: on different boards, check alternate functions, pull-ups, oscillators, clock values and equivalent (or not) registers.

## License

Released under the [MIT License](LICENSE). The bundled ST CMSIS headers retain their own STMicroelectronics copyright and license.
