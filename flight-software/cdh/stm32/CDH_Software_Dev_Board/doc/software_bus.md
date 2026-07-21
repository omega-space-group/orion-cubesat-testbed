@page software_bus Software Bus Architecture

## Publisher / Subscriber Model
> **Overview:** A lightweight version of NASA’s cFS implemented on FreeRTOS.

@image html sw_bus_arch.png "" width=700px

---

## Architectural Concept

Each task is created using statically allocated memory. Every task is a pair consisting of a thread and its own local queue. 

### 1. Subscriber Side
When a task boots and runs for the first time, it executes the setup code outside of its infinite loop once. During this step, the task subscribes to all the topics it needs to listen to. 

After setup, it enters the infinite loop. Inside the loop, the task blocks (goes to sleep) until a new message for one of its subscribed topics arrives in its local queue. When a message arrives, the task wakes up, processes it, and loops again. This is how the subscriber side works.

### 2. Publisher Side
When a task needs to send a message (publish a message) about a topic, the message goes through the bus to get delivered to everyone who cares about it. 

The bus is made of two parts: a **dispatcher task** and a **master queue**. When you publish a message, it gets pushed into the master queue. The dispatcher task then checks the subscription table and copies that message into the local queue of every task that subscribed to the topic. 

@note This could be optimized later using a zero-copy method.
@warning **Sidenote:** Right now, this implementation is missing error handlers. It does not check if queues are full or handle crashes when pushing/popping messages.

---

## Architecture Timeline

@image html sw_bus_timeline.png "" width=550px

---


## Example Scenario
@image html real_case_scenario.png "" width=1000px