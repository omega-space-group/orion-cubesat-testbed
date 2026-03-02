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
- **AI Payload**: Modern framework for rich AI/robotics ecosystem  
- **Communication**: Dual-layer approach for different data types

## 🏗️ Inter-Subsystem Communication

### CSP over CAN Bus (Primary Protocol)

**Purpose**: Control messaging, commands, telemetry, status updates

**Used Between:**
- C&DH ↔ EPS (power control, battery monitoring)
- C&DH ↔ Payload (application lifecycle, status updates)
- C&DH ↔ Comms (data for ground downlink)


### Zenoh over Gigabit Ethernet (High-Bandwidth Data)

**Purpose**: Large data transfer (images, AI results, data products)

**Used Between:**
- Payload → COMMS (camera images, processed data for ground downlink) (direct link)


### Payload-Internal Communication

**Protocol**: Zenoh

**Used Within Payload Only:**
- Camera nodes → AI application nodes
- AI application nodes → Application Manager
- Resource Monitor → Application Manager


## 🏗️ Architecture Principles

1. **Modularity**: Each subsystem is independently developed and testable
2. **Protocol Separation**: Use appropriate protocol for each data type
3. **Flight Heritage**: Adopt proven patterns from successful missions
4. **Modern Frameworks**: Leverage Zenoh for AI where appropriate
5. **Fault Tolerance**: Graceful degradation and error handling
6. **Real-Time Performance**: Critical paths meet timing requirements
7. **Testability**: Comprehensive unit and integration tests

## 📊 Subsystem Communication Matrix

| From/To | C&DH | EPS | Payload | Comms |
|---------|------|-----|---------|-------|
| **C&DH** | - | CSP/CAN | CSP/CAN | CSP/CAN |
| **EPS** | CSP/CAN | - | - | - |
| **Payload** | CSP/CAN | - | - | Zenoh/GigE |
| **Comms** | CSP/CAN | - | Zenoh/GigE | - |


## 📖 Related Documentation

- [System Architecture](../docs/architecture/README.md)

## 📚 Key References

1. **NASA cFS**: Core Flight System architectural patterns - https://cfs.gsfc.nasa.gov/
4. **libcsp**: CubeSat Space Protocol library - https://github.com/libcsp/libcsp
5. **Eclipse Zenoh**: Modern pub/sub middleware - https://zenoh.io/
