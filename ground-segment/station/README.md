# Ground Station

RF ground station using HackRF One SDR.

## Hardware

- HackRF One SDR
- Appropriate antenna for frequency band
- LNA (Low Noise Amplifier) for receiving
- Computer with USB 2.0

## Software

- HackRF drivers and tools
- GNU Radio or custom SDR software
- Satellite tracking (optional)

## Setup

```bash
TBD
```

## Operations

- Monitor satellite passes
- Send commands during pass
- Receive telemetry and payload data
- Log all communications

## Configuration

Edit `config/station_config.yaml`:
- Frequency settings
- Modulation parameters
- Antenna pointing
- Data storage paths

## Development Status

- [ ] Setup HackRF hardware
- [ ] Configure RF parameters
- [ ] Test with flatsat
- [ ] Implement tracking
