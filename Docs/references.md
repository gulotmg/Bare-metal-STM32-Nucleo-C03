# References & Documentation

External resources consulted during the development of this project.
All documents are referenced by link only; no third-party PDF is
redistributed in this repository.

## MCU: STM32 Nucleo-C031C6

- [STM32C031C6 datasheet (mirror)](https://www.alldatasheet.com/datasheet-pdf/pdf/1570836/STMICROELECTRONICS/STM32C031C6.html)
  Device datasheet: electrical characteristics, pinout and ADC dynamic
  performance.

- [RM0490: STM32C0 series Reference Manual](https://www.st.com/resource/en/reference_manual/rm0490-stm32c0-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
  Register-level description of every peripheral used by the firmware:
  RCC, GPIO, EXTI, ADC, USART, I2C, SPI, RTC and IWDG.

- [MB1717-C031C6-B02 schematic pack](https://www.st.com/resource/en/schematic_pack/mb1717-c031c6-b02_schematic.pdf)
  Official Nucleo-C031C6 board schematics; used to verify the pin
  assignments, the VCP solder-bridge configuration (SB27/SB32 ON,
  USART2 on PA2/PA3) and the default jumper settings.

## Sensor: InvenSense MPU-6500

- [MPU-6500 Product Specification (datasheet)](https://www.mouser.com/datasheet/2/400/MPU-6500-Datasheet2-1112506.pdf)
  Official datasheet of the 6-axis MotionTracking device: register map,
  accelerometer full-scale ranges and sensitivity (8192 LSB/g at ±4 g)
  and DLPF settings used by the I²C and SPI examples.

- [GitHub: libdriver/mpu6500](https://github.com/libdriver/mpu6500)
  Full-featured open-source C driver for the MPU-6500 (MCU and Linux
  platforms); inside, you can find the documentation needed in /datasheet.

## ARM core

- [ARM Cortex-M0+ Devices Generic User Guide (DUI0662B)](https://support.arm.com/documentation/dui0662/b/)
  Core-level documentation for the Cortex-M0+: programmers model,
  exception/NVIC behavior, power management (WFI) and instruction set,
  used for the bare-metal firmware development.
