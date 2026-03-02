# Command & Data Handling (C&DH)

The C&DH subsystem is the central flight computer responsible for commanding all satellite subsystems, collecting and processing telemetry, managing the mission timeline, and handling communications with the ground station.

## 🎯 Responsibilities

- **Command Distribution**: Parse and route commands to appropriate subsystems
- **Telemetry Collection**: Gather, format, and store telemetry data
- **Housekeeping**: Monitor system health and perform routine tasks
- **Time Management**: Maintain onboard time and schedule operations
- **Mode Management**: Control satellite operational states (Safe, Nominal, Payload modes)
- **Event System**: Unified logging and event management across subsystems
- **Communication Interface**: Bridge between ground station and satellite systems

## 🏗️ Architecture

This C&DH implementation follows a **hybrid approach**:

- **Software Architecture**: Custom implementation inspired by NASA cFS patterns
  - Modular application design (Mode Manager, Telecommand Handler, Telemetry Manager, etc.)
  - Software bus concept for inter-module communication
  - Event services
- **Communication Protocol**: CubeSat Space Protocol (CSP) over CAN bus
  - Primary interface for all subsystem communication (EPS, Payload)
  - Proven flight heritage in CubeSat missions


## 🖥️ Hardware Platforms

### Raspberry Pi 4 (Phase 1) (TBD)
- **Purpose**: Initial development and functional validation
- **OS**: Ubuntu 22.04
- **Software**: Python 3.10+ with cFS-inspired architecture
- **Benefits**: Rapid prototyping, full Linux environment, ease of development

### STM32 Nucleo (Phase 2)
- **Purpose**: Flight-ready system with RTOS
- **OS**: FreeRTOS
- **Software**: C/C++ port of Phase 1 Python implementation
- **Benefits**: Real-time performance, lower power consumption, flight heritage


## 🔌 Interfaces

### CSP over CAN (Primary Protocol)
**To EPS:**
- Commands: Power on/off subsystems, request status
- Telemetry: Voltage, current, battery state

**To Payload:**
- Commands: start, stop, reset, etc
- Telemetry: Application status, resource usage, AI result summaries

**From Ground (via Comms subsystem):**
- Telecommands for satellite operations
- Configuration updates

### Output to Ground
- Telemetry downlink via Comms subsystem

## 🏗️ Software Architecture


## 🔧 Build Instructions

```bash
TBD
```

## 🧪 Testing

```bash
TBD
```

## ⚙️ Configuration

```bash
TBD
```

## 📖 Related Documentation

- [cFS Architectural Patterns](../../docs/architecture/cfs-patterns.md)
- [CSP Protocol Specification](../../docs/architecture/csp-protocol.md)
- [Command Specifications](../../docs/api/commands.md)
- [Telemetry Specifications](../../docs/api/telemetry.md)
- [CAN Bus Configuration](../../hardware/bus-configs/can/)
- [Integration with Payload](../../docs/architecture/cdh-payload-interface.md)

## 📚 References

1. NASA core Flight System (cFS): https://cfs.gsfc.nasa.gov/
3. CubeSat Space Protocol: https://github.com/libcsp/libcsp
