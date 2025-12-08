# System Architecture - ORION CubeSat FlatSat Testbed

## Overview

The ORION CubeSat FlatSat testbed implements a **hybrid architecture**, combining traditional satellite protocols with modern AI frameworks.

## Design Philosophy

### Why Hybrid?

Our architecture strategically uses different frameworks for different purposes:

1. **Satellite Bus (C&DH, EPS)**: Traditional aerospace patterns
   - Proven flight heritage
   - Real-time deterministic behavior
   - Safety-critical operations
   - Resource-efficient

2. **AI Payload**: Modern ROS2 framework
   - Rich AI/robotics ecosystem
   - Extensive library support
   - Rapid development for AI applications
   - Hardware acceleration support


## System Architecture

### High-Level Block Diagram

```
                    ┌─────────────────────┐
                    │   Ground Station    │
                    │    (GNU Radio)      │
                    └──────────┬──────────┘
                               │ RF (UHF/VHF/S)
                        ┌──────┴──────┐
                        │    COMMS    │
                        │  (HackRF)   │
                        └──────┬──────┘
                               │ CSP/CAN + GigE
              ┌────────────────┴───────────────┐
              │            C&DH                │
              │   (RPi4 / STM32 + FreeRTOS)    │
              │                                │
              │  • cFS-inspired architecture   │
              │  • CSP/CAN for control         │
              │  • Zenoh subscriber            │
              └─┬──────────────────────────┬───┘
                │                          │
             CSP/CAN               CSP/CAN + Zenoh/GigE
                │                          │
         ┌──────┴──────┐          ┌────────┴──────────┐
         │     EPS     │          │     Payload       │
         │   (STM32)   │          │  (Jetson/FPGA)    │
         │             │          │                   │
         │ • FreeRTOS  │          │ • SpaceROS/ROS2   │
         │ • CSP/CAN   │          │ • CSP interface   │
         │ • Power mgmt│          │ • Zenoh publishing│
         └─────────────┘          └───────────────────┘
```

## Communication Architecture

### Dual-Layer Communication

Our architecture uses **two communication layers** for different data types:

#### Layer 1: CSP over CAN (Control & Telemetry)

**Protocol**: CubeSat Space Protocol (CSP) over CAN bus  
**Bitrate**: 250 kbps CAN bus  
**Effective Throughput**: ~40-60 kbps  

**Used For**:
- Telecommands (C&DH → Subsystems)
- Telemetry (Subsystems → C&DH)
- Status updates and health monitoring
- Time synchronization
- Small data packets

**Advantages**:
- Flight-proven protocol (used in many CubeSats)
- Reliable packet delivery with acknowledgments
- Addressing and routing built-in
- Low overhead
- Deterministic behavior

**Communication Matrix**:
```
┌─────────┬─────────┬─────────┬─────────┬─────────┐
│ From/To │  C&DH   │   EPS   │ Payload │  Comms  │
├─────────┼─────────┼─────────┼─────────┼─────────┤
│  C&DH   │    -    │ CSP/CAN │ CSP/CAN │ CSP/CAN │
│  EPS    │ CSP/CAN │    -    │    -    │    -    │
│ Payload │ CSP/CAN │    -    │    *    │    -    │
│  Comms  │ CSP/CAN │    -    │    -    │    -    │
└─────────┴─────────┴─────────┴─────────┴─────────┘

* Payload internal: ROS2 with Zenoh middleware (not CSP)
```

#### Layer 2: Zenoh over Gigabit Ethernet (High-Bandwidth Data)

**Protocol**: Eclipse Zenoh over Gigabit Ethernet  
**Bitrate**: Up to 1 Gbps  
**Effective Throughput**: ~100 MB/s for image data  

**Used For**:
- Camera images (RGB, IR)
- Processed images from AI algorithms
- Large AI processing results
- Bulk data products for ground downlink

**Direction**: Primarily Payload → C&DH (for eventual downlink via Comms)

**Implementation**:
- **Payload**: Publishes to Zenoh topics from ROS2 nodes (native SpaceROS with rmw_zenoh)
- **C&DH**: Zenoh subscriber (lightweight Zenoh library, NOT full ROS2)


### Payload-Internal Communication

**Protocol**: ROS2 with Zenoh middleware (via SpaceROS)

**Used Within Payload Subsystem Only**:
```
Camera Nodes ─┐
              ├─► ROS2 with Zenoh middleware ─► AI Application Nodes
Sensors ──────┘                         │
                                        │ ROS2 with Zenoh middleware
                                        ▼
                            Application Manager
                                        │
                                        │ (Bridge)
                                        ▼
                                  CSP Interface
                                        │ CSP/CAN
                                        ▼
                                      C&DH
```

## Subsystem Details

### C&DH (Command & Data Handling)

**Purpose**: Central flight computer managing satellite operations

**Hardware**:
- Phase 1: Raspberry Pi 4 (Ubuntu 22.04)
- Phase 2: STM32 Nucleo (FreeRTOS) ← flight-ready

