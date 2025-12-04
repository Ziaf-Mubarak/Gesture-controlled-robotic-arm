# Gesture-controlled-robotic-arm
# Gesture Controlled Robotic Arm using Dual ESP32 and ESP-NOW

![Platform](https://img.shields.io/badge/Platform-ESP32-blue?logo=espressif)
![Language](https://img.shields.io/badge/Language-C++-blue)
![Sensor](https://img.shields.io/badge/Sensor-MPU6050-orange)
![Wireless](https://img.shields.io/badge/Protocol-ESP--NOW-green)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

<p align="center">
  <img src="./arm.jpg" alt="Gesture controlled robotic arm" width="420">
</p>

---

## Gesture Controlled Robotic Arm using Dual ESP32 and ESP-NOW

This project implements a *wireless gesture-controlled robotic arm* using:

- An *ESP32-based glove* with an MPU6050 IMU and a flex sensor  
- An *ESP32-based robotic arm controller* driving 5 servo motors  
- *ESP-NOW* for low-latency communication between glove and arm  

Hand orientation (pitch & roll) and a finger bend (flex) are mapped to the arm’s base, shoulder, elbow, wrist, and gripper motion. The arm moves in *smooth steps* instead of jumping directly to raw values, giving more stable and natural motion.

---

## ✨ Features

- Dual ESP32 nodes communicating over *ESP-NOW*
- *MPU6050* for gesture sensing (pitch and roll)
- *Flex sensor* for gripper control (open / close)
- *5-DOF robotic arm*
  - Base  
  - Shoulder  
  - Elbow  
  - Wrist  
  - Gripper  
- Step-wise servo motion for smooth, jitter-free behaviour
- Clean separation of:
  - glove_transmitter.ino (glove side)
  - arm_receiver.ino (robotic arm side)

---

## 🧰 Hardware Used

### Glove (Transmitter)

- ESP32 development board  
- MPU6050 6-axis accelerometer + gyroscope  
- Flex sensor (finger bend for gripper)  
- Resistors for flex sensor voltage divider  
- Power source (5 V USB or battery pack)  
- Wires and a glove

### Robotic Arm (Receiver)

- ESP32 development board  
- 3 × MG996R (or similar high-torque servos) for base, shoulder, elbow  
- 2 × SG90 (or similar micro servos) for wrist and gripper  
- External 5 V supply for servos (with *common ground* to ESP32)  
- Robotic arm structure (3D printed or kit)  
- Breadboard / PCB and wiring  

Some reference images are included in this repo:

- arm.jpg – final arm prototype  
- glove.jpg – gesture glove  
- 3d model.jpg – 3D model render  
- block diagram.jpg – system block diagram  
- circuit diagram.jpg – circuit / wiring

---

## 🪛 Wiring Overview

### Glove Side (Transmitter ESP32)

*MPU6050*

- VCC → 3.3 V  
- GND → GND  
- SCL → GPIO 22 (default ESP32 SCL)  
- SDA → GPIO 21 (default ESP32 SDA)  

*Flex Sensor (voltage divider)*

- One end → 3.3 V  
- Other end → series resistor → GND  
- Junction of flex + resistor → GPIO 34 (ADC input)  

### Arm Side (Receiver ESP32)

*Servos* (with external 5 V supply):

- BASE_SERVO_PIN → GPIO 13  
- SHOULDER_SERVO_PIN → GPIO 12  
- ELBOW_SERVO_PIN → GPIO 14  
- WRIST_SERVO_PIN → GPIO 27  
- GRIPPER_SERVO_PIN → GPIO 26  

> ⚠ *Important*  
> - Servo *V+* → 5 V external supply  
> - Servo *GND* → external supply GND *and* ESP32 GND must be common  

---

## 📂 Project Files

- glove_transmitter.ino – ESP32 glove code  
- arm_receiver.ino – ESP32 arm controller code  
- arm.jpg, glove.jpg – photos of the hardware  
- 3d model.jpg, block diagram.jpg, circuit diagram.jpg – documentation images  
- LICENSE – MIT license  

You can later move code into a src/ folder and images into docs/ if you want an even more structured layout.

---

## 🧩 Software Setup

You can use the *Arduino IDE* with ESP32 support.

1. Install *ESP32 board package* in Arduino IDE.  
2. Install required libraries:
   - ESP32Servo
   - Adafruit_MPU6050
   - Adafruit Unified Sensor
3. Clone this repository:

```bash
git clone https://github.com/Ziaf-Mubarak/Gesture-controlled-robotic-arm.git
cd Gesture-controlled-robotic-arm
