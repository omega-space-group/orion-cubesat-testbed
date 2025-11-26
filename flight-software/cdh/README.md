# Command & Data Handling (C&DH)

The C&DH subsystem is the central flight computer responsible for commanding all satellite subsystems, collecting and processing telemetry, managing the mission timeline, and handling communications with the ground station.

## 🎯 Responsibilities

- **Command Distribution**: Parse and route commands to appropriate subsystems
- **Telemetry Collection**: Gather, format, and store telemetry data
- **Housekeeping**: Monitor system health and perform routine tasks
- **Time Management**: Maintain onboard time and schedule operations
- **Mode Management**: Control satellite operational states (Safe, Nominal, Payload modes)
- **Event System**: Unified logging and event management across subsystems
- **Communication Interface**: Bridge between ground station and satellite systems

## 🏗️ Architecture

This C&DH implementation follows a **hybrid approach inspired by JAXA's RACS initiative**:

- **Software Architecture**: Custom Python implementation inspired by NASA cFS patterns
  - Modular application design (Mode Manager, Telecommand Handler, Telemetry Manager, etc.)
  - Software bus concept for inter-module communication
  - Table-driven configuration (YAML files, hot-reloadable)
  - Event services for unified logging
- **Communication Protocol**: CubeSat Space Protocol (CSP) over CAN bus
  - Primary interface for all subsystem communication (EPS, Payload)
  - Proven flight heritage in CubeSat missions
- **High-Bandwidth Interface**: Minimal DDS subscriber for large payload data
  - Lightweight DDS library (CycloneDDS or FastDDS) 
  - Does NOT require full ROS2 installation
  - Subscribes to payload image/result topics for ground downlink

**Note**: This is NOT a full ROS2 implementation. The C&DH uses proven satellite bus patterns with CSP/CAN as the primary protocol, only adding minimal DDS capability for high-bandwidth payload data when needed.

## 🖥️ Hardware Platforms

### Raspberry Pi 4 (Phase 1 - Development)
- **Purpose**: Initial development and functional validation
- **OS**: Ubuntu 22.04
- **Software**: Python 3.10+ with cFS-inspired architecture
- **Benefits**: Rapid prototyping, full Linux environment, ease of development

### STM32 Nucleo (Phase 2 - Production)
- **Purpose**: Flight-ready system with RTOS
- **OS**: FreeRTOS
- **Software**: C/C++ port of Phase 1 Python implementation
- **Benefits**: Real-time performance, lower power consumption, flight heritage

## 📁 Directory Structure

```
cdh/
├── rpi4/           # Raspberry Pi 4 implementation
│   ├── src/        # Source code (Python)
│   │   ├── mode_manager.py
│   │   ├── telecommand_handler.py
│   │   ├── telemetry_manager.py
│   │   ├── housekeeping.py
│   │   ├── time_services.py
│   │   ├── event_system.py
│   │   └── csp_interface.py
│   ├── config/     # Configuration files (YAML)
│   │   ├── system_config.yaml
│   │   ├── csp_config.yaml
│   │   └── tables/
│   └── tests/      # Unit and integration tests
└── stm32/          # STM32 implementation (Phase 2)
    ├── src/        # C/C++ source
    ├── include/
    ├── config/
    └── tests/
```

## 🔌 Interfaces

### CSP over CAN (Primary Protocol)
**To EPS:**
- Commands: Power on/off subsystems, request status
- Telemetry: Voltage, current, battery state

**To Payload:**
- Commands: START_APP, STOP_APP, REQUEST_STATUS, CHANGE_MODE
- Telemetry: Application status, resource usage, AI result summaries

**From Ground (via Comms subsystem):**
- Telecommands for satellite operations
- Configuration updates

### DDS over Gigabit Ethernet (High-Bandwidth Data)
**From Payload:**
- Camera images for ground downlink
- Large AI processing results
- Detailed data products

**Implementation:**
- Minimal DDS subscriber (not full ROS2)
- Uses CycloneDDS or FastDDS C/Python API
- Receives data published by Payload's ROS2 nodes

### Output to Ground
- Telemetry downlink via Comms subsystem
- Stored data products for scheduled downlink

## 🏗️ Software Architecture

### cFS-Inspired Components

1. **Mode Manager**
   - Implements operational state machine
   - States: Safe Mode, Nominal Mode, Payload Mode
   - Mode transitions based on commands or system conditions
   - Inspired by cFS Executive Services

2. **Telecommand Handler**
   - Receives CSP packets over CAN
   - Command validation and parsing
   - Command routing and execution
   - Acknowledgment generation
   - Follows cFS Command Ingest patterns

3. **Telemetry Manager**
   - Collects telemetry from all C&DH modules
   - Packages in defined format (JSON or binary)
   - Sends via CSP over CAN
   - Local logging for analysis
   - Follows cFS Telemetry Output patterns

4. **Housekeeping Module**
   - Periodic health data collection (5-10s interval)
   - System metrics: CPU, memory, disk, CAN status
   - Subsystem monitoring: EPS responding, Payload alive
   - Publishes as telemetry
   - Follows cFS Health & Safety patterns

