<div align="center">
  
<h3 align="center">Glove Bot</h3>

  <p align="center">
    Wireless robotic hand controlled by a glove with flex sensors.
    <br />
  </p>
</div>

<!-- REMOVE THIS IF YOU DON'T HAVE A DEMO -->
<!-- TIP: You can alternatively directly upload a video up to 100MB by dropping it in while editing the README on GitHub. This displays a video player directly on GitHub instead of making it so that you have to click an image/link -->
<p align="center">
  <a href="https://youtu.be/sKcfIg82iaY">
    <img src="assets/demo.gif" alt="Project Demo">
  </a>
</p>
<p align="center">
  <a href="https://youtu.be/sKcfIg82iaY">Watch Demo Video</a>
</p>


## Table of Contents

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#key-features">Key Features</a></li>
      </ul>
    </li>
    <li><a href="#architecture">Architecture</a></li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#cad-files">CAD Files</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

## About The Project

The Glove Bot project is a robotic hand controlled wirelessly by a glove equipped with flex sensors. The glove, connected to an ESP32, transmits finger bend data to an Arduino Uno, which controls the robotic hand's servo motors. An OLED display provides visual feedback of the hand's current state.

### Key Features

- **Wireless Control:** Uses nRF24L01+ radio modules for wireless communication between the glove and the robotic hand.
- **Flex Sensor Input:** Flex sensors on the glove fingers translate finger movements into data.
- **Servo Motor Control:** Arduino Uno controls five servo motors to mimic the glove's finger movements.
- **OLED Display:** Displays the state of each finger (Open/Closed) for real-time feedback.
- **Emergency Stop:** Includes a button to halt the system and detach the servos.
## Architecture

The system comprises two main parts: the transmitter (glove) and the receiver (robotic hand).

- **Transmitter (Glove):**
  - Flex sensors on each finger measure the degree of bending.
  - An ESP32 microcontroller reads the sensor data and transmits it wirelessly.
  - nRF24L01+ radio module sends the data to the receiver.

- **Receiver (Robotic Hand):**
  - An Arduino Uno receives the data from the transmitter via an nRF24L01+ radio module.
  - The Arduino maps the received data to servo angles and controls five servo motors.
  - An OLED display shows the current state (Open/Closed) of each finger.
  - A button connected to the Arduino allows for emergency stopping of the system.

## Getting Started

### Prerequisites

- Arduino IDE
- ESP32 board
- Arduino Uno board
- nRF24L01+ radio modules (2)
- Flex sensors (5)
- Servo motors (5)
- OLED display (SSD1306)
- Necessary Arduino libraries:
  - SPI.h
  - RF24.h
  - Servo.h
  - Wire.h
  - Adafruit_GFX.h
  - Adafruit_SSD1306.h

  Install libraries via the Arduino IDE Library Manager.

### Installation

1.  **Clone the repository:**
    ```sh
    git clone https://github.com/surjahead/glove-bot.git
    cd glove-bot
    ```

2.  **Open `main.ino` in the Arduino IDE.**

3.  **Install the required libraries:**
    Navigate to Sketch > Include Library > Manage Libraries... and search for and install the following:
    - RF24
    - Adafruit GFX Library
    - Adafruit SSD1306

4.  **Configure the Arduino code:**
    - Ensure the correct board and port are selected in the Arduino IDE (Tools > Board and Tools > Port).
    - Verify that the pin definitions in the code match your hardware setup.

5.  **Upload the code to the Arduino Uno.**

6.  **Set up the transmitter (glove) code on the ESP32 (code not provided in this repository):**
    - Ensure the transmitter code is configured to send data to the same `radioAddress` as the receiver.
    - Connect the flex sensors to the ESP32's analog pins.
    - Connect the nRF24L01+ module to the ESP32.

7.  **Power on both the transmitter and receiver.**

## CAD Files

The `cad/` directory contains STL files for 3D printing the robotic hand components:

-   `Arm.stl`
-   `Arm_Cover.stl`
-   `Finger_Index.stl`
-   `Finger_Middle.stl`
-   `Finger_Pinky.stl`
-   `Finger_Ring.stl`
-   `Finger_Thumb.stl`
-   `Left_Hand.stl`
-   `Right_Hand.stl`

These files can be used to 3D print the mechanical parts of the robotic hand.

## Acknowledgments

- This project was inspired by [this](https://youtu.be/Fvg-v8FPcjg?si=RiB0s6e0aVF1fIAb) video.
