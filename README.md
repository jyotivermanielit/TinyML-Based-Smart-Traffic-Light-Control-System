# Smart Adaptive Traffic Control using TinyML & Drone IoT Surveillance

## Overview
This project presents a Smart Adaptive Traffic Control System that combines TinyML, IoT, and drone-based surveillance to optimize traffic signal timing in real time. A drone-mounted ESP32-CAM captures aerial traffic images and performs on-device traffic density classification using a TinyML model. The classified traffic density is transmitted wirelessly to an ESP32 DevKit, which dynamically adjusts traffic signal timings based on road congestion levels.

## System Architecture
<img width="1350" height="892" alt="image" src="https://github.com/user-attachments/assets/a400a3b5-db64-4c32-8192-5eddff6433f0" />

## Working
ESP32-CAM Capture
↓
Image Preprocessing
↓
TinyML Inference
↓
Traffic Density Classification
↓
Adaptive Signal Timing 

### Future Enhancement: Drone-Based Surveillance
As a future enhancement, the ESP32-CAM can be mounted on a drone to provide aerial traffic monitoring. In this setup, the classified traffic density can be transmitted wirelessly to an ESP32 DevKit located at the traffic junction. The ESP32 DevKit can then control traffic signals based on real-time traffic conditions captured from the drone, enabling wider coverage and smart city deployment.

## Hardware Used
- ESP32-CAM
- ESP32 DevKit
- Drone Platform
- Traffic Signal LEDs
- Power Supply Module

## Software Used
- Edge Impulse
- Arduino IDE

## Features
- Real-time traffic density monitoring
- Drone-based aerial surveillance
- TinyML inference on ESP32-CAM
- Wireless communication between nodes
- Adaptive traffic signal control
- Low-cost IoT implementation

## Results
- Real-time traffic density classification
- Dynamic green signal timing
- Improved traffic flow management
- Reduced waiting time at intersections

## Future Scope
- Multi-junction traffic monitoring using multiple drones
- Emergency vehicle detection and priority signaling
- Accident detection and automatic alerts
- Integration with smart city infrastructure
- AI-based traffic prediction and route optimization
- Cloud dashboard for real-time traffic analytics
- Automatic drone patrol scheduling for large urban areas

## Applications
- Smart Cities
- Traffic Management Systems
- Highway Monitoring
- Emergency Response Systems
- Urban Mobility Solutions
