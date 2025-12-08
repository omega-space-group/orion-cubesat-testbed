# Flight Software

This directory contains all on-board software for the ORION CubeSat testbed, organized by subsystem.

## 📁 Structure

```
flight-software/
├── cdh/        # Command & Data Handling
├── eps/        # Electrical Power System
├── payload/    # AI Payload
└── comms/      # Communications subsystem
```

## 🎯 Overview

The flight software implements core satellite functionality using a **hybrid architecture** that combines proven satellite protocols with modern AI frameworks:

- **Satellite Bus (C&DH, EPS)**: Traditional aerospace patterns with CubeSat Space Protocol (CSP)
- **AI Payload**: Modern SpaceROS framework for rich AI/robotics ecosystem  
- **Communication**: Dual-layer approach for different data types

This architecture, inspired by JAXA's RACS initiative, enables both flight-proven reliability and cutting-edge AI capabilities.

## 🏗️ Inter-Subsystem Communication

### CSP over CAN Bus (Primary Protocol)

**Purpose**: Control messaging, commands, telemetry, status updates

**Used Between:**
- C&DH ↔ EPS (power control, battery monitoring)
- C&DH ↔ Payload (application lifecycle, status updates)
- C&DH ↔ Comms (data for ground downlink)

**Characteristics:**
- Flight-proven protocol with CubeSat heritage
- Reliable packet delivery
- Addressing and routing
- ~40-60 kbps effective throughput on 250 kbps CAN
- Suitable for commands and small telemetry packets

### Zenoh over Gigabit Ethernet (High-Bandwidth Data)

**Purpose**: Large data transfer (images, AI results, data products)

**Used Between:**
- Payload → C&DH (camera images, processed data for ground downlink)

**Characteristics:**
- High bandwidth (up to 1 Gbps)
- Publish/subscribe pattern (Zenoh)
- C&DH uses Zenoh subscriber (using Zenoh client)
- Payload publishes from ROS2 nodes
- Uses Zenoh for high-bandwidth communication

### Payload-Internal Communication

**Protocol**: ROS2 with Zenoh middleware

**Used Within Payload Only:**
- Camera nodes → AI application nodes
- AI application nodes → Application Manager
- Resource Monitor → Application Manager


## 🏗️ Architecture Principles

1. **Modularity**: Each subsystem is independently developed and testable
2. **Protocol Separation**: Use appropriate protocol for each data type
3. **Flight Heritage**: Adopt proven patterns from successful missions
4. **Modern Frameworks**: Leverage ROS2 for AI/robotics where appropriate
5. **Fault Tolerance**: Graceful degradation and error handling
6. **Real-Time Performance**: Critical paths meet timing requirements
7. **Testability**: Comprehensive unit and integration tests

## 📊 Subsystem Communication Matrix

| From/To | C&DH | EPS | Payload | Comms |
|---------|------|-----|---------|-------|
| **C&DH** | - | CSP/CAN | CSP/CAN + Zenoh/GigE | CSP/CAN |
| **EPS** | CSP/CAN | - | - | - |
| **Payload** | CSP/CAN + Zenoh/GigE | - | - | - |
| **Comms** | CSP/CAN | - | - | - |

## 🚀 Development Status

### Current Phase: Core Implementation

**C&DH:**
- [ ] cFS-inspired architecture (Python on RPi4)
- [ ] CSP over CAN interface
- [ ] Mode management, telecommand handling
- [ ] Telemetry collection and formatting
- [ ] Zenoh subscriber for payload data

**Payload:**
- [ ] SpaceROS setup on Jetson
- [ ] AI application framework
- [ ] CSP interface for C&DH communication
- [ ] Camera driver integration
- [ ] Cloud detection AI application
- [ ] Resource monitoring

**EPS:**
- [ ] Power distribution control
- [ ] Battery management
- [ ] CSP over CAN interface
- [ ] STM32 + FreeRTOS implementation

