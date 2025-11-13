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

The flight software implements the core functionality of each satellite subsystem, with proper separation of concerns and well-defined interfaces for inter-subsystem communication.

### Inter-Subsystem Communication
- **CAN Bus**: Critical real-time commands and telemetry
- **Gigabit Ethernet**: High-bandwidth payload data transfer
- **Space ROS 2**: Middleware for coordination and message passing

## 🏗️ Architecture Principles

1. **Modularity**: Each subsystem is independently developed and testable
2. **Fault Tolerance**: Graceful degradation and error handling
3. **Real-Time Performance**: Critical paths meet timing requirements
4. **Testability**: Comprehensive unit and integration tests

## 🚀 Development Status

### Current Phase: Functional Validation
- [x] Basic subsystem interfaces defined
- [ ] CAN communication protocol implemented
- [ ] GigE communication protocol implemented
- [ ] Space ROS integration
- [ ] End-to-end testing

### Future Phase: Advanced Features
- [ ] Power cycling capabilities
- [ ] Fault injection testing
- [ ] Radiation mitigation strategies
- [ ] Full mission simulation

## 📚 Subsystem Documentation

Each subsystem has its own README with detailed information:
- [Command & Data Handling (C&DH)](cdh/README.md)
- [Electrical Power System (EPS)](eps/README.md)
- [Payload](payload/README.md)
- [Communications](comms/README.md)

## 🔧 Build Instructions

```bash
# Build all flight software
cd flight-software
./build.sh

# Build specific subsystem
cd cdh
./build.sh
```

Detailed build instructions are in each subsystem's README.

## 🧪 Testing

```bash
# Run all tests
./test.sh

# Run subsystem-specific tests
cd cdh && ./test.sh
```

## 🤝 Contributing

See the main [CONTRIBUTING.md](../CONTRIBUTING.md) guide. For flight software:
- Follow safety-critical coding standards
- Ensure real-time constraints are met
- Add comprehensive tests for all new features
- Document all external interfaces

## 📖 Related Documentation

- [System Architecture](../docs/architecture/)
- [Inter-Subsystem Interfaces](../middleware/interfaces/)
- [Hardware Configuration](../hardware/)
