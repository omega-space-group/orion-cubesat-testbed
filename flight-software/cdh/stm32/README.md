# C&DH - STM32 Implementation

Production-ready C&DH implementation on STM32 Nucleo with FreeRTOS.

## Purpose

Flight-ready implementation optimized for real-time performance, low power consumption, and reliability.

## Hardware Setup

- **Board**: STM32 NUCLEO-H755ZI-Q
- **RTOS**: FreeRTOS
- **Interfaces**:
  - CAN: Onboard CAN peripheral
  - Ethernet: STM32 Ethernet MAC (if available) or external module
  - Flash: External flash for data storage

## Development Status

⏳ **Future Implementation** - Will be developed after RPi4 prototype validation.

## Migration Plan

1. Validate all functionality on RPi4
2. Design hardware abstraction layer (HAL)
3. Port core algorithms to FreeRTOS
4. Optimize for memory and performance
5. Comprehensive testing on target hardware

## Requirements

- Real-time command processing (< 10ms response time)
- Low power consumption
- Flash memory management for data storage
- Watchdog timer implementation
- Fault detection and recovery

## Development Tools

- STM32CubeIDE
- STM32CubeMX for peripheral configuration
- OpenOCD for debugging
- FreeRTOS for RTOS

## Coming Soon

This directory will be populated once the RPi4 implementation reaches functional maturity.
