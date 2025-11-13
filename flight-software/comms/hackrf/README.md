# Comms - HackRF One

Software-defined radio implementation using HackRF One.

## Hardware Setup

- **Device**: HackRF One
- **Connection**: USB 2.0
- **Antenna**: Appropriate for frequency band (UHF/VHF)
- **Amplifier**: LNA for receive, PA for transmit (optional)

## Software Setup

```bash
# Install HackRF tools
sudo apt install hackrf libhackrf-dev

# Test device
hackrf_info

# Verify firmware version
hackrf_info | grep "Firmware Version"

# Update firmware if needed
hackrf_spiflash -w hackrf_one_usb.dfu
```


## GNU Radio Integration

```bash
# Install GNU Radio
sudo apt install gnuradio

# Launch GNU Radio Companion
gnuradio-companion

# Load flowgraph
gnuradio-companion flowgraphs/cubesat_transceiver.grc
```


## Modulation Schemes

Implement in `modulation/`:
- FSK (Frequency Shift Keying)
- BPSK (Binary Phase Shift Keying)
- GMSK (Gaussian Minimum Shift Keying)
- OQPSK (Offset Quadrature PSK)

## Testing

```bash
TBD
```

## Safety

⚠️ **Important RF Safety**:
- Always use dummy load or proper antenna
- Check local regulations for frequency usage
- Ensure proper RF shielding in lab
- Monitor transmit power levels


## Development Status

- [ ] Setup HackRF device
- [ ] Implement basic modulator
- [ ] Test transmission with ground station
- [ ] Develop protocol handler
- [ ] Integrate with C&DH
