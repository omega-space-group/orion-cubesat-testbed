# Payload - Xilinx FPGA

Hardware-accelerated processing using Xilinx FPGA.

## Hardware

- **FPGA**: Xilinx (specific model TBD)
- **Development Board**: TBD
- **Interfaces**: PCIe/Ethernet to Jetson

## Tools

- **Vivado**: HDL design and synthesis
- **Vitis/Vitis-AI**: High-level synthesis and acceleration
- **Simulator**: ModelSim or Vivado simulator

## Directory Structure

```
fpga/
├── hdl/           # Verilog/VHDL source
├── ip/            # Custom IP cores
├── constraints/   # XDC constraint files
├── sim/           # Testbenches
└── scripts/       # Build automation
```

## Development Workflow

```bash
TBD
```


## Interface to Jetson

- High-speed data transfer via PCIe or 10GigE
- Control via memory-mapped registers
- DMA for efficient data movement

## Simulation

```bash
TBD
```

## Timing Constraints

- Define clock constraints in `constraints/timing.xdc`
- Verify timing closure after implementation
- Optimize critical paths

## Development Status

- [ ] Setup development environment
- [ ] Design basic processing pipeline
- [ ] Test AI deployment
- [ ] Setup Payload Subsystem Interface to C&DH
