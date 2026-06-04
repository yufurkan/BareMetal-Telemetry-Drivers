# Real-Time Telemetry and Ground Control System Architecture

## Overview
This project is an end-to-end, HAL-independent bare-metal telemetry network. It bridges low-level hardware register manipulation (I2C, SPI, EXTI, DMA) with a high-level asynchronous desktop application. The system is designed to acquire, transmit, receive, and visualize sensor data with zero reliance on opaque hardware abstraction layers, ensuring deterministic execution and failsafe operation.

## System Topology

The complete architecture consists of three specialized nodes:

### 1. Sensor Node (STM32F446RE)
The data acquisition and transmission hub.
* **Sensor Polling (I2C):** Interfaces with the MPU6050 (6-DOF IMU) using a custom bare-metal I2C driver. Implements DO-178C inspired deterministic timeout counters to prevent `while`-loop deadlocks in case of sensor loss.
* **RF Transmission (SPI):** Data payload is packed and sent to the nRF24L01 2.4GHz transceiver via hardware SPI, entirely controlled through direct register manipulation.

### 2. Receiver Dongle (STM32F103)
The deterministic hardware bridge between the RF link and the PC.
* **Interrupt-Driven Reception (EXTI):** Eliminates CPU polling overhead. The nRF24L01 triggers a hardware interrupt (IRQ) on the STM32F103 immediately upon receiving an RF packet.
* **Direct Memory Access (DMA):** Upon interrupt, the DMA controller autonomously moves the payload from the SPI data registers directly into SRAM without CPU intervention, ensuring zero-latency data handling.
* **PC Interface (UART):** The buffered telemetry frames are streamed to the Ground Control Station via a bare-metal UART configuration.

### 3. Ground Control Station (Qt C++ & TTS)
The asynchronous desktop interface for monitoring and alerting.
* **Asynchronous Parsing (QThread):** Serial data parsing runs on dedicated background threads, ensuring the high-frequency telemetry stream never blocks or freezes the main GUI thread.
* **Event-Driven UI (Signals & Slots):** Extracted IMU states, RF link quality, and system health metrics are routed to the visual dashboards via Qt's native Signals & Slots architecture.


## Tech Stack & Core Concepts
* **Microcontrollers:** ARM Cortex-M4 (STM32F4) & Cortex-M3 (STM32F1)
* **Firmware:** Bare-Metal C/C++ (Memory-Mapped I/O, Interrupt Vectoring, Hardware DMA)
* **Desktop:** Qt 6 / C++ (Multithreading, Serial Communication)
* **Design ** Failsafe integration, zero dynamic memory allocation on MCUs, and register-level hardware mastery.