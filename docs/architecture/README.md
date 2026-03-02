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

2. **AI Payload**: Modern pub/sub framework
   - Linux OS + Python
   - Extensive library support
   - Rapid development for AI applications
   - Hardware acceleration support


## System Architecture

### High-Level Block Diagram

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
│           │           │       Zenoh       │
│  CSP/CAN  │           │   CSP Interface   │
└───────────┘           └───────────────────┘
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
- Flight-proven protocol
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
│ Payload │ CSP/CAN │    -    │    -    │   GigE  │
│  Comms  │ CSP/CAN │    -    │    -    │    -    │
└─────────┴─────────┴─────────┴─────────┴─────────┘

* Payload internal: Zenoh pub/sub for multi-app coordination
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

**Direction**: Primarily Payload → COMMS (Direct link to COMMS with C&DH control)

**Implementation**:
- **Payload**: Publishes to Zenoh topics
- **COMMS**: Zenoh subscriber (TBD)


### Payload-Internal Communication

**Protocol**: Zenoh

**Used Within Payload Subsystem Only**:
```
Camera ───────┐
              ├─► Zenoh nodes ─► AI Application Nodes
Sensors ──────┘                         │
                                        │ Zenoh pub/sub
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
- Phase 1: Raspberry Pi 4 (Ubuntu)
- Phase 2: STM32 Nucleo (FreeRTOS) ← flight-ready

**Software Architecture**: cFS-inspired custom Python implementation

**Key Components**:
TBD

**Communication Interfaces**:
- CSP over CAN: Primary protocol (all subsystems)
- Zenoh subscriber: Receive large payload data
- RF link via Comms: Ground station interface

**Design Pattern**: NASA cFS architectural patterns
- Modular applications
- Software bus concept

### Payload

**Purpose**: AI processing, computer vision, data analysis

**Hardware**:
- Primary: NVIDIA Jetson Xavier NX
- Future: Xilinx FPGA

**Key Components**:
TBD

**Communication Interfaces**:
- **External**: CSP/CAN for commands/telemetry to C&DH
- **External**: Zenoh/GigE for publishing Data to COMMS
- **Internal**: Zenoh for inter-node communication

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

**Communication**: CSP/CAN to C&DH for control, TM/TC , GigE to Payload for Data
