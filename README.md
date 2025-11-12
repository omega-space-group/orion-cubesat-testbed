# ORION CubeSat Flatsat Testbed

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![ROS 2](https://img.shields.io/badge/ROS%202-Humble-blue)]()

CubeSat flatsat testbed for deploying and testing AI algorithms, end-to-end processing pipelines and avionics SW/HW. Supports on-board processing validation and serves as development platform for university student projects and future open-source CubeSat missions.

**Ω-Space Group** | **ORION Lab** | **National Technical University of Athens**

Open-source

---

## 🎯 Overview

Ground-based testbed that mirrors actual CubeSat functionality for comprehensive end-to-end software testing and AI algorithm validation before deployment.

### Key Capabilities

- 🤖 **AI/ML Deployment**: Test algorithms on NVIDIA Jetson and Xilinx FPGA
- 🛰️ **Full Avionics Stack**: C&DH, EPS, Payload, Communications
- 🔄 **End-to-End Testing**: From sensor to ground station
- 🎓 **Educational**: Platform for student projects and learning
- 🌍 **Open Source**: GPL-3.0, following Libre Space Foundation principles

---

## 🏗️ System Architecture

### Hardware

| Subsystem | Hardware | Software | Communication |
|-----------|----------|----------|---------------|
| **C&DH** | Raspberry Pi 4 → STM32 Nucleo | Ubuntu → FreeRTOS | CAN + GigE |
| **EPS** | STM32 Nucleo | FreeRTOS | CAN |
| **Payload** | NVIDIA Jetson + Xilinx FPGA | Ubuntu | GigE |
| **Comms** | HackRF One SDR | GNU Radio | RF Link |

### Software Stack

- **Operating Systems**: Ubuntu 22.04 (prototype), FreeRTOS (production)
- **Middleware**: Space ROS 2 (Humble)
- **Communication**: CAN bus (critical), Gigabit Ethernet (high-bandwidth), CSP protocol
- **Ground Segment**: SDR-based (GNU Radio)

### Block Diagram

```
┌─────────────────────┐
│   Ground Station    │
│   (HackRF + GUI)    │
└──────────┬──────────┘
           │ RF
    ┌──────┴──────┐
    │    COMMS    │
    │  (HackRF)   │
    └──────┬──────┘
           │ GigE + CAN
    ┌──────┴──────────────────────┐
    │         C&DH                │
    │  (RPi4/STM32 + Space ROS)   │
    └─┬────────────────────────┬──┘
      │ CAN                    │ GigE + CAN
┌─────┴─────┐           ┌──────┴────────┐
│    EPS    │           │   Payload     │
│  (STM32)  │           │ (Jetson/FPGA) │
│           │           │               │
└───────────┘           └───────────────┘
```

---

## 📁 Repository Structure

```
orion-cubesat-testbed/
├── flight-software/      # On-board SW (C&DH, EPS, Payload, Comms)
├── ground-segment/       # Ground station & mission control
├── middleware/           # Space ROS & interfaces
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
git clone https://github.com/YOUR-USERNAME/orion-cubesat-testbed.git
cd orion-cubesat-testbed

# Setup environment
./scripts/setup.sh

# Build
./scripts/build.sh

# Run tests
./scripts/test.sh
```

**Prerequisites**: Ubuntu 22.04, ROS 2 Humble, Python 3.10+


---

## 🔧 Current Development Status


### 🔄 In Progress
- C&DH basic functionality (Space ROS on RPi4)
- CAN bus communication protocol
- Space ROS integration
- Payload containerization framework
- AI model deployment

### ⏳ Planned
- Full subsystem integration (Comms, EPS)
- Ground station implementation
- End-to-end testing

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
- [Space ROS](https://space.ros.org/) - ROS 2 for space applications
- [ROS 2 Documentation](https://docs.ros.org/en/humble/) - Robot Operating System 2
- [micro-ROS](https://micro.ros.org/) - ROS 2 for microcontrollers (STM32)
- [FreeRTOS](https://www.freertos.org/) - Real-time operating system

### Communication & Protocols
- [libcsp](https://github.com/libcsp/libcsp) - CubeSat Space Protocol library
- [SocketCAN](https://www.kernel.org/doc/html/latest/networking/can.html) - Linux CAN bus support

### SDR & Ground Station
- [GNU Radio](https://www.gnuradio.org/) - Software-defined radio toolkit
- [HackRF](https://greatscottgadgets.com/hackrf/) - Software-defined radio platform

### Containerization & Deployment
- [Docker](https://www.docker.com/) - Container platform
- [NVIDIA Container Toolkit](https://github.com/NVIDIA/nvidia-docker) - GPU support in containers

### AI/ML on Edge
- [TensorRT](https://developer.nvidia.com/tensorrt) - NVIDIA inference optimizer
- [Jetson Linux](https://developer.nvidia.com/embedded/jetson-linux) - Jetson development resources

---

## 📧 Contact

**Ω-Space Group**  
**ORION Lab**  
National Technical University of Athens  

Website: [https://orionlab.space.noa.gr/](https://orionlab.space.noa.gr/)  

---

**Status**: 🔄 Active Development | **Version**: 0.1.0-alpha
