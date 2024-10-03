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

## Data Curation

The data curation process is handled using a Python script available in this [Google Colab notebook](https://colab.research.google.com/github/ShawnHymel/ei-keyword-spotting/blob/master/ei-audio-dataset-curation.ipynb). This script helps in organizing and preparing your audio data, which can then be exported to Edge Impulse for further processing and model training.

## Obtaining the Code from Edge Impulse

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
   - Add the downloaded Edge Impulse library to your Arduimo project.
   - Include the necessary headers and functions in your code to use the trained model.

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

## License

This project is licensed under the MIT License.

## Contact

For any questions or support, please contact Gabriel at dinggabriel2@gmail.com.
