## 🚧 Project Overview

STM32-Safety-Net is a safety system designed for construction workers, utilizing STM32 microcontrollers, ultrasonic sensors, buzzers, and LEDs to detect nearby objects and alert workers of potential hazards. The system consists of two STM32 devices:

**Detection Unit**: Uses an ultrasonic sensor to sense nearby objects.

**Alert Unit**: Receives signals from the Detection Unit via Bluetooth and activates a buzzer and LED warning system.

## 🛠 Features

- **Object Detection**: Uses an HC-SR04 ultrasonic sensor to detect objects within a predefined distance.

- **Wireless Communication**: Utilizes Bluetooth (HC-05/HC-06) to send alerts between STM32 units.

- **Audible and Visual Alerts**: Activates a buzzer and flashing LED when an obstacle is detected.

- **Low Power Consumption**: Optimized for battery operation in the field.

## ⚙️ Hardware Requirements

- 2x STM32 Development Boards (e.g., STM32F4 or STM32F1 series)

- 1x HC-SR04 Ultrasonic Sensor

- 1x HC-05/HC-06 Bluetooth Module

- 1x Buzzer

- 1x LED + Resistor

- Jumper Wires & Breadboard (for prototyping)

## 🔧 Setup Instructions

1️⃣ Install Required Software

- VS Code with C/C++ and Cortex-Debug extensions

- STM32CubeIDE (optional, for code generation)

-ARM GCC Toolchain (Download)

OpenOCD for debugging

2️⃣ Clone the Repository

git clone https://github.com/yourusername/STM32-Safety-Net.git
cd STM32-Safety-Net

3️⃣ Build the Project

make

4️⃣ Flash to STM32 Board

Use OpenOCD or STM32CubeProgrammer to flash the firmware:

openocd -f board/stm32f4discovery.cfg

5️⃣ Run & Debug

Set up Cortex-Debug in VS Code and start debugging.

## 📡 Wireless Communication Protocol

The Detection Unit sends a Bluetooth signal (ALERT) when an object is detected.

The Alert Unit listens for this signal and activates the buzzer/LED upon reception.

## 🎯 Future Enhancements

Add LoRa for long-range communication.

Integrate machine learning for smarter hazard detection.

Implement a mobile app for real-time alerts.
