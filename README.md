# Automatic-Traffic-Signal-ESP32
An IoT-based Smart Traffic Management System using ESP32, ultrasonic sensors, and Blynk that dynamically controls traffic signals based on vehicle density, with real-time monitoring and emergency override capabilities.


# IoT-Based Smart Traffic Light Controller

This ESP32 program implements an **IoT-based Smart Traffic Light Controller** using:

- **ESP32** as the main controller
- **2 Ultrasonic Sensors** to estimate vehicle density in two traffic lanes
- **Blynk IoT Platform** for real-time monitoring and remote control
- **Traffic LEDs** representing the traffic signals for both lanes

## Features

- Counts the number of vehicles detected in each lane.
- Compares the traffic density of both lanes.
- Automatically gives the **green signal** to the lane with higher vehicle density while keeping the other lane at **red**.
- Displays vehicle counts and traffic light status on the **Blynk mobile application**.
- Supports **emergency vehicle override**, allowing either lane to be manually given priority through the Blynk app.
- Provides a **remote reset** option to clear both vehicle counters from the app.
- Continuously updates traffic status every second.

## Hardware Used

- ESP32 Development Board
- HC-SR04 Ultrasonic Sensors (×2)
- LEDs (Red, Yellow, Green) for two traffic lanes
- Wi-Fi Network
- Blynk IoT Platform

## Working Principle

1. The ESP32 continuously reads the distance from both ultrasonic sensors.
2. A vehicle is counted whenever it is detected within the predefined threshold distance.
3. The vehicle counts for both lanes are compared.
4. The lane with the higher traffic density receives the **green signal**, while the other lane remains **red**.
5. Vehicle counts and traffic light states are transmitted to the Blynk dashboard for real-time monitoring.
6. Emergency override buttons on the Blynk app can temporarily prioritize either lane.
7. A reset button on the dashboard clears both vehicle counters when required.

## Technologies Used

- ESP32
- Arduino Framework
- Blynk IoT
- Wi-Fi Communication
- Ultrasonic Distance Sensing


PS : There are only two ultrasound sensors in the code, due to hardware limitations during the implementation. For a typical 4 lane traffic control module, more ultrasonic sensors can be added by just altering the code.