5. **Time Synchronization Module**
   - C&DH as time master
   - Periodic time broadcast via CSP
   - Follows cFS Time Services patterns

6. **Event Messaging System**
   - Unified logging across modules
   - Severity levels: INFO, WARNING, ERROR, CRITICAL
   - Event IDs and descriptions
   - Follows cFS Event Services patterns

7. **Configuration System**
   - Table-driven configuration (YAML files)
   - Hot-reloadable parameters
   - System parameters, thresholds, subsystem configs
   - Follows cFS Table Management patterns

### Communication Flow
```
Ground Station → Comms → [CSP/CAN] → C&DH → [Command Parser] → Subsystems
                                      ↓
                                [CSP Interface]
                                      ↓
                            EPS ← CSP/CAN → Payload
                                      ↑
                               [Telemetry Collector]
                                      ↓
Subsystems → C&DH → [CSP/CAN] → Comms → Ground Station

Payload Images → [DDS/GigE] → C&DH → Store for Downlink
```

## 🚀 Development Status

### Raspberry Pi 4 Implementation (Phase 1)
- [ ] Basic system initialization
- [ ] CSP over CAN interface implementation
- [ ] Mode Manager module
- [ ] Telecommand Handler module
- [ ] Telemetry Manager module
- [ ] Housekeeping module
- [ ] Time Services module
- [ ] Event System module
- [ ] Configuration system (YAML tables)
- [ ] DDS subscriber for payload data (optional)
- [ ] Integration with EPS subsystem
- [ ] Integration with Payload subsystem
- [ ] End-to-end testing

### STM32 Implementation (Phase 2 - Future)
- [ ] Hardware Abstraction Layer design
- [ ] FreeRTOS port
- [ ] C/C++ conversion from Python
- [ ] Real-time scheduler
- [ ] Flash memory management
- [ ] Watchdog implementation

## 🔧 Build Instructions

### Phase 1 - Raspberry Pi 4

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install python3.10 python3-pip can-utils

# Install Python libraries
pip3 install python-can pyyaml psutil

# Install libcsp
git clone https://github.com/libcsp/libcsp.git
cd libcsp && mkdir build && cd build
cmake .. && make && sudo make install

# Configure CAN interface
sudo ip link set can0 type can bitrate 250000
sudo ip link set up can0

# Build (if applicable)
cd flight-software/cdh/rpi4
./build.sh

# Run C&DH
python3 src/main.py
```

## 🧪 Testing

```bash
# Unit tests
cd flight-software/cdh/rpi4
pytest tests/

# Integration test with mock subsystems
./tests/integration/test_cdh_eps.sh
./tests/integration/test_cdh_payload.sh

# CAN bus testing
# Terminal 1: Start C&DH
python3 src/main.py

# Terminal 2: Send test command via CAN
cansend can0 123#01020304

# Monitor telemetry
candump can0
```

## ⚙️ Configuration

Configuration files are located in `rpi4/config/`:

**system_config.yaml**: System parameters
```yaml
telemetry_rate_hz: 1
command_timeout_ms: 100
mode_transition_rules:
  safe_to_nominal: [battery_voltage > 7.0, no_critical_errors]
  nominal_to_payload: [ground_command, battery_voltage > 7.2]
```

**csp_config.yaml**: CSP network configuration
```yaml
csp:
  address: 1  # C&DH address
  can_interface: can0
  can_bitrate: 250000
  eps_address: 2
  payload_address: 3
  buffer_size: 256
```

**tables/**: cFS-style table files
- `command_table.yaml`: Valid command definitions
- `telemetry_format.yaml`: Telemetry packet formats
- `threshold_table.yaml`: Health monitoring thresholds

## 📊 Performance Requirements

- **Command Latency**: < 100ms from CSP receipt to execution
- **Telemetry Rate**: 1 Hz nominal, 10 Hz during critical operations
- **CPU Usage**: < 50% under nominal load (RPi4)
- **Memory Usage**: < 1GB RAM (RPi4)
- **CAN Bus Load**: < 40% utilization

## 🤝 Contributing

When contributing to C&DH:
- Follow cFS architectural patterns when applicable
- Ensure CSP commands are validated before routing
- Add telemetry points for new features
- Test all command paths thoroughly
- Document command and telemetry formats
- Add configuration to YAML tables, not hardcoded
- Ensure hardware abstraction for future STM32 port

## 📖 Related Documentation

- [cFS Architectural Patterns](../../docs/architecture/cfs-patterns.md)
- [CSP Protocol Specification](../../docs/architecture/csp-protocol.md)
- [Command Specifications](../../docs/api/commands.md)
- [Telemetry Specifications](../../docs/api/telemetry.md)
- [CAN Bus Configuration](../../hardware/bus-configs/can/)
- [Integration with Payload](../../docs/architecture/cdh-payload-interface.md)

## 📚 References

1. NASA core Flight System (cFS): https://cfs.gsfc.nasa.gov/
2. JAXA RACS: H. Kato et al., "ROS and cFS System (RACS)", IEEE Aerospace 2021
3. CubeSat Space Protocol: https://github.com/libcsp/libcsp
