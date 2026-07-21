@page flash_and_debug Flash and Debug the STM32H7 Dual Core MCU

## Debug Configurations - Initial FLashing
### Instructions
This manual follows the steps as described in the following ST forum:
https://community.st.com/stm32-mcus-60/how-to-create-and-run-an-stm32h7-dual-core-basic-example-160437

### How Flashing Works
Flashing a dual-core STM32H7 requires two distinct binaries (one for the Cortex-M7 and one for the Cortex-M4).

* The Master Project: In the IDE, the CM4 project is added to the startup setup of the CM7 project.

* The Single-Click Flash: When you flash or hit run on the CM7 project, the compiler automatically handles everything—it flashes the CM7 binary first, followed immediately by the CM4 binary into their respective memory regions.

* Debugging Setup: Each core runs an independent GDB debugger server instance utilizing separated port numbers (differing by at least 4, e.g., port 61234 and 61238).

### Sequence of Events (Boot & Run)
* System Power-On / Reset: 
The primary core (Cortex-M7) boots up first. It is entirely responsible for setting up fundamental system resources, the system clock configuration, and initializing peripheral components.

* Secondary Core Wakeup: 
While the M7 initializes the board, the Cortex-M4 core is kept on hold in reset or a low-power hardware stop mode. Once the M7 finishes basic system setup, it safely releases the M4 core via software control.

* Cross-Trigger Synchronization (CTI): 
Both debuggers use the Cross-Trigger Interface (CTI). If an error occurs or you hit a breakpoint, one core can instantly signal and halt the other core to prevent synchronization mismatches.

* Execution Parallelism: 
Once fully synchronized, the M7 and M4 process their logic simultaneously in parallel (demonstrated by toggling their respective hardware pins within roughly ~12 to 20 microseconds of one another).

For more information checkout ST's application notes:
* https://www.st.com/resource/en/application_note/dm00629855-getting-started-with-projects-based-on-dualcore-stm32h7-microcontrollers-in-stm32cubeide-stmicroelectronics.pdf
* https://www.st.com/content/ccc/resource/technical/document/application_note/group1/96/bf/35/36/e6/97/42/2e/DM00597308/files/DM00597308.pdf/jcr:content/translations/en.DM00597308.pdf

### How "Debug Configurations" should look:
@image html M4_debug.png "Cortex-M4 Debug Tab" width=1000px

---

@image html M4_startup.png "Cortex-M4 Startup Tab" width=1000px

---

@image html M7_debug.png "Cortex-M7 Debug Tab" width=1000px

---

@image html M7_startup.png "Cortex-M7 Startup Tab" width=1000px

---

@note Make sure main.c includes the following auto-generated boot code for both cores:
@image html M4_main.png "Cortex-M4 Boot Code" width=1000px
@image html M7_main.png "Cortex-M7 Boot Code" width=1000px

---

## Auto-generated Code and Re-flashing
@warning **Important Warning:** Modifying the `.ioc` file generates fresh code but safely preserves anything inside the `USER CODE BEGIN / END` sections. 
However, the `MX_IWDG1_Init()` function call is generated *outside* of these user blocks. 

### Watchdog Configuration Warning
* The MCU's internal watchdog (**IWDG1**) is enabled in the `.ioc` configuration for testing. Because the software is still in development and system timings are not finalized, the watchdog needs to remain disabled during standard testing to prevent unwanted resets.
* **The Current Workaround:** The `MX_IWDG1_Init()` function call is manually commented out in `main.c`.
* **The Risk:** Every single time you generate code from CubeMX after making a change to the `.ioc` file, it will automatically **uncomment** `MX_IWDG1_Init()` and re-enable the watchdog.
* **Required Action:** Whenever you update or change the `.ioc` configuration, you **must** go back into `main.c` and manually comment out `MX_IWDG1_Init()` again for the software to run correctly.