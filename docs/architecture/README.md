# System Architecture - ORION CubeSat FlatSat Testbed

## Overview

The ORION CubeSat FlatSat testbed implements a **hybrid architecture** inspired by JAXA's RACS (ROS and cFS System) initiative, combining traditional satellite protocols with modern AI frameworks.

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

### Inspiration: JAXA RACS

JAXA's Robotic Asteroid eXploration (RACS) project demonstrated that hybrid architectures can successfully combine:
- **cFS (core Flight System)**: For spacecraft bus management
- **ROS (Robot Operating System)**: For robotic/AI payload processing
- **Well-defined interfaces**: Clean separation between domains

**Reference**: H. Kato, D. Hirano, S. Mitani, T. Saito and S. Kawaguchi, "ROS and cFS System (RACS): Easing Space Robotic Development," 2021 IEEE Aerospace Conference.

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
              ┌────────────────┴────────────────┐
              │            C&DH                 │
              │   (RPi4 → STM32 + FreeRTOS)    │
              │                                 │
              │  • cFS-inspired architecture    │
              │  • CSP/CAN for control         │
              │  • Minimal DDS subscriber      │
              └─┬──────────────────────────┬───┘
                │ CSP/CAN              CSP/CAN + DDS/GigE
                │                            │
         ┌──────┴──────┐            ┌────────┴─────────┐
         │     EPS     │            │     Payload      │
         │   (STM32)   │            │  (Jetson/FPGA)   │
         │             │            │                  │
         │ • FreeRTOS  │            │ • SpaceROS/ROS2  │
         │ • CSP/CAN   │            │ • CSP interface  │
         │ • Power mgmt│            │ • DDS publishing │
         └─────────────┘            └──────────────────┘
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

* Payload internal: ROS2 DDS (not CSP)
```

#### Layer 2: DDS over Gigabit Ethernet (High-Bandwidth Data)

**Protocol**: Data Distribution Service (DDS) over Gigabit Ethernet  
**Bitrate**: Up to 1 Gbps  
**Effective Throughput**: ~100 MB/s for image data  

**Used For**:
- Camera images (RGB, IR)
- Processed images from AI algorithms
- Large AI processing results
- Bulk data products for ground downlink

**Direction**: Primarily Payload → C&DH (for eventual downlink via Comms)

**Implementation**:
- **Payload**: Publishes to DDS topics from ROS2 nodes (native SpaceROS)
- **C&DH**: Minimal DDS subscriber (lightweight DDS library, NOT full ROS2)
- **Inspiration**: JAXA RACS2 Extended DDS pattern

**Why Not Use ROS2 on C&DH?**
- C&DH doesn't need full ROS2 stack
- Lightweight DDS subscriber is sufficient
- Reduces complexity and resource usage
- Maintains clear separation between bus and payload
- Easier migration to STM32 + FreeRTOS

**Reference**: JAXA RACS2 Extended DDS - https://github.com/jaxa/racs2_extended-dds

### Payload-Internal Communication

**Protocol**: ROS2 DDS (via SpaceROS)

**Used Within Payload Subsystem Only**:
```
Camera Nodes ─┐
              ├─► ROS2 DDS ─► AI Application Nodes
Sensors ──────┘                         │
                                        │ ROS2 DDS
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
- Minimal DDS subscriber: Receive large payload data
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
- Future: Xilinx FPGA for hardware acceleration

**Software Architecture**: Full SpaceROS (ROS2 Humble)

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
   - RGB camera: usb_cam or v4l2_camera
   - IR camera: Custom driver (future)
   
4. **AI Applications** (ROS2 nodes)
   - Cloud detection (TensorRT-optimized)
   - Additional apps as needed
   - Containerization option (Phase 2)
   
5. **Resource Monitor** (ROS2 node)
   - CPU, GPU, memory tracking
   - Temperature and power monitoring

**Communication Interfaces**:
- **External**: CSP/CAN for commands/telemetry to C&DH
- **External**: DDS/GigE for publishing images to C&DH
- **Internal**: ROS2 DDS for inter-node communication

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

## Data Flow Examples

### Example 1: Start AI Application

```
1. Ground Station sends command via RF
                    ↓
2. Comms receives, forwards via CSP/CAN
                    ↓
3. C&DH Telecommand Handler validates command
                    ↓
4. C&DH routes command via CSP/CAN to Payload
                    ↓
5. Payload CSP Interface receives CSP packet
                    ↓
6. CSP Interface calls Application Manager (ROS2)
                    ↓
7. Application Manager starts AI app ROS2 node
                    ↓
8. AI app subscribes to camera topic, begins inference
                    ↓
9. Application Manager sends status via CSP Interface
                    ↓
10. C&DH receives status via CSP/CAN
                    ↓
11. C&DH forwards acknowledgment to ground
```

### Example 2: Image Processing and Downlink

```
1. Camera Driver (ROS2 node) captures image
                    ↓
2. Publishes to /camera/rgb/image_raw (ROS2 DDS)
                    ↓
3. AI App subscribes, runs inference
                    ↓
4. AI App publishes results to /ai/output (ROS2 DDS)
                    ↓
5. Application Manager receives results
                    ↓
        ┌───────────┴───────────┐
        │                       │
6a. Small summary via CSP  6b. Full image via DDS/GigE
    to C&DH CSP interface      to C&DH DDS subscriber
        │                       │
        └───────────┬───────────┘
                    ↓
7. C&DH stores both for downlink queue
                    ↓
8. C&DH schedules downlink via Comms
                    ↓
9. Data transmitted to ground station
```

### Example 3: Housekeeping Telemetry

```
Every 10 seconds:

1. C&DH Housekeeping module collects system metrics
   - CPU usage, temperature, memory
   - CAN bus status
                    ↓
2. Sends housekeeping request via CSP/CAN to EPS
                    ↓
3. EPS responds with power telemetry (battery, voltage, current)
                    ↓
4. Sends housekeeping request via CSP/CAN to Payload
                    ↓
5. Payload CSP Interface queries Application Manager (ROS2)
                    ↓
6. Application Manager queries Resource Monitor (ROS2 node)
                    ↓
7. Resource telemetry sent back via CSP/CAN
                    ↓
8. C&DH Telemetry Manager packages all housekeeping
                    ↓
9. Stored locally and queued for ground downlink
```

## Protocol Selection Guidelines

### When to Use CSP over CAN

✅ **Use for**:
- Commands and telecommands
- Small telemetry packets (< 1 KB)
- Status updates and acknowledgments
- Health monitoring data
- Configuration updates
- Inter-subsystem control messaging

❌ **Don't use for**:
- Large files or images
- High-rate data streams
- Bulk data transfer

### When to Use DDS over GigE

✅ **Use for**:
- Camera images (MB per image)
- Processed image data
- Large AI results
- Bulk data products
- High-bandwidth data streams

❌ **Don't use for**:
- Simple commands
- Small status updates
- Time-critical control messages

## Technology Choices

### Why cFS Patterns for C&DH?

**Advantages**:
- 20+ years of flight heritage
- Modular, proven architecture
- Table-driven configuration
- Well-documented patterns
- Safety-critical design principles

**Our Approach**:
- Inspired by cFS patterns, not a direct port
- Python implementation for rapid development (Phase 1)
- C/C++ port for flight hardware (Phase 2)

### Why SpaceROS for Payload?

**Advantages**:
- Rich AI/ML ecosystem
- Hardware acceleration support (CUDA, TensorRT)
- Extensive robotics libraries
- Strong community and tooling
- Rapid development for AI applications

**Limitations Addressed**:
- Not used for critical bus functions
- Isolated to payload subsystem
- Well-defined interface via CSP

### Why CSP Protocol?

**Advantages**:
- Specifically designed for CubeSats
- Flight-proven (many missions)
- Built-in routing and addressing
- Reliable delivery with ACKs
- Works over multiple physical layers (CAN, I2C, UART)
- Open-source implementation (libcsp)

**Alternative Considered**:
- Raw CAN: No addressing, routing, or reliability
- Custom protocol: Reinventing the wheel

### Why Dual-Layer Communication?

**Rationale**:
1. **Right tool for the job**: Different data types need different protocols
2. **Bandwidth efficiency**: Don't burden CAN with large images
3. **Proven + Modern**: Combine flight heritage (CSP) with high performance (DDS)
4. **Flexibility**: Add high-bandwidth as needed, fallback to CSP-only
5. **JAXA validation**: Similar approach used successfully in RACS

## Migration Path

### Phase 1: Development (Current)

**Hardware**:
- C&DH: Raspberry Pi 4 (Ubuntu)
- Payload: Jetson Xavier NX (Ubuntu)
- EPS: STM32 Nucleo (FreeRTOS)

**Software**:
- C&DH: Python with cFS patterns
- Payload: Native ROS2 nodes (no Docker yet)
- Communication: CSP/CAN + DDS/GigE

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
| High-BW Protocol | DDS over GigE | Images, large data |
| RTOS Choice | FreeRTOS | CubeSat dominance |
| C&DH Hardware (P1) | Raspberry Pi 4 | Development ease |
| C&DH Hardware (P2) | STM32 | Flight-ready, efficient |

## References

1. **JAXA RACS**: H. Kato, D. Hirano, S. Mitani, T. Saito and S. Kawaguchi, "ROS and cFS System (RACS): Easing Space Robotic Development," 2021 IEEE Aerospace Conference, doi: 10.1109/AERO50100.2021.9438288

2. **JAXA RACS Extended DDS**: https://github.com/jaxa/racs2_extended-dds

3. **NASA cFS**: Core Flight System, https://cfs.gsfc.nasa.gov/

4. **SpaceROS**: A. Probe et al., "Space ROS: An Open-Source Framework for Space Robotics and Flight Software," AIAA 2023-2709

5. **libcsp**: CubeSat Space Protocol, https://github.com/libcsp/libcsp

## Conclusion

This hybrid architecture strategically combines:
- **Flight-proven satellite protocols** for reliable, deterministic bus operations
- **Modern AI frameworks** for rich payload processing capabilities
- **Well-defined interfaces** for clean separation of concerns
- **Flexible communication layers** for different data types

The result is a testbed that demonstrates both traditional aerospace engineering and cutting-edge AI technologies, providing an educational platform and validation environment for future CubeSat missions.
