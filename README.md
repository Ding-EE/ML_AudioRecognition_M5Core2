# Audio Recognition with M5Core2

This project demonstrates audio recognition using the M5Stack Core2 device. It leverages Edge Impulse for inferencing and Blynk for IoT connectivity.

## Features

- **Audio Recognition**: Uses Edge Impulse's inferencing library for speech recognition.
- **IoT Connectivity**: Integrates with Blynk to control and monitor from a mobile app.
- **Visual Feedback**: Displays audio waveform and recognition results on the M5Core2 screen.

## Requirements

- **Hardware**: M5Stack Core2
- **Libraries**:
  - Edge Impulse inferencing library
  - M5Core2 library
  - Blynk library for ESP32
  - ESP32 I2S driver

## Setup

1. **Wi-Fi Credentials**: Update the `ssid` and `password` variables with your Wi-Fi network details.

2. **Blynk Configuration**:
   - Replace `BLYNK_TEMPLATE_ID`, `BLYNK_TEMPLATE_NAME`, and `BLYNK_AUTH_TOKEN` with your Blynk project credentials.

3. **I2S Pin Configuration**:
   - Ensure the I2S pins are correctly configured for your M5Core2 hardware setup.

## Installation

1. **Clone the Repository**:
   ```bash
   <https://github.com/Ding-EE/Audio_Recognition_by_Machine_LearningM5Core2>
