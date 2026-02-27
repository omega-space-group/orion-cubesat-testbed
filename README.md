# ORION CubeSat Flatsat Testbed

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Python 3.10+](https://img.shields.io/badge/python-3.10+-blue.svg)](https://www.python.org/downloads/)

CubeSat flatsat testbed for deploying and testing AI algorithms, end-to-end processing pipelines and avionics SW/HW. Supports on-board processing validation and serves as development platform for university student projects and future open-source CubeSat missions.

**Ω-Space Group** | **ORION Lab** | **National Technical University of Athens**

Open-source

---

## 🎯 Overview

Ground-based testbed that mirrors actual CubeSat functionality for comprehensive end-to-end software testing and AI algorithm validation before deployment.

### Key Capabilities

- 🤖 **AI/ML Deployment**: Test algorithms on GPUs, FPGAs, Neural/Tensor/Neuromorphic Processors
- 🛰️ **Hybrid Architecture**: Flight-proven protocols + modern AI frameworks
- 🔄 **End-to-End Testing**: From sensor to ground station
- 🎓 **Educational**: Platform for student projects and learning
- 🌍 **Open Source**: GPL-3.0, following Libre Space Foundation principles

---

## 🏗️ System Architecture

### Hybrid Architecture Approach

Our testbed implements a **hybrid architecture** that combines proven satellite protocols with modern AI capabilities:

- **Satellite Bus (C&DH, EPS)**: Custom flight software inspired by NASA cFS patterns with CubeSat Space Protocol (CSP) over CAN
- **AI Payload**: Zenoh middleware for a pub/sub, AI-enabled cognitive payload 
- **Communication**: Dual-layer approach
  - **CSP over CAN**: Control messaging (commands, telemetry, status)
  - **Zenoh over GigE**: High-bandwidth data transfer (images, large AI results)

### Hardware

| Subsystem | Hardware | Software | Primary Protocol |
|-----------|----------|----------|------------------|
| **C&DH** | Raspberry Pi 4 → STM32 Nucleo | Ubuntu → FreeRTOS | CSP/CAN |
| **EPS** | STM32 Nucleo | FreeRTOS | CSP/CAN |
| **Payload** | NVIDIA Jetson / Xilinx Ultrascale+ | Ubuntu | CSP/CAN + Zenoh/GigE |
| **Comms** | HackRF One SDR | GNU Radio | RF Link |

### Software Stack

- **Operating Systems**: Ubuntu 22.04, FreeRTOS
- **C&DH Middleware**: Custom cFS-inspired architecture with CSP
- **Communication Protocols**: 
  - CubeSat Space Protocol (CSP) over CAN bus
  - Zenoh over Gigabit Ethernet (payload data)
- **Ground Segment**: SDR-based (GNU Radio)

### Block Diagram

```
                           ┌─────────────────────┐
                           │   Ground Station    │
                           │    (SDR + GUI)      │
                           └──────────┬──────────┘
                                      │ RF
                                      |
                               ┌──────┴───────┐
                               │     COMMS    │
                               │     (SDR)    │
                               └─┬─────┬──────┘
                                 |     |
                         CSP/CAN |     |
                                 |     |
    ┌──────────────────────────────┐   |
    │            C&DH              │   | GigE
    │  (RPi4/STM32 + cFS-inspired) │   |
    │   CSP/CAN                    │   |
    └─┬────────────────────────┬───┘   |
      │ CSP/CAN        CSP/CAN │       |
┌─────┴─────┐           ┌──────┴───────┴────┐
│    EPS    │           │     Payload       │
│  (STM32)  │           │   (Jetson/FPGA)   │
│           │           │ SpaceROS + Zenoh  │
│  CSP/CAN  │           │   CSP Interface   │
└───────────┘           └───────────────────┘
```

### Communication Layers

**Control Layer (CSP over CAN)**
- Telecommands: C&DH → Payload/EPS
- Telemetry: Payload/EPS → C&DH  
- Status updates and health monitoring
- Time synchronization

**High-Bandwidth Layer (Zenoh over GigE)**
- Camera images: Payload → C&DH
- Processed AI results: Payload → C&DH
- Large data products for ground downlink

---

## 📁 Repository Structure

```
orion-cubesat-testbed/
├── flight-software/      # On-board SW (C&DH, EPS, Payload, Comms)
├── ground-segment/       # Ground station & mission control
├── middleware/           # Payload-internal interfaces
├── hardware/             # HW docs, CAN/GigE configs, BOM
├── simulation/           # Testing infrastructure
├── tools/                # Build & deployment utilities
├── scripts/              # Setup scripts
├── research/             # Publications & results
└── docs/                 # Documentation
```


---

## 🚀 Quick Start

```bash
# Clone repository
git clone https://github.com/omega-space-group/orion-cubesat-testbed.git
cd orion-cubesat-testbed

# Setup environment
./scripts/setup.sh

# Build
./scripts/build.sh

# Run tests
./scripts/test.sh
```

**Prerequisites**: Ubuntu 22.04, Python 3.10+, libcsp

**For Payload Development**: CUDA, TensorRT (Jetson only), Zenoh


---

## 🔧 Current Development Status


### 🔄 In Progress
- C&DH cFS-inspired software
- CSP over CAN bus implementation
- Payload framework (Jetson)
- AI model deployment and optimization
- Zenoh over GigE for high-bandwidth data

### ⏳ Planned
- Full subsystem integration (Comms, EPS)
- STM32 + FreeRTOS migration for C&DH
- Ground station implementation
- End-to-end mission testing

---

## 🤝 Contributing

Contributions welcome! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

**Quick workflow:**
1. Fork the repository
2. Create feature branch
3. Commit changes
4. Submit Pull Request

---

## 📄 License

GPL-3.0 License - see [LICENSE](LICENSE) for details.

---

## 🔗 Related Projects & Resources

### Core Technologies
- [NASA cFS](https://cfs.gsfc.nasa.gov/) - Core Flight System (architectural inspiration)
- [Space ROS](https://space.ros.org/) - ROS 2 for space applications
- [FreeRTOS](https://www.freertos.org/) - Real-time operating system

### Communication & Protocols
- [libcsp](https://github.com/libcsp/libcsp) - CubeSat Space Protocol library
- [SocketCAN](https://www.kernel.org/doc/html/latest/networking/can.html) - Linux CAN bus support
- [Eclipse Zenoh](https://zenoh.io/) - Zenoh middleware
- [Zenoh ROS2 Middleware](https://github.com/ros2/rmw_zenoh) - A ROS 2 RMW implementation based on Zenoh

### Research References
- [JAXA RACS](https://ieeexplore.ieee.org/abstract/document/9438288) - Hybrid ROS/cFS approach


### SDR & Ground Station
- [GNU Radio](https://www.gnuradio.org/) - Software-defined radio toolkit
- [HackRF](https://greatscottgadgets.com/hackrf/) - Software-defined radio platform

### AI/ML on Edge
- [TensorRT](https://developer.nvidia.com/tensorrt) - NVIDIA inference optimizer
- [Jetson Linux](https://developer.nvidia.com/embedded/jetson-linux) - Jetson development resources

---

## 📧 Contact

**Ω-Space Group**  
**ORION Lab**  
National Technical University of Athens  

- Simon Vellas: svellas@mail.ntua.gr
- Christos Chronis: chronis@hua.gr
- Alexis Apostolakis: alexis.apostolakis@gmail.com
- Giorgos Athanasiou: georgios.athanasiou.ntua@gmail.com

---

**Status**: 🔄 Active Development | **Version**: 0.1.0-alpha

### Versioning

We use [Semantic Versioning](https://semver.org/): `MAJOR.MINOR.PATCH-STAGE`
- Versions `0.x.x` indicate active development
- Version `1.0.0` will be the first stable release
- See git tags for release history
