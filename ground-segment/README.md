# Ground Segment

Ground station software for satellite communication and mission control.

## 🎯 Purpose

Provide complete ground infrastructure for:
- Satellite communication and tracking
- Command and control
- Telemetry monitoring
- Mission planning and operations
- Data archival and analysis

## 📁 Structure

```
ground-segment/
├── station/          # Ground station hardware/software
└── mission-control/  # Mission operations and planning
```

## 🖥️ Components

### Ground Station
- HackRF One SDR for RF communication
- Antenna tracking (manual or automated)
- Modulation/demodulation
- Link management

### Mission Control
- Command generation and validation
- Telemetry display and monitoring
- Mission planning tools
- Data archival

## 🔗 Communication with Flatsat

- **RF Link**: Via HackRF One
- **Protocol**: Same as flight software
- **Backup**: Direct Ethernet during testing

## 🚀 Quick Start

```bash
TBD
```

## 📖 Documentation

- [Ground Station Setup](station/README.md)
- [Mission Control Guide](mission-control/README.md)
- [Operations Manual](../docs/tutorials/operations.md)
