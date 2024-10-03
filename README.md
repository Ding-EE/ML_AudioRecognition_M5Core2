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

## Microphone Configuration for M5Core2

To correctly configure the microphone on the M5Core2, set up the I2S (Inter-IC Sound) interface for processing audio input.

### I2S Pin Configuration

- **I2S Serial Clock (BCK) Pin**: `GPIO 12`
- **I2S Left/Right Clock (LRCK) Pin**: `GPIO 0`
- **I2S Data Out Pin**: `GPIO 2`
- **I2S Data In Pin (Microphone input)**: `GPIO 34`

### Initialization

The I2S interface is initialized in the `InitI2SSpeakOrMic` function, set to microphone input (PDM mode). Configuration includes:

- **Mode**: Master, RX (receive), PDM
- **Sample Rate**: `EI_CLASSIFIER_FREQUENCY` (set by the Edge Impulse model)
- **Bits per Sample**: 16-bit
- **Channel Format**: Mono
- **DMA Buffer Settings**: Configured for efficient data transfer

### Functionality

- **Data Capture**: Microphone data is captured in chunks and stored in a buffer for processing.
- **Signal Processing**: The audio signal is prepared for inferencing by converting it to the necessary format for the Edge Impulse classifier.

## Obtaining the Code from Edge Impulse

To integrate Edge Impulse with the M5Core2 for audio recognition, follow these steps:

1. **Sign Up or Log In**:
   - Go to [Edge Impulse](https://www.edgeimpulse.com/) and create an account or log in.

2. **Create a New Project**:
   - Click on "Create new project" and name your project.

3. **Data Collection**:
   - Use the Edge Impulse Studio to collect audio samples or upload existing data.

4. **Design Your Impulse**:
   - In the "Impulse Design" tab, configure the processing blocks and learning blocks for your audio data.

5. **Train Your Model**:
   - Navigate to the "Model" tab and train your model with the collected data.

6. **Deployment**:
   - Once training is complete, go to the "Deployment" tab.
   - Select "ESP32" and download the firmware or library.

7. **Integrate with Your Project**:
   - Add the downloaded Edge Impulse library to your PlatformIO project.
   - Include the necessary headers and functions in your code to use the trained model.

These steps will help you set up the Edge Impulse model for use with your M5Core2 device.
