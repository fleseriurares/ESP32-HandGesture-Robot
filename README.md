# ESP32 Hand Gesture Controlled Robot🤖

This repository contains the project for a hand gesture controlled robot using ESP32. The robot can be controlled using hand gestures detected by an MPU accelerometer sensor and processed by the ESP32 microcontroller.

## Features

- **Hand Gesture Control**: Control the robot using predefined hand gestures.
- **Wireless Communication**: Uses two ESP32 modules for wireless communication and processing.
- **MPU Accelerometer Sensor Integration**: Detects hand gestures using an MPU accelerometer sensor.
- **Motor Control**: Uses a double H-bridge to control 2 DC motors.
- **Server-Client Architecture**: Implements a master-slave architecture for communication and control.

## Components Used

- **2 ESP32 Modules**: Microcontrollers for processing and wireless communication (one as master and one as slave).
- **MPU Accelerometer Sensor**: Used to detect hand gestures.
- **Double H-Bridge**: For controlling the DC motors.
- **2 DC Motors**: For the robot's movement.
- **2 Battery Holders**: To power the ESP32 modules and motors.
- **Robot Chassis**: The physical structure of the robot.

## Software Architecture

The project is designed using a modular architecture to separate concerns and improve maintainability. The main components are:

### 1. Gesture Detection Module
This module is responsible for detecting hand gestures using the MPU accelerometer sensor. It processes the sensor data and identifies the gestures.

### 2. Communication Module
This module handles the wireless communication between the hand gesture detection module (master ESP32) and the robot (slave ESP32). It uses a server-client architecture for communication.

### 3. Control Module
This module receives the gesture commands from the master and controls the robot's movements accordingly. It interfaces with the robot's hardware, including the double H-bridge and DC motors, to execute the commands.
