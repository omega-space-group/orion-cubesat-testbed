# Payload

The payload subsystem is responsible for AI algorithm execution, on-board data processing, and high-bandwidth data handling using NVIDIA Jetson and Xilinx FPGA.

## 🎯 Objectives

- **AI Algorithm Validation**: Test and validate AI/ML algorithms in space-like environment
- **On-Board Processing**: Real-time data processing and analysis
- **Multi-Application Framework**: Deploy and manage multiple AI applications
- **End-to-End Pipeline Testing**: Complete data flow from camera acquisition to analysis
- **Performance Benchmarking**: Measure algorithm performance on edge hardware

## 🏗️ Architecture

This payload implementation uses a **hybrid communication architecture**:

- **Internal Processing**: Full SpaceROS (ROS2) for rich AI/robotics ecosystem
  - Camera drivers as ROS2 nodes
  - AI applications as ROS2 nodes
  - Application Manager as ROS2 node
  - Zenoh middleware (rmw_zenoh) for internal inter-node communication
  
- **External Interface to C&DH**: Dual-layer communication
  - **CSP over CAN**: Commands, telemetry, control (primary)
  - **Zenoh over GigE**: High-bandwidth data (images, large results)


## 🖥️ Hardware Platforms

### NVIDIA Jetson Xavier NX
- **Purpose**: AI inference, image processing, ML workloads
- **OS**: Ubuntu 22.04
- **Middleware**: SpaceROS (ROS2 Humble)
- **Capabilities**: 
  - CUDA acceleration
  - TensorRT optimization
  - Deep learning frameworks (PyTorch, TensorFlow)
  - 21 TOPS AI performance

### Xilinx FPGA (Future)
- **Purpose**: Hardware-accelerated processing, custom algorithms
- **Tools**: Vivado, Vitis
- **Capabilities**:
  - Custom image processing pipelines
  - Signal processing
  - Low-latency processing

## 📁 Directory Structure

```
payload/
├── jetson/                    # NVIDIA Jetson implementation
│   ├── src/                   # Source code
│   │   ├── application_manager.py    # ROS2 node
│   │   ├── csp_interface.py          # CSP bridge
│   │   ├── resource_monitor.py       # ROS2 node
│   │   └── ai_apps/
│   │       ├── cloud_detection/      # AI app (ROS2 node)
│   │       └── existing_app/         # Integrated existing app
│   ├── models/                # AI/ML models
│   │   └── cloud_detection/
│   ├── config/                # Configuration files
│   │   ├── application_registry.json
│   │   └── csp_config.yaml
│   ├── docker/                # Phase 2: Dockerfiles
│   ├── data/                  # Sample data for testing
│   └── tests/                 # Unit and integration tests
└── fpga/                      # Xilinx FPGA implementation
    ├── hdl/                   # Hardware description
    ├── ip/                    # Custom IP cores
    ├── constraints/           # Timing and pin constraints
    └── sim/                   # Simulations
```

## 🔌 Interfaces

### Input from C&DH

**CSP over CAN (Control):**
- Commands: `START_APP <app_name>`, `STOP_APP <app_name>`, `REQUEST_STATUS`, `LIST_APPS`
- Configuration updates
- Mode change commands

### Output to C&DH

**CSP over CAN (Telemetry):**
- Application status (running/stopped/error)
- Resource usage summaries
- AI result summaries (small data)
- Health and status updates

**Zenoh over GigE (High-Bandwidth Data):**
- Camera images (raw/processed)
- Detailed AI processing results
- Large data products for ground downlink

### Internal (SpaceROS/ROS2)

**Camera → AI Apps:**
- RGB camera: `/camera/rgb/image_raw` (sensor_msgs/Image)
- IR camera: `/camera/ir/image_raw` (when hardware available)

**AI Apps → Application Manager:**
- Results: `/ai/<app_name>/output`
- Status: `/ai/<app_name>/status`

## 🏗️ Software Architecture

### Phase 1: Native ROS2 Implementation

#### Core Components

1. **CSP Interface Module** (Python)
   - Bridges between CSP/CAN and ROS2 domain
   - Receives CSP packets from C&DH over CAN bus
   - Converts commands to function calls for Application Manager
   - Converts ROS2 data to CSP telemetry packets
   - Not a ROS2 node itself - uses libcsp

2. **Application Manager** (ROS2 Node)
   - Receives commands from CSP Interface
   - Manages AI application lifecycle (start/stop ROS2 nodes)
   - Maintains application registry (JSON config)
   - Subscribes to AI result topics
   - Publishes status updates
   - Coordinates telemetry generation

