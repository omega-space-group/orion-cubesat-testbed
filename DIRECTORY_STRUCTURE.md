# ORION CubeSat Testbed - Directory Structure

Complete directory tree and organization.

## Root Level

```
orion-cubesat-testbed/
├── README.md                    # Main project README
├── LICENSE                      # GPL-3.0 License
├── CONTRIBUTING.md              # Contribution guidelines
├── CODE_OF_CONDUCT.md           # Community guidelines
├── DIRECTORY_STRUCTURE.md       # This file
├── .gitignore                   # Git ignore rules (ROS template)
│
├── flight-software/             # All on-board software
│   ├── cdh/                     # Command & Data Handling
│   │   ├── rpi4/                # Raspberry Pi 4 prototype
│   │   └── stm32/               # STM32 production
│   ├── eps/                     # Electrical Power System
│   │   └── stm32/               # STM32 implementation
│   ├── payload/                 # AI Payload
│   │   ├── jetson/              # NVIDIA Jetson
│   │   └── fpga/                # Xilinx FPGA
│   └── comms/                   # Communications
│       └── hackrf/              # HackRF One
│
├── ground-segment/              # Ground station software
│   ├── station/                 # Ground station
│   └── mission-control/         # Mission operations
│
├── middleware/                  # Space ROS & integration
│   ├── spaceros/                # Space ROS configs
│   └── interfaces/              # Interface definitions
│
├── hardware/                    # Hardware docs & configs
│   ├── bus-configs/             # CAN & GigE configs
│   │   ├── can/                 # CAN configuration
│   │   └── gige/                # Gigabit Ethernet
│   ├── schematics/              # Hardware schematics
│   └── bom/                     # Bill of Materials
│
├── simulation/                  # Testing infrastructure
│   ├── unit-tests/              # Module tests
│   ├── integration-tests/       # Subsystem integration
│   └── scenarios/               # Mission scenarios
│
├── tools/                       # Shared utilities
│   ├── build-scripts/           # Build automation
│   └── deployment/              # Deployment tools
│
├── scripts/                     # Quick setup scripts
│   ├── setup.sh                 # Initial setup
│   ├── build.sh                 # Build script
│   └── test.sh                  # Test script
│
├── research/                    # Research documentation
│   └── (Publications, results, experiments)
│
└── docs/                        # Project documentation
    ├── architecture/            # System architecture
    ├── getting-started/         # Setup guides
    └── tutorials/               # Learning materials
```

## Subsystem Organization

Each subsystem follows a consistent structure:

```
subsystem/
├── README.md           # Subsystem documentation
├── src/                # Source code
├── include/            # Header files
├── config/             # Configuration files
├── tests/              # Unit tests
└── docs/               # Subsystem-specific docs
```

## Key Files by Purpose

### Getting Started
- `README.md` - Project overview and quick start
- `docs/getting-started/` - Detailed setup guides
- `CONTRIBUTING.md` - How to contribute

### Development
- `scripts/build.sh` - Build all components
- `tools/build-scripts/` - Build automation
- `simulation/` - Testing infrastructure

### Documentation
- `docs/architecture/` - System design
- `docs/api/` - API reference
- `docs/tutorials/` - Learning materials

### Hardware
- `hardware/bus-configs/` - Communication protocols
- `hardware/schematics/` - System diagrams
- `hardware/bom/` - Parts list

## Navigation Tips

- Each directory has its own README.md explaining its purpose
- Start with the main README.md for project overview
- Check docs/getting-started/ for setup instructions
- Browse docs/tutorials/ for learning materials
- See CONTRIBUTING.md before making changes

