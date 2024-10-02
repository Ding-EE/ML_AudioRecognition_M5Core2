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

 ## Microphone Configuration for M5Core2

To correctly configure the microphone on the M5Core2, you need to set up the I2S (Inter-IC Sound) interface. This setup allows the device to process audio input for recognition tasks.

### I2S Pin Configuration

The M5Core2 uses specific pins for I2S communication. Here's the configuration used in this project:

- **I2S Serial Clock (BCK) Pin**: `GPIO 12`
- **I2S Left/Right Clock (LRCK) Pin**: `GPIO 0`
- **I2S Data Out Pin**: `GPIO 2`
- **I2S Data In Pin (Microphone input)**: `GPIO 34`

### Initialization

The I2S interface is initialized in the `InitI2SSpeakOrMic` function, where the mode is set to microphone input (PDM mode). The configuration includes:

- **Mode**: Master, RX (receive), PDM (Pulse Density Modulation)
- **Sample Rate**: `EI_CLASSIFIER_FREQUENCY` (set by the Edge Impulse model)
- **Bits per Sample**: 16-bit
- **Channel Format**: Only left channel (mono)
- **DMA Buffer Settings**: Configured for efficient data transfer

### Functionality

- **Data Capture**: The microphone data is captured in chunks and stored in a buffer for processing.
- **Signal Processing**: The captured audio signal is prepared for inferencing by converting it to the necessary format for the Edge Impulse classifier.

This setup ensures that the M5Core2 can effectively capture and process audio data for speech recognition tasks.

## Setup

1. **Wi-Fi Credentials**: Update the `ssid` and `password` variables with your Wi-Fi network details.

2. **Blynk Configuration**:
   - Replace `BLYNK_TEMPLATE_ID`, `BLYNK_TEMPLATE_NAME`, and `BLYNK_AUTH_TOKEN` with your Blynk project credentials.

3. **I2S Pin Configuration**:
   - Ensure the I2S pins are correctly configured for your M5Core2 hardware setup.

## Installation

1. **Clone the Repository**:
   ```bash
   https://github.com/Ding-EE/Audio_Recognition_by_Machine_LearningM5Core2
