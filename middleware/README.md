# Middleware

Inter-subsystem communication and coordination using Space ROS.

## Purpose

Provide standardized interfaces and message passing between all subsystems.

## Components

- **Space ROS 2**: Core middleware
- **Interface Definitions**: Message and service types
- **Communication Manager**: Route messages appropriately

## Architecture

```
┌─────┐    ┌─────┐    ┌─────────┐    ┌───────┐
│ EPS │←→│ C&DH│←→│ Payload │←→│ Comms │
└─────┘    └─────┘    └─────────┘    └───────┘
    ↕          ↕            ↕              ↕
    └──────────┴────────────┴──────────────┘
              Space ROS 2 Middleware
```

## Setup

```bash
# Install Space ROS
# Follow: https://space.ros.org/

# Build interfaces
TBD
```

## Message Types

Define custom messages in `interfaces/`:
- Command messages
- Telemetry messages
- Status messages
- Data transfer messages

## Development Status

- [ ] Install Space ROS 2
- [ ] Define message interfaces
- [ ] Implement communication nodes
- [ ] Test inter-subsystem comms