**Software Architecture**: cFS-inspired custom Python implementation

**Key Components**:
1. **Mode Manager**: Satellite state machine (Safe/Nominal/Payload modes)
2. **Telecommand Handler**: Command parsing, validation, routing
3. **Telemetry Manager**: Data collection, formatting, storage
4. **Housekeeping**: System health monitoring
5. **Time Services**: Onboard time management and sync
6. **Event System**: Unified logging across modules
7. **Configuration System**: Table-driven config (YAML)

**Communication Interfaces**:
- CSP over CAN: Primary protocol (all subsystems)
- Zenoh subscriber: Receive large payload data
- RF link via Comms: Ground station interface

**Design Pattern**: NASA cFS architectural patterns
- Modular applications
- Software bus concept
- Table-driven configuration
- Event services
- Time services

### Payload

**Purpose**: AI processing, computer vision, data analysis

**Hardware**:
- Primary: NVIDIA Jetson Xavier NX
- Future: Xilinx FPGA

**Software Architecture**: Full SpaceROS

**Key Components**:
1. **Application Manager** (ROS2 node)
   - Manages AI application lifecycle
   - Receives commands from CSP Interface
   - Monitors resource usage
   
2. **CSP Interface** (Python + libcsp)
   - Bridges between CSP/CAN and ROS2 domains
   - Converts CSP commands to ROS2 function calls
   - Generates CSP telemetry from ROS2 data
   
3. **Camera Drivers** (ROS2 nodes)
   - RGB camera: TBD
   - IR camera: TBD
   
4. **AI Applications** (ROS2 nodes)
   - Custom + third-party 
   - Containerization option (Phase 2)
   
5. **Resource Monitor** (ROS2 node)
   - CPU, GPU, memory tracking
   - Temperature and power monitoring

**Communication Interfaces**:
- **External**: CSP/CAN for commands/telemetry to C&DH
- **External**: Zenoh/GigE for publishing images to C&DH
- **Internal**: ROS2 with Zenoh middleware for inter-node communication

### EPS (Electrical Power System)

**Purpose**: Power generation, distribution, battery management

**Hardware**: STM32 Nucleo

**Software**: FreeRTOS-based control

**Communication**: CSP over CAN to C&DH

### Comms (Communications)

**Purpose**: RF link to ground station

**Hardware**: HackRF One SDR

**Software**: GNU Radio flowgraphs

**Protocols**: CCSDS TM/TC

**Communication**: CSP/CAN to C&DH for data buffering


## Migration Path

### Phase 1: Development (Current)

**Hardware**:
- C&DH: Raspberry Pi 4 (Ubuntu)
- Payload: Jetson Xavier NX (Ubuntu)
- EPS: STM32 Nucleo (FreeRTOS)

**Software**:
- C&DH: Python with cFS patterns
- Payload: Native ROS2 nodes (no Docker yet)
- Communication: CSP/CAN + Zenoh/GigE

**Goals**:
- Rapid prototyping
- Functional validation
- Integration testing
- Student thesis projects

### Phase 2: Flight-Representative

**Hardware**:
- C&DH: STM32 Nucleo (FreeRTOS)
- Payload: Same (Jetson)
- EPS: Same (STM32)

**Software**:
- C&DH: C/C++ port with Hardware Abstraction Layer
- Payload: Optional containerization for apps
- Communication: Same protocols

**Goals**:
- Real-time deterministic behavior
- Resource optimization
- Power optimization
- Flight-ready characteristics

## Design Decisions Summary

| Aspect | Decision | Rationale |
|--------|----------|-----------|
| C&DH Architecture | cFS-inspired | Flight heritage, modularity |
| C&DH Language (P1) | Python | Rapid development |
| C&DH Language (P2) | C/C++ | Real-time, efficiency |
| Payload Framework | SpaceROS/ROS2 | AI ecosystem, tooling |
| Primary Protocol | CSP over CAN | CubeSat flight heritage |
| High-BW Protocol | Zenoh over GigE | Images, large data |
| RTOS Choice | FreeRTOS | CubeSat dominance |
| C&DH Hardware (P1) | Raspberry Pi 4 | Development ease |
| C&DH Hardware (P2) | STM32 | Flight-ready, efficient |

## References

1. **JAXA RACS**: H. Kato, D. Hirano, S. Mitani, T. Saito and S. Kawaguchi, "ROS and cFS System (RACS): Easing Space Robotic Development," 2021 IEEE Aerospace Conference, doi: 10.1109/AERO50100.2021.9438288

2. **Eclipse Zenoh RMW**: https://github.com/ros2/rmw_zenoh

3. **NASA cFS**: Core Flight System, https://cfs.gsfc.nasa.gov/

4. **SpaceROS**: A. Probe et al., "Space ROS: An Open-Source Framework for Space Robotics and Flight Software," AIAA 2023-2709

5. **libcsp**: CubeSat Space Protocol, https://github.com/libcsp/libcsp
