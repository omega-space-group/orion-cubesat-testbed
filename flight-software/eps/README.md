# Electrical Power System (EPS)

The EPS subsystem manages all power generation, distribution, and monitoring for the satellite.

## 🎯 Responsibilities

- **Power Monitoring**: Track voltage, current, and power consumption for all subsystems
- **Battery Management**: Monitor battery state of charge, health, and temperature
- **Power Distribution**: Control power switches to subsystems
- **Solar Panel Management**: Monitor solar panel performance
- **Power Budget Management**: Ensure power availability for mission operations
- **Protection**: Overcurrent, overvoltage, and thermal protection

## 🖥️ Hardware Platform

- **MCU**: STM32 Nucleo
- **RTOS**: FreeRTOS
- **Interfaces**:
  - CAN: For telemetry and commands
  - ADC: For voltage/current sensing
  - GPIO: For power switch control


## 🚀 Development Status

**Coming Soon**
