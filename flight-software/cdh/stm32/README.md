# Google Summer of Code 2026 Work Product Submission

**Student:** [Your Name]  
**Organization:** [Omega Space Group / Organization Name]  
**Project:** [Exact Project Title, e.g., Flight Software Development for Orion CubeSat CDH]  
**Mentors:** [Mentor 1 Name], [Mentor 2 Name]  

## 🚀 Project Overview

The objective of this project was to develop, optimize, and document the Command and Data Handling (CDH) flight software for the Orion CubeSat Testbed, specifically targeting the STM32 Development Board. 

Over the course of GSoC, my work focused on establishing a robust software architecture, implementing the software bus, handling task creation, and ensuring the system is thoroughly documented for future contributors.

## 📖 Documentation & Manuals

A major component of my deliverables was the creation of a comprehensive developer manual. This documentation serves as the foundation for the CDH Software Development Board and includes detailed guides on system architecture and API usage. 

You can explore the generated manuals here:

*   **[Software Bus Architecture](https://omega-space-group.github.io/orion-cubesat-testbed/flight-software/cdh/stm32/CDH_Software_Dev_Board/doc/html/d8/ded/software_bus.html)**: Details the message-passing system and how different software components communicate within the CDH module.
*   **[API References](https://omega-space-group.github.io/orion-cubesat-testbed/flight-software/cdh/stm32/CDH_Software_Dev_Board/doc/html/d1/d35/api_references.html)**: Complete documentation of the functions, structures, and interfaces developed for the flight software.
*   **[Task Creation Guide](https://omega-space-group.github.io/orion-cubesat-testbed/flight-software/cdh/stm32/CDH_Software_Dev_Board/doc/html/de/d28/task_creation.html)**: Instructions and standards for defining, spawning, and managing RTOS tasks on the STM32 board.
*   **[Flash and Debugging](https://omega-space-group.github.io/orion-cubesat-testbed/flight-software/cdh/stm32/CDH_Software_Dev_Board/doc/html/d4/d87/flash_and_debug.html)**: A step-by-step guide for flashing the compiled firmware onto the hardware and setting up the debugging environment.

## 💻 Code Contributions

*All code written during this period was merged into the main organization repository. Below is a summary of my contributions:*

*   **Primary Repository:** [Link to the main repo, e.g., omega-space-group/orion-cubesat-testbed]
*   **My Commits:** [Link to a filtered view of your commits, e.g., GitHub commit search `author:yourusername`]

### Key Pull Requests
1.  **[Title of PR 1](#)** - *Merged* - Implemented the core Software Bus mechanics.
2.  **[Title of PR 2](#)** - *Merged* - Refactored Task Creation and integrated RTOS standards.
3.  **[Title of PR 3](#)** - *Open/Under Review* - Added complete Doxygen documentation pipeline.
*(Add or remove PRs as necessary, keeping descriptions to one brief sentence).*

## 🏁 Current State & What's Left

**What works:**
*   The software bus successfully routes messages between core subsystems.
*   RTOS tasks can be created and scheduled according to the newly documented standards.
*   The API is fully documented, and the Doxygen pipeline generates the manual automatically.
*   Flashing and debugging workflows are verified on the STM32 hardware.

**What's left / Future Work:**
*   [e.g., Implementing specific sensor drivers to communicate over the software bus.]
*   [e.g., Expanding hardware-in-the-loop (HIL) testing.]
*   [e.g., Optimizing power consumption during idle RTOS tasks.]

## 🙏 Acknowledgments

I want to extend a huge thank you to my mentors, [Mentor Names], for their guidance, patience, and expertise throughout the summer. Working on the Orion CubeSat Testbed has been an incredible learning experience in embedded systems and flight software engineering!
