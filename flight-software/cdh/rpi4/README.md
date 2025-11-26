# C&DH - Raspberry Pi 4 Implementation

Prototype C&DH implementation running on Raspberry Pi 4 with Ubuntu.

## Purpose

This implementation serves as the initial development and testing platform for C&DH functionality before transitioning to the flight-ready STM32 implementation.

## Hardware Setup

- **Board**: Raspberry Pi-4 8GB
- **OS**: Ubuntu 22.04 LTS (64-bit)
- **Storage**: 64GB microSD card
- **Interfaces**:
  - CAN: via RS485 CAN HAT
  - Ethernet: Onboard Gigabit Ethernet
  - GPIO: For subsystem interfacing

## Software Stack

- **OS**: Ubuntu 22.04
- **Framework**: core Flight System (cFS)
- **Language**: Python 3.10 / C++17
- **Dependencies**: Listed in `requirements.txt` and `package.xml`

## Quick Start

```bash
# Install dependencies
sudo apt update
sudo apt install -y python3-pip can-utils

# Setup Python environment
pip3 install -r requirements.txt

# Build
TBD

# Run
TBD
```

## Configuration

Edit `config/cdh_config.yaml` to configure:
- CAN interface settings
- Network parameters
- Telemetry rates
- Command timeouts

## Testing

```bash
TBD
```

## Development Notes

- This implementation uses standard Linux interfaces (SocketCAN, TCP/UDP)
- Code should be written with future STM32 porting in mind
- Avoid platform-specific features that won't be available on embedded RTOS

## Next Steps

- [ ] Setup Environment
- [ ] Setup CAN communication
- [ ] Setup GigE communication
- [ ] Implement control SW
- [ ] Implement housekeeping SW
- [ ] Implement basic command parser
- [ ] Add telemetry collection
- [ ] Test with other subsystems
