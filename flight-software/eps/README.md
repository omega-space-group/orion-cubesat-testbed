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

## 📁 Directory Structure

```
eps/
└── stm32/
    ├── src/           # Source code
    ├── include/       # Header files
    ├── config/        # Configuration
    ├── drivers/       # Hardware drivers
    └── tests/         # Unit tests
```

## 🔌 Interfaces

### CAN Bus Interface
- **Telemetry**: Power status, battery level, subsystem power consumption
- **Commands**: Enable/disable subsystem power, set power modes

### Hardware Interfaces
- **Input**: Solar panels, battery pack
- **Output**: Power rails to all subsystems
- **Sensing**: Voltage, current, and temperature sensors

## 🏗️ Software Architecture

### Key Components
1. **Power Monitor**: ADC sampling and data processing
2. **Battery Manager**: State estimation and protection
3. **Switch Controller**: Manages power distribution
4. **CAN Interface**: Communication with C&DH
5. **Safety Monitor**: Fault detection and protection

### State Machine
```
States:
- SAFE_MODE: Minimum power, only critical systems
- NOMINAL: Normal operations
- HIGH_POWER: Full payload operations
- CHARGING: Solar charging priority

Transitions based on:
- Battery level
- Commands from C&DH
- Fault conditions
```

## 🚀 Development Status

- [ ] Hardware interface drivers
- [ ] Power monitoring implementation
- [ ] Battery management algorithms
- [ ] CAN communication
- [ ] Safety and protection logic
- [ ] FreeRTOS task implementation
- [ ] Integration testing

## 🔧 Build Instructions

```bash
TBD
```

## 🧪 Testing

```bash
TBD
```

## ⚙️ Configuration

Power budget and thresholds configured in `config/power_config.h`:
- Battery voltage thresholds
- Current limits per subsystem
- Power mode definitions
- Safety margins

## 📊 Performance Requirements

- **Telemetry Rate**: 1 Hz for all power metrics
- **Command Response**: < 50ms
- **Fault Detection**: < 100ms from fault to protection action
- **Measurement Accuracy**: ±5% for voltage/current

## 🔒 Safety Features

- Overcurrent protection per rail
- Battery charge/discharge limits
- Thermal shutdown
- Watchdog timer
- Safe mode entry on critical faults

## 🤝 Contributing

When contributing to EPS:
- Safety-critical code requires thorough testing
- Document all protection thresholds
- Test fault scenarios extensively
- Consider power budget impact of changes

## 📖 Related Documentation

- [Power Budget Analysis](../../docs/architecture/power-budget.md)
- [CAN Protocol](../../hardware/bus-configs/can/)
- [Safety Requirements](../../docs/architecture/safety.md)
