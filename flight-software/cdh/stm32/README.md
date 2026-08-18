# Orion CubeSat Testbed: CDH Flight Software
**Google Summer of Code 2026 Work Product Submission**

**Student:** Ada Roufogali  
**Organization:** Orion Space Group | National Technical University of Athens  
**Project:** Message-Based Flight Software Architecture for CubeSats  
**Mentors:** Simon Vellas 

---

## 🚀 Technical Overview

This repository contains the Command and Data Handling (CDH) flight software for the Orion CubeSat Testbed, developed specifically for the STM32 Development Board (Nucleo-H755ZI-Q). The system is built on **FreeRTOS v10.3.1** (CMSIS-RTOS V2 wrapper) and targets the **STM32H755ZI Dual-Core MCU** (Cortex-M7 & Cortex-M4 running at 64 MHz).

### Core Architecture: Publish/Subscribe Software Bus
At the heart of the flight software is a lightweight, message-based **Publish/Subscribe (Pub/Sub) Software Bus**, heavily inspired by NASA’s Core Flight System (cFS). This architecture guarantees decoupled, highly modular communication between critical software components such as telemetry, power management, and ADCS.

**Key Technical Highlights:**
*   **Static Allocation:** Every task is instantiated using statically allocated memory, pairing a thread with its own local queue.
*   **Bus Dispatching:** Publishers push messages to a central Master Queue. A high-priority Dispatcher Task then references a static Subscription Table and copies messages into the local queues of subscribed tasks.
*   **Event Groups & Synchronization:** Implemented native FreeRTOS Event Groups acting as Thread Sync Barriers (`TaskSync_SetAndWait`) and comprehensive Health Checks (`TaskHealth_SetBit`), ensuring safe boot sequences and runtime stability.
*   **Dual-Core Execution:** The boot sequence is orchestrated by the primary Cortex-M7 core, which configures system clocks and peripherals before waking the Cortex-M4. Debugging relies on independent GDB servers synchronized via the Cross-Trigger Interface (CTI).

---

## 📖 Developer Manuals & API Reference

Comprehensive [documentation](https://omega-space-group.github.io/orion-cubesat-testbed/flight-software/cdh/stm32/CDH_Software_Dev_Board/doc/html/index.html) for the CDH subsystem has been generated via Doxygen and deployed to GitHub Pages. Here is the documentation for the core framework and setup:

*   **[Software Bus Architecture](https://omega-space-group.github.io/orion-cubesat-testbed/flight-software/cdh/stm32/CDH_Software_Dev_Board/doc/html/d8/ded/software_bus.html)**
    *   Details the Pub/Sub model, Master Queue vs. Local Queue routing, and message dispatching timelines.
*   **[API References](https://omega-space-group.github.io/orion-cubesat-testbed/flight-software/cdh/stm32/CDH_Software_Dev_Board/doc/html/d1/d35/api_references.html)**
    *   Complete API specifications including `Subscribe()`, `Publish()`, `SleepUntil()`, and the Task Health/Sync macros.
*   **[Task Creation Guide](https://omega-space-group.github.io/orion-cubesat-testbed/flight-software/cdh/stm32/CDH_Software_Dev_Board/doc/html/de/d28/task_creation.html)**
    *   Architectural templates for initializing periodic and non-periodic (event-driven) tasks using FreeRTOS static allocation.
*   **[Flash and Debugging Guide](https://omega-space-group.github.io/orion-cubesat-testbed/flight-software/cdh/stm32/CDH_Software_Dev_Board/doc/html/d4/d87/flash_and_debug.html)**
    *   Configuration steps for compiling, single-click dual-core flashing, and establishing CTI synchronization in STM32CubeIDE. Includes vital workarounds for internal watchdog (IWDG1) auto-generation issues.

---

## 💻 Code Contributions

All developed code has been reviewed and merged into the dev organization repository. Below is a direct mapping of the changes introduced during this period:

*   **Target Repository:** https://omega-space-group.github.io/orion-cubesat-testbed/
*   **Commit History:** https://github.com/omega-space-group/orion-cubesat-testbed/commits/gsoc?author=adaroufogali

---

## ⭐ Flight Software Core

This project serves as the **heart of the flight software**. It is designed with modularity in mind: you can easily port this architecture by dropping the `/App` directory into your own STM32 project. Doing so provides you with an instant, robust, message-based flight software foundation, allowing you to focus entirely on building your custom application tasks.

### Core Capabilities

*   **Software Bus Routing:** Fully operational. Messages are reliably dispatched from publishers to multiple subscribers without deadlocks. The subscription table is highly adaptable and can be easily customized to fit the specific needs of your application.
*   **Task Management:** Successfully integrates standardized templates for both periodic and message-driven (non-periodic) tasks. This streamlines the development of new custom tasks tailored to your specific mission requirements.

### Implemented Tasks

The following core tasks have been fully implemented and can be used directly. *Note: Detailed explanations of their functionality can be found in the user manual.*

| Task File | Description | Status |
| :--- | :--- | :--- |
| `can_rx_task.c` | Parses FDCAN messages received over the CAN bus. | **TESTED** |
| `dispatcher_task.c` | Handles the dispatcher and master queue logic. | **TESTED** |
| `healthMonitor_task.c` | Performs background execution checks and task accounting. | **TESTED** |
| `modeManager_task.c` | Manages the System State/Mode Finite State Machine (FSM). | **TESTED** |
| `root_task.c` | Starts all peripherals and spawns (initializes) every other task. | **TESTED** |
| `subsystemControl_task.c` | Manages subsystem control logic. | **TESTED** |
| `subsystemMonitoring_task.c`| Handles subsystem monitoring logic. | **TESTED** |
| `telecommandHandler_task.c` | Parses and handles Telecommands received from the ground station. | **TESTED** |
| `housekeeping_task.c` | Acquires telemetry data and manages logging/downlinking. | *NOT TESTED* |
| `iwdg_task.c` | Handles the internal watchdog (IWDG). | *NOT TESTED* |

### Shared Services

Services are centralized utility functions that tasks share to perform specific operations efficiently:

*   **`app_events.c`**: Implements all application-level events necessary for event triggering and task synchronization.
*   **`subscriptions.c`**: Implements the core Publisher/Subscriber message routing logic. *For a comprehensive breakdown of how this routing system works, please refer to our [API References](https://omega-space-group.github.io/orion-cubesat-testbed/flight-software/cdh/stm32/CDH_Software_Dev_Board/doc/html/d1/d35/api_references.html).*

---

## 🚀 Future Work
*   **Zero-Copy Optimization:** Transition the Dispatcher's message delivery mechanism to a zero-copy pointer-passing approach to save MCU cycles and memory overhead.
*   **Error Handling:** Implement robust bus error handlers (e.g., checking for saturated local queues, crash recovery during push/pop operations).
*   **Watchdog Integration:** Finalize hardware timing specifications to safely re-enable the MCU internal watchdog (`IWDG1`) without interrupting standard testing loops.
