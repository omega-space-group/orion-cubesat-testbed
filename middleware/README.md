# Middleware

Communication middleware for the ORION CubeSat testbed.

## 📋 Scope

**Important**: This directory contains middleware components for **payload-internal communication only**. 

The testbed uses a hybrid communication architecture:
- **Satellite Bus (C&DH ↔ EPS ↔ Payload)**: CubeSat Space Protocol (CSP) over CAN bus
- **Payload Internal**: Space ROS (ROS2 DDS) for AI processing pipeline
- **High-Bandwidth Data**: DDS over Gigabit Ethernet for images/large data

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
│   └── config/         # DDS QoS profiles
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
│  ┌──────────┐    ROS2 DDS     ┌──────────────┐    │
│  │  Camera  │◄──────────────► │  AI Apps     │    │
│  │  Nodes   │                 │  (ROS2 Nodes)│    │
│  └──────────┘                 └──────┬───────┘    │
│                                       │             │
│                          ROS2 DDS     │             │
│                                       ▼             │
│                            ┌──────────────────┐    │
│                            │   Application    │    │
│                            │    Manager       │    │
│                            │   (ROS2 Node)    │    │
│                            └────────┬─────────┘    │
│                                     │               │
│                                     │               │
└─────────────────────────────────────┼───────────────┘
                                      │
                          CSP/CAN (not ROS2!)
                                      │
                                      ▼
                               ┌──────────┐
                               │   C&DH   │
                               │ (No ROS2)│
                               └──────────┘
```

**Key Point**: Inter-subsystem communication (C&DH ↔ Payload, C&DH ↔ EPS) uses CSP over CAN, 
NOT ROS2/DDS. This middleware is only for payload-internal ROS2 communication.

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

### Install SpaceROS

```bash
# Follow official SpaceROS installation
# https://space.ros.org/

# Or use Docker
docker pull osrf/space-ros:latest
```

### Build Custom Interfaces

```bash
cd middleware/interfaces
colcon build
source install/setup.bash
```

### Configure DDS QoS

Edit `spaceros/config/qos_profiles.yaml`:

```yaml
camera_qos:
  history: keep_last
  depth: 10
  reliability: reliable
  durability: volatile

ai_results_qos:
  history: keep_last
  depth: 5
  reliability: reliable
  durability: transient_local
```

## 🚀 Development Status

### Phase 1 - Core Interfaces
- [ ] Install SpaceROS on Jetson
- [ ] Define custom message types
- [ ] Define service interfaces
- [ ] Configure DDS QoS profiles
- [ ] Test inter-node communication

### Phase 2 - Advanced Features
- [ ] Action interfaces for long-running AI tasks
- [ ] Parameter management for AI apps
- [ ] Dynamic discovery of AI nodes
- [ ] Performance monitoring interfaces

## 🧪 Testing

```bash
# Test message generation
ros2 interface show middleware_interfaces/msg/CloudResult

# Test publisher/subscriber
ros2 topic pub /test/cloud_result middleware_interfaces/msg/CloudResult "..."
ros2 topic echo /test/cloud_result

# Test service
ros2 service call /test/start_app middleware_interfaces/srv/StartApplication "..."
```

## 📚 ROS2 Communication Patterns Used

### Topics (Publish/Subscribe)
- Camera images: `sensor_msgs/Image`
- AI results: Custom messages
- Status updates: Custom messages
- Resource monitoring: Custom messages

### Services (Request/Response)
- Application lifecycle: Custom services
- Configuration: ROS2 parameters
- Status queries: Custom services

### Actions (Long-running goals)
- AI inference tasks (Phase 2)
- Data processing pipelines (Phase 2)

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
- [DDS QoS Policies](https://docs.ros.org/en/humble/Concepts/About-Quality-of-Service-Settings.html)
- [SpaceROS Documentation](https://space.ros.org/)
- [Payload Architecture](../docs/architecture/payload.md)
- [CSP Protocol (Inter-subsystem)](../docs/architecture/csp-protocol.md)

## ⚠️ Important Notes

1. **This middleware is payload-internal only** - it does NOT handle communication with C&DH or other subsystems
2. **CSP over CAN is the primary protocol** for satellite bus communication
3. **ROS2 DDS** is only used within the payload subsystem
4. **High-bandwidth data** (images) can also use DDS over GigE to C&DH, but C&DH uses minimal DDS subscriber (not full ROS2)

## 📚 References

1. ROS 2 Documentation: https://docs.ros.org/en/humble/
2. SpaceROS: https://space.ros.org/
3. DDS Specification: https://www.omg.org/spec/DDS/
4. JAXA RACS Extended DDS: https://github.com/jaxa/racs2_extended-dds