**Comms:**
- [ ] SDR interface (HackRF One)
- [ ] GNU Radio flowgraphs
- [ ] CCSDS protocol stack
- [ ] Ground station integration

### Integration Milestones:
- [ ] C&DH ↔ EPS via CSP/CAN
- [ ] C&DH ↔ Payload via CSP/CAN
- [ ] Payload → C&DH via Zenoh/GigE (images)
- [ ] End-to-end command/telemetry flow
- [ ] Full subsystem integration test

### Future Phase: Advanced Features
- [ ] STM32 + FreeRTOS migration for C&DH
- [ ] Fault Detection, Isolation, Recovery (FDIR)
- [ ] Power cycling capabilities
- [ ] Radiation mitigation strategies
- [ ] Full mission simulation

## 📚 Subsystem Documentation

Each subsystem has detailed documentation:
- [Command & Data Handling (C&DH)](cdh/README.md) - cFS-inspired architecture
- [Electrical Power System (EPS)](eps/README.md) - Power management
- [Payload](payload/README.md) - SpaceROS AI framework
- [Communications](comms/README.md) - SDR and ground link

## 🔧 Build Instructions

```bash
# Build all flight software
cd flight-software
./build_all.sh

# Build specific subsystem
cd cdh && ./build.sh
cd payload && colcon build
cd eps && ./build.sh
```

Detailed build instructions are in each subsystem's README.

## 🧪 Testing

### Unit Tests
```bash
TBD
```

### Integration Tests
```bash
TBD
```

### Hardware-in-the-Loop Tests
```bash
TBD
```

## 🛠️ Development Tools

### CAN Bus Debugging
```bash
TBD
```

### ROS2 Debugging (Payload)
```bash
TBD
```

### CSP Debugging
```bash
TBD
```

## ⚙️ Configuration

### Global Configuration
- `config/network.yaml` - Network addressing and routing
- `config/timing.yaml` - Timing constraints and schedules

### Subsystem Configuration
- `cdh/config/` - C&DH-specific configs
- `payload/config/` - Payload application registry
- `eps/config/` - Power management parameters
- `comms/config/` - RF and protocol settings

## 📊 Performance Metrics

### Communication Performance
- **CSP/CAN Latency**: < 50ms typical
- **CSP/CAN Throughput**: ~40-60 kbps effective
- **Zenoh/GigE Latency**: < 10ms typical  
- **Zenoh/GigE Throughput**: Up to 100 MB/s for images

### Subsystem Performance
- **C&DH CPU**: < 50% nominal load
- **Payload GPU**: < 80% during AI inference
- **EPS Response**: < 100ms for power commands
- **AI Inference**: < 100ms per frame (cloud detection)

## 🤝 Contributing

When contributing to flight software:
- Follow subsystem architectural patterns
- Test with both simulation and hardware
- Document all external interfaces
- Add comprehensive tests for new features
- Follow safety-critical coding standards where applicable
- Ensure real-time constraints are met
- Use appropriate communication protocol (CSP vs Zenoh)

## 📖 Related Documentation

- [System Architecture](../docs/architecture/system-overview.md)
- [CSP Protocol Specification](../docs/architecture/csp-protocol.md)
- [Zenoh Interface Specification](../docs/architecture/zenoh-interface.md)
- [Hardware Configuration](../hardware/)
- [Testing Strategy](../docs/testing-strategy.md)

## 📚 Key References

1. **NASA cFS**: Core Flight System architectural patterns - https://cfs.gsfc.nasa.gov/
2. **JAXA RACS**: Hybrid ROS/cFS approach - H. Kato et al., IEEE Aerospace 2021
3. **SpaceROS**: ROS 2 for space applications - https://space.ros.org/
4. **libcsp**: CubeSat Space Protocol library - https://github.com/libcsp/libcsp
5. **Eclipse Zenoh**: Modern pub/sub middleware - https://zenoh.io/