3. **Camera Driver Nodes** (ROS2)
   - RGB camera: `usb_cam` or `v4l2_camera` package
   - IR camera: Custom driver (when hardware arrives)
   - Publishes to standard ROS2 image topics

4. **AI Application Nodes** (ROS2)
   - Subscribe to camera topics
   - Run AI inference (TensorRT-optimized models)
   - Publish results to output topics
   - Examples: cloud_detection, existing_app

5. **Resource Monitor** (ROS2 Node)
   - Tracks CPU, GPU, memory usage
   - Monitors Jetson health (temperature, power)
   - Publishes resource telemetry
   - Forwards critical metrics to CSP Interface

### Phase 2: Optional Containerization

- Dockerize AI applications as alternative deployment
- Each container includes ROS2 + AI dependencies
- Containers use `--network=host` for Zenoh
- Application Manager extended with Docker SDK
- Trade-off analysis: native vs. containerized

### Data Flow

```
┌─────────────────────────────────────────────────┐
│            Payload Subsystem                    │
│                                                 │
│ ┌───────────┐             ┌──────────────┐      │
│ │  Camera   │─ROS2+Zenoh──│   AI Apps    │      │
│ │  Driver   │             │ (ROS2 Nodes) │      │
│ │(ROS2 Node)│             └──────┬───────┘      │
│ └───────────┘                    │ ROS2+Zenoh   │
│                                  │              │
│                          ┌────────────────┐     │
│                          │   Application  │     │
│                          │    Manager     │     │
│                          │  (ROS2 Node)   │     │
│                          └────────┬───────┘     │
│                                   │             │
│                          ┌────────┴────────┐    │
│                          │  CSP Interface  │    │
│                          │   (Python +     │    │
│                          │    libcsp)      │    │
│                          └────────┬────────┘    │
│                                   │             │
└───────────────────────────────────┼─────────────┘
                                    │
                       CSP/CAN      │    Zenoh/GigE
                      (Commands)    │    (Images)
                                    │
                               ┌─────────┐
                               │  C&DH   │
                               └─────────┘
```

## 🚀 Development Status

### Jetson - Phase 1 (Native ROS2)
- [ ] SpaceROS installation and setup
- [ ] CSP Interface implementation
- [ ] Application Manager (ROS2 node)
- [ ] Camera driver integration
- [ ] Custom AI app development
- [ ] Integration of existing AI app
- [ ] Resource Monitor implementation
- [ ] CSP/CAN integration with C&DH
- [ ] Zenoh/GigE data publishing
- [ ] End-to-end testing

### Jetson - Phase 2 (Optional)
- [ ] Dockerfiles for AI applications
- [ ] Docker-based Application Manager extension
- [ ] Trade-off analysis (native vs. container)
- [ ] Performance benchmarking

### FPGA (Future)
- [ ] Development environment setup
- [ ] Basic hardware design
- [ ] Interface to Jetson
- [ ] Custom processing pipeline
- [ ] Simulation and verification
- [ ] Hardware testing

## 🔧 Build Instructions

### Phase 1 Setup

```bash
TBD
```

## 🧪 Testing

### Unit Tests
```bash
TBD
```

## ⚙️ Configuration

```json
TBD
```

## 🤖 AI Models

### Supported Frameworks
- **PyTorch**: Via TorchScript
- **TensorFlow**: Via TensorRT
- **ONNX**: Via TensorRT

### Model Optimization Pipeline
1. Train model (PyTorch/TensorFlow)
2. Export to ONNX
3. Convert to TensorRT
4. Optimize (INT8 quantization, FP16)
5. Benchmark on Jetson
6. Deploy as ROS2 node


## 🤝 Contributing

When contributing to Payload:
- Test on actual Jetson hardware
- Optimize models with TensorRT
- Follow ROS2 node design patterns
- Document CSP interface changes
- Provide sample data for testing
- Consider power and thermal constraints
- Benchmark all AI models

## 📖 Related Documentation

- [SpaceROS Documentation](https://space.ros.org/)
- [Application Manager API](../../docs/api/application-manager.md)
- [AI Model Guidelines](../../docs/tutorials/ai-models.md)
- [CSP Protocol Interface](../../docs/architecture/csp-protocol.md)
- [Zenoh Publishing Guide](../../docs/architecture/zenoh-publishing.md)
- [FPGA Development Guide](../../docs/tutorials/fpga-dev.md) (future)

## 📚 References

1. JAXA RACS: H. Kato et al., "ROS and cFS System (RACS)", IEEE Aerospace 2021
2. Eclipse Zenoh RMW: https://github.com/ros2/rmw_zenoh
3. SpaceROS: A. Probe et al., "Space ROS: An Open-Source Framework", AIAA 2023
4. TensorRT Optimization: https://developer.nvidia.com/tensorrt
