@mainpage OBC Software v0

@image html manual.png "" width=800px

### Flight Software Overview

This project aims to develop a lightweight, message-based flight software firmware built on FreeRTOS specifically 
for the Open-Source CubeSat FlatSat Testbed. By leveraging a publish-subscribe (Pub/Sub) messaging architecture, 
the system will ensure decoupled, modular communication between software components (e.g., telemetry, power 
management, and ADCS). Furthermore it aims to comply with the mission requirements of the Orion CubeSat (more info in Cubesat Testbed Project Overview.pdf that can be found in the project folder). 

### System Requirements

| Category | Parameter | Specification |
| :--- | :--- | :--- |
| **Toolchain** | IDE Version | STM32CubeIDE v1.13.0 (Caution with migration)|
| | Compiler & Settings | arm-none-eabi-gcc (-O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb) |
| **Operating System**| FreeRTOS Version | v10.3.1 (CMSIS-RTOS V2 wrapper) or later |
| **Hardware Target** | MCU Model | STM32H755ZI (ARM Cortex-M7 @ **64 MHz** & ARM Cortex-M4 @ **64 MHz**) |
| **Clock and Timers** |    HSI | 64 MHz |
| | Systick | **TIM6** (Dedicated Peripheral for HAL Timebase - Assigned Highest Priority) & **Systick** (Dedicated Exclusively to RTOS - Assigned Lowest Priority)|
| **Critical Drivers** | STM32Cube FW Library | Patch v1.11.12 or later |
| **Peripherals** | Communication Buses | FDCAN1 (Payload) |

### Current Setup
| Hardware | Part Number | 
| :--- | :--- |
| **STM32 Nucleo Board** | nucleo-h755zi-q | 
| **FDCAN Transceiver Hat** | Waveshare RS485 CAN Shield | 

<div style="display: none;">
@subpage manuals
</div>