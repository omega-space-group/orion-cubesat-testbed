# EPS - STM32 Implementation

FreeRTOS-based Electrical Power System implementation on STM32 Nucleo.

## Hardware Setup

- **Board**: STM32 NUCLEO-H755ZI-Q
- **RTOS**: FreeRTOS
- **Required Peripherals**:
  - CAN controller
  - Multiple ADC channels (voltage/current sensing)
  - GPIO (power switches, status LEDs)
  - I2C/SPI (sensor communication)
  - Timers (for PWM, watchdog)

## Software Architecture

### FreeRTOS Tasks
1. **Power Monitor Task** (High Priority): Continuous power monitoring
2. **Battery Management Task** (Medium): State estimation and protection
3. **CAN Communication Task** (Medium): Handle commands and send telemetry
4. **Safety Task** (Highest Priority): Fault detection and protection

### Inter-Task Communication
- Message queues for commands
- Semaphores for shared resource protection
- Event flags for fault conditions

## Development Tools

- **IDE**: STM32CubeIDE
- **Configuration**: STM32CubeMX
- **Debugger**: ST-Link
- **RTOS**: FreeRTOS (included with STM32Cube)

## Pin Configuration

*TBD*

## Quick Start

```bash
TBD
```

## Configuration

Edit `config/eps_config.h` for:
- ADC channel mappings
- Power rail definitions
- Protection thresholds
- Task priorities and stack sizes


## Development Status

- [ ] Define complete pin configuration
- [ ] Implement ADC drivers
- [ ] Setup FreeRTOS tasks
- [ ] Implement CAN interface
- [ ] Add protection logic
- [ ] Comprehensive testing
