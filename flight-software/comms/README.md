# Communications (Comms)

RF communications subsystem using HackRF One software-defined radio.

## 🎯 Responsibilities

- **RF Link**: Establish and maintain communication with ground station
- **Data Transfer**: Uplink commands, downlink telemetry and payload data
- **Modulation**: Implement modulation/demodulation schemes
- **Protocol**: Handle communication protocol and error correction

## 🖥️ Hardware

- **SDR**: HackRF One
- **Frequency Range**: 1 MHz - 6 GHz
- **Sample Rate**: Up to 20 MSPS
- **Interface**: USB 2.0 to C&DH computer

## 📁 Directory Structure

```
comms/
└── hackrf/
    ├── src/           # Communication software
    ├── protocols/     # Protocol implementations
    ├── modulation/    # Modulation schemes
    └── tests/         # Unit and integration tests
```

## 🔌 Interfaces

### Internal
- **C&DH**: Commands to transmit, telemetry to send
- **USB**: Connection to HackRF

### External
- **Ground Station**: RF link

## 🏗️ Software Architecture

### Components
1. **SDR Driver**: Interface with HackRF hardware
2. **Modulator/Demodulator**: Signal processing
3. **Protocol Handler**: Packet framing, error correction
4. **Data Manager**: Queue management for uplink/downlink

### Communication Flow
```
Uplink:   Ground → RF → HackRF → Demod → Protocol → C&DH
Downlink: C&DH → Protocol → Mod → HackRF → RF → Ground
```

## 🚀 Development Status

- [ ] HackRF driver integration
- [ ] Basic modulation (FSK/BPSK)
- [ ] Protocol implementation
- [ ] Error correction (Reed-Solomon, etc.)
- [ ] Integration with C&DH
- [ ] Link budget analysis
- [ ] Ground station synchronization

## 🔧 Software Stack

### Libraries
- **libhackrf**: HackRF API
- **GNU Radio**: Signal processing (optional)
- **Liquid DSP**: Modulation library
- **Custom**: Protocol implementation

### Installation
```bash
# Install HackRF tools
sudo apt install hackrf libhackrf-dev

# Install signal processing libraries
sudo apt install gnuradio liquid-dsp

# Build comms software
cd hackrf
mkdir build && cd build
cmake ..
make
```

## 📡 Communication Parameters

*To be defined based on link budget analysis*


## 🧪 Testing

```bash
TBD
```

## 📊 Performance Requirements

*To be defined*

## 🔒 Protocol Features

(*To be defined*)
- Packet framing with sync words
- CRC for error detection
- Forward error correction
- Automatic repeat request (ARQ)
- Link quality monitoring

## 🤝 Contributing

When contributing to Comms:
- Test with RF dummy loads
- Document modulation schemes used
- Consider link budget implications
- Ensure protocol is well-documented

## 📖 Related Documentation

- [Link Budget Analysis](../../docs/architecture/link-budget.md)
- [Communication Protocol](../../docs/api/comms-protocol.md)
- [Ground Station Setup](../../ground-segment/station/)
