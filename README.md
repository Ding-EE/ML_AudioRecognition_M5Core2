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

## Installation

1. **Clone the Repository**:
   ```bash
   git clone <repository-url>
   cd <repository-directory>

## Troubleshooting

- **Upload Issues**: If the code fails to upload, use the UIFLOW Firmware Burning Tool. Download it from [M5Stack Documentation](https://docs.m5stack.com/en/download).

## Usage

- **Start**: Upon powering the device, it initializes the display, connects to Wi-Fi, and starts audio recognition.
- **Inferencing**: The device listens for audio and displays recognized words on the screen.
- **Blynk Integration**: Based on recognized commands (e.g., "go" or "stop"), it sends updates to the Blynk app.

## Code Overview

- **setup()**: Initializes the device, display, and I2S for microphone input.
- **loop()**: Continuously records audio, processes inferencing, and updates the display and Blynk app.
- **displayWaveform()**: Renders the audio waveform on the display.
- **microphone_inference_start()**: Prepares the buffer for audio sampling.
- **microphone_inference_record()**: Captures audio data for inferencing.

## Contact

For any questions or support, please contact Gabriel at [dinggabriel2@gmail.com].
