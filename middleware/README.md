# Middleware

Communication middleware for the ORION CubeSat testbed.

## 📋 Scope

**Important**: This directory contains middleware components for **payload-internal communication only**. 

The testbed uses a hybrid communication architecture:
- **Satellite Bus (C&DH ↔ EPS ↔ Payload)**: CubeSat Space Protocol (CSP) over CAN bus
- **Payload Internal**: Space ROS (ROS2 with Zenoh middleware) for AI processing pipeline
- **High-Bandwidth Data**: Zenoh over Gigabit Ethernet for images/large data

This middleware directory focuses on the **payload-internal SpaceROS/ROS2 interfaces**.

## 🎯 Purpose

Provide standardized ROS2 interfaces for payload-internal communication:
- Camera drivers → AI applications
- AI applications → Application Manager
- Resource monitoring → Application Manager
- Internal data flow within the Jetson-based payload

## 📁 Components

```
middleware/
├── spaceros/           # SpaceROS-specific configurations
│   ├── setup/          # Installation scripts
│   └── config/         # Zenoh QoS profiles
└── interfaces/         # Custom ROS2 message/service definitions
    ├── msg/            # Custom message types
    ├── srv/            # Custom service types
    └── action/         # Custom action types
```

## 🏗️ Architecture Context

```
┌─────────────────────────────────────────────────────┐
│              Payload Subsystem                      │
│                                                     │
│  ┌──────────┐  ROS2 with Zenoh ┌──────────────┐     │
│  │  Camera  │◄───────────────►│   AI Apps    │     │
│  │  Nodes   │                  │ (ROS2 Nodes) │     │
│  └──────────┘                  └──────┬───────┘     │
│                                       │             │
│                                 ROS2 with Zenoh     │             
│                                       │             │
│                                       │             │
│                              ┌─────────────────┐    │
│                              │   Application   │    │
│                              │    Manager      │    │
│                              │   (ROS2 Node)   │    │
│                              └────────┬────────┘    │
│                                       │             │
│                                       │             │
└───────────────────────────────────────┼─────────────┘
                                        │
                                     CSP/CAN
                                        │
                                        ▼
                                   ┌──────────┐
                                   │          │
                                   │   C&DH   │
                                   │          │
                                   └──────────┘
```


## 📦 Custom Message Types

Define payload-specific ROS2 interfaces in `interfaces/`:

### Example Message Definitions

**msg/CloudResult.msg**
```
std_msgs/Header header
float64 cloud_coverage_percent
float64 confidence
geometry_msgs/Point[] cloud_regions
```

**msg/AIStatus.msg**
```
std_msgs/Header header
string application_name
string state  # running/stopped/error
float64 inference_time_ms
float64 cpu_usage_percent
float64 gpu_usage_percent
```

**srv/StartApplication.srv**
```
# Request
string application_name
---
# Response
bool success
string message
```

## 🔧 Setup

```
TBD
```

## 🚀 Development Status

### Phase 1 - Core Interfaces
- [ ] Install SpaceROS on Jetson
- [ ] Define custom message types
- [ ] Define service interfaces
- [ ] Configure Zenoh QoS profiles
- [ ] Test inter-node communication

### Phase 2 - Advanced Features
- [ ] Action interfaces for long-running AI tasks
- [ ] Parameter management for AI apps
- [ ] Dynamic discovery of AI nodes
- [ ] Performance monitoring interfaces

## 🧪 Testing

```bash
TBD
```

## 🤝 Contributing

When contributing to middleware:
- Follow ROS2 message design guidelines
- Use standard ROS2 message types when possible
- Document all custom interfaces
- Test with actual hardware
- Consider bandwidth and latency
- Use appropriate QoS settings

## 📖 Related Documentation

- [ROS 2 Interface Design](https://docs.ros.org/en/humble/Concepts/About-ROS-Interfaces.html)
- [ROS2 QoS Policies](https://docs.ros.org/en/humble/Concepts/About-Quality-of-Service-Settings.html)
- [SpaceROS Documentation](https://space.ros.org/)
- [Payload Architecture](../docs/architecture/payload.md)
- [CSP Protocol (Inter-subsystem)](../docs/architecture/csp-protocol.md)

## ⚠️ Important Notes

1. **CSP over CAN is the primary protocol** for satellite bus communication
3. **High-bandwidth data** (images) can also use Zenoh over GigE to C&DH. C&DH uses Zenoh subscriber

## 📚 References

1. ROS 2 Documentation: https://docs.ros.org/en/humble/
2. SpaceROS: https://space.ros.org/
3. Eclipse Zenoh Documentation: https://zenoh.io
4. Eclipse Zenoh RMW: https://github.com/ros2/rmw_zenoh
