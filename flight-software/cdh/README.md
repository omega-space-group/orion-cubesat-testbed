# Command & Data Handling (C&DH)

The C&DH subsystem is responsible for commanding all satellite subsystems, collecting and processing telemetry, managing the mission timeline, and handling communications with the ground station.

## 🎯 Responsibilities

- **Command Distribution**: Parse and route commands to appropriate subsystems
- **Telemetry Collection**: Gather, format, and store telemetry data
- **Housekeeping**: Monitor system health and perform routine tasks
- **Time Management**: Maintain onboard time and schedule operations
- **Data Storage**: Manage filesystem and data buffering
- **Communication Interface**: Bridge between ground station and satellite systems

## 🖥️ Hardware Platforms

### Raspberry Pi 4 (Prototype)
- **Purpose**: Initial development and functional validation
- **OS**: Ubuntu 22.04
- **Benefits**: Rapid prototyping, full Linux environment, ease of development

### STM32 Nucleo (Production)
- **Purpose**: Flight-ready system with RTOS
- **OS**: FreeRTOS
- **Benefits**: Real-time performance, lower power consumption, flight heritage

## 📁 Directory Structure

```
cdh/
├── rpi4/           # Raspberry Pi 4 implementation
│   ├── src/        # Source code
│   ├── include/    # Header files
│   ├── config/     # Configuration files
│   └── tests/      # Unit tests
└── stm32/          # STM32 implementation
    ├── src/
    ├── include/
    ├── config/
    └── tests/
```

## 🔌 Interfaces

### Input Interfaces
- **Ground Commands**: Via comms subsystem (RF link)
- **Subsystem Telemetry**: Via CAN bus
- **Payload Data**: Via Gigabit Ethernet

### Output Interfaces
- **Subsystem Commands**: Via CAN bus
- **Telemetry Downlink**: Via comms subsystem
- **Payload Control**: Via GigE

## 🏗️ Software Architecture

### Key Components
1. **Command Parser**: Validates and routes incoming commands
2. **Telemetry Manager**: Collects and formats telemetry data
3. **Scheduler**: Manages timed operations and routines
4. **Data Handler**: Manages onboard storage and data queues
5. **Health Monitor**: Tracks subsystem status and anomalies

### Communication Flow
```
Ground Station → Comms → C&DH → [Command Router] → Subsystems
Subsystems → [Telemetry Collector] → C&DH → Comms → Ground Station
```

## 🚀 Development Status

### Raspberry Pi 4 Implementation
- [X] Basic system initialization
- [ ] CAN bus communication
- [ ] GigE communication
- [ ] Command parser
- [ ] Telemetry collection
- [ ] Space ROS integration
- [ ] File system management

### STM32 Implementation
- [ ] FreeRTOS port
- [ ] Hardware abstraction layer
- [ ] Real-time scheduler
- [ ] Flash memory management
- [ ] Watchdog implementation

## 🔧 Build Instructions

*To be defined*

## 🧪 Testing

```bash
*To be defined*
```

## ⚙️ Configuration

Configuration files are located in `config/`:
- `system_config.yaml`: System parameters
- `command_table.yaml`: Valid command definitions
- `telemetry_format.yaml`: Telemetry packet formats

## 📊 Performance Requirements

*To be refined*
- **Command Latency**: < 100ms from receipt to execution
- **Telemetry Rate**: 1 Hz nominal, 10 Hz during critical operations
- **CPU Usage**: < 50% under nominal load
- **Memory Usage**: *To be defined*

## 🤝 Contributing

When contributing to C&DH:
- Ensure commands are validated before routing
- Add telemetry points for new features
- Test all command paths thoroughly
- Document command and telemetry formats

## 📖 Related Documentation

- [System Architecture](../../docs/architecture/)
- [Command Specifications](../../docs/api/commands.md)
- [Telemetry Specifications](../../docs/api/telemetry.md)
- [CAN Bus Protocol](../../hardware/bus-configs/can/)
