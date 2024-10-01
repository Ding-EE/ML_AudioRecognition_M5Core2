// Disable quantization of the filterbank to improve accuracy
#define EIDSP_QUANTIZE_FILTERBANK 0

// Include necessary libraries
#include <Speech_Recognition_inferencing.h>  // Edge Impulse inferencing library for speech recognition
#include <M5Core2.h>  // Library for M5Stack Core2 device, providing hardware abstraction
#include <driver/i2s.h>  // ESP32 I2S driver for audio input/output

// Blynk configuration for IoT connectivity
#define BLYNK_TEMPLATE_ID "TMPL6aBf4tE3e"
#define BLYNK_TEMPLATE_NAME "Audio Detector"
#define BLYNK_AUTH_TOKEN "6Lytdc7ZS_jDHqjPMSxqomHb_FoyarkU"
#include <BlynkSimpleEsp32.h>  // Blynk library for ESP32, simplifying IoT setup

// Wi-Fi credentials for network connectivity
const char* ssid = "/**/";  // Your Wi-Fi SSID (network name)
const char* password = "22334455";  // Your Wi-Fi password

// I2S pin configuration for M5Core2
// These pins are specific to the M5Core2 hardware layout
#define CONFIG_I2S_BCK_PIN 12   // I2S Serial Clock (BCK) pin
#define CONFIG_I2S_LRCK_PIN 0   // I2S Left/Right Clock (LRCK) pin
#define CONFIG_I2S_DATA_PIN 2   // I2S Data Out pin
#define CONFIG_I2S_DATA_IN_PIN 34  // I2S Data In pin (Microphone input)

#define Speak_I2S_NUMBER I2S_NUM_0  // Use I2S0 peripheral for audio

// Mode definitions for I2S configuration
#define MODE_MIC 0  // Microphone mode
#define MODE_SPK 1  // Speaker mode

#define DATA_SIZE 1024  // Size of data chunks for I2S operations (in bytes)

// Screen dimensions and waveform display parameters
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define WAVEFORM_HEIGHT 100
#define WAVEFORM_Y_OFFSET 70

int data_offset = 0;  // Offset for data processing in the audio buffer

// Struct to hold inference-related data
typedef struct {
    int16_t *buffer;     // Buffer to store audio samples
    uint8_t buf_ready;   // Flag to indicate if buffer is ready for processing
    uint32_t buf_count;  // Count of buffers processed
    uint32_t n_samples;  // Number of samples in the buffer
} inference_t;

static inference_t inference;  // Global inference object
static signed short sampleBuffer[2048];  // Buffer for audio samples
static bool debug_nn = false;  // Flag to enable/disable neural network debugging

// Function to initialize I2S for either speaker or microphone mode
bool InitI2SSpeakOrMic(int mode)
{
    esp_err_t err = ESP_OK;
    i2s_driver_uninstall(Speak_I2S_NUMBER);  // Uninstall previous driver, if any

    // Configure I2S
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),  // Set to PDM mode for mic input
        .sample_rate = EI_CLASSIFIER_FREQUENCY,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,  // Standard I2S communication format
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,     // Interrupt allocation level
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true,
    };

    // Set mode-specific configurations
    if (mode == MODE_MIC) {
        i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
    } else {
        i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
    }

    // Install I2S driver
    err = i2s_driver_install(Speak_I2S_NUMBER, &i2s_config, 0, NULL);
    if (err != ESP_OK) {
        ei_printf("ERR: Failed to install I2S driver\n");
        return false;
    }

    // Configure I2S pins
    i2s_pin_config_t pin_config = {
        .bck_io_num = CONFIG_I2S_BCK_PIN,
        .ws_io_num = CONFIG_I2S_LRCK_PIN,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = CONFIG_I2S_DATA_IN_PIN
    };

    // Set I2S pin configuration
    err = i2s_set_pin(Speak_I2S_NUMBER, &pin_config);
    if (err != ESP_OK) {
        ei_printf("ERR: Failed to set I2S pin configuration\n");
        return false;
    }

    // Set I2S clock
    err = i2s_set_clk(Speak_I2S_NUMBER, EI_CLASSIFIER_FREQUENCY, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
    if (err != ESP_OK) {
        ei_printf("ERR: Failed to set I2S clock\n");
        return false;
    }

    return true;
}

// Function to initialize the display
void DisplayInit(void)
{
  M5.Lcd.fillScreen(WHITE);  // Set background to white
  M5.Lcd.setTextColor(BLACK);  // Set text color to black
  M5.Lcd.setTextSize(2);  // Set text size
}

// Setup function - runs once at the start
void setup() {
    M5.begin(true, true, true, true);  // Initialize M5Stack Core2 (enable LCD, SD card, serial port, I2C)
    M5.Axp.SetSpkEnable(true);  // Enable speaker
    DisplayInit();  // Initialize display
    M5.Lcd.setTextColor(RED);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.printf("Audio Recognition");
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);  // Initialize Blynk with authentication and network credentials
    delay(100);

    Serial.begin(115200);  // Initialize serial communication
    Serial.println("Edge Impulse Inferencing Demo");

    // Print inferencing settings
    ei_printf("Inferencing settings:\n");
    ei_printf("\tInterval: %.2f ms.\n", (float)EI_CLASSIFIER_INTERVAL_MS);
    ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    ei_printf("\tSample length: %d ms.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / (EI_CLASSIFIER_FREQUENCY / 1000));
    ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));

    // Start microphone inference
    if (microphone_inference_start(EI_CLASSIFIER_RAW_SAMPLE_COUNT) == false) {
        ei_printf("ERR: Failed to setup audio sampling\r\n");
        return;
    }

    // Initialize I2S for microphone
    if (!InitI2SSpeakOrMic(MODE_MIC)) {
        ei_printf("ERR: Failed to initialize I2S\r\n");
        return;
    }
}

// Main loop function - runs repeatedly
void loop() {
    Blynk.run();  // Run Blynk to maintain connection and process incoming data
    ei_printf("Starting inferencing in 2 seconds...\n");
    delay(2000);

    // Display recording status
    M5.Lcd.fillRect(10, 40, SCREEN_WIDTH - 20, 40, WHITE);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10, 40);
    M5.Lcd.printf("Recording...");
    
    M5.Lcd.fillRect(0, WAVEFORM_Y_OFFSET, SCREEN_WIDTH, WAVEFORM_HEIGHT, WHITE);  // Clear the waveform area
    displayWaveform();  // Display the waveform

    ei_printf("Recording...\n");

    // Record audio
    bool m = microphone_inference_record();
    if (!m) {
        ei_printf("ERR: Failed to record audio...\n");
        return;
    }

    ei_printf("Recording done\n");

    // Display processing status
    M5.Lcd.fillRect(10, 100, SCREEN_WIDTH - 20, 40, WHITE);
    M5.Lcd.setTextColor(BLUE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10, 100);
    M5.Lcd.printf("Processing...");

    displayWaveform();  // Display the waveform

    // Prepare signal for classification
    signal_t signal;
    signal.total_length = EI_CLASSIFIER_RAW_SAMPLE_COUNT;
    signal.get_data = &microphone_audio_signal_get_data;
    ei_impulse_result_t result = { 0 };

    // Run the classifier
    EI_IMPULSE_ERROR r = run_classifier(&signal, &result, debug_nn);
    if (r != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", r);
        return;
    }

    // Clear processing message
    M5.Lcd.fillRect(10, 100, SCREEN_WIDTH - 20, 40, WHITE);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10, 100);

    // Print predictions
    ei_printf("Predictions ");
    ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);
    ei_printf(": \n");

    // Clear previous detection
    M5.Lcd.fillRect(10, 180, SCREEN_WIDTH - 20, 40, WHITE);

    // Display detected word if confidence is above 0.6
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
        if (result.classification[ix].value > 0.6) {
            M5.Lcd.setTextColor(BLUE);
            M5.Lcd.setTextSize(3);
            M5.Lcd.setCursor(10, 190);
            M5.Lcd.printf("Detected: %s", result.classification[ix].label);

            // Send data to Blynk app
            if (strcmp(result.classification[ix].label, "go") == 0) {
                Blynk.virtualWrite(V0, 255);  // Light up V0 (green light)
                Blynk.virtualWrite(V1, 0);    // Turn off V1 (red light)
            } else if (strcmp(result.classification[ix].label, "stop") == 0) {
                Blynk.virtualWrite(V1, 255);  // Light up V1 (red light)
                Blynk.virtualWrite(V0, 0);    // Turn off V0 (green light)
            }    
        }
    }

#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("    anomaly score: %.3f\n", result.anomaly);
#endif

    // Show the inference result status on the display
    M5.Lcd.setTextColor(RED);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10, 140);
    M5.Lcd.printf("Inference complete!");
}

// Function to display the audio waveform
void displayWaveform() {
    M5.Lcd.fillRect(0, WAVEFORM_Y_OFFSET, SCREEN_WIDTH, WAVEFORM_HEIGHT, WHITE);
    
    int16_t x = 0;
    int16_t last_y = WAVEFORM_Y_OFFSET + WAVEFORM_HEIGHT / 2;
    
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        int index = i * (EI_CLASSIFIER_RAW_SAMPLE_COUNT / SCREEN_WIDTH);
        int16_t sample = inference.buffer[index];
        
        int16_t y = map(sample, -32768, 32767, WAVEFORM_Y_OFFSET + WAVEFORM_HEIGHT, WAVEFORM_Y_OFFSET);
        
        M5.Lcd.drawLine(x, last_y, x + 1, y, BLACK);
        
        x++;
        last_y = y;
    }
}

// Function to start microphone inference
static bool microphone_inference_start(uint32_t n_samples)
{
    inference.buffer = (int16_t *)malloc(n_samples * sizeof(int16_t));

    if (inference.buffer == NULL) {
        return false;
    }

    inference.buf_count  = 0;
    inference.n_samples  = n_samples;
    inference.buf_ready  = 0;
 
    return true;
}

// Function to record audio for inference
static bool microphone_inference_record(void)
{
    inference.buf_ready = 0;
    inference.buf_count = 0;

    size_t bytes_read = 0;
    size_t bytes_to_read = inference.n_samples * sizeof(int16_t);

    int32_t sum = 0;
    int16_t max_audio = 0;

    while (bytes_read < bytes_to_read) {
        size_t bytes_left = bytes_to_read - bytes_read;
        size_t chunk_size = (bytes_left < DATA_SIZE) ? bytes_left : DATA_SIZE;

        size_t chunk_bytes_read = 0;
        i2s_read(Speak_I2S_NUMBER, (char *)(inference.buffer + (bytes_read / sizeof(int16_t))), chunk_size, &chunk_bytes_read, portMAX_DELAY);

        if (chunk_bytes_read == 0) {
            return false;
        }
        bytes_read += chunk_bytes_read;

        // Process audio data
        for (int i = 0; i < chunk_bytes_read / sizeof(int16_t); i++) {
            int16_t sample = inference.buffer[bytes_read / sizeof(int16_t) - chunk_bytes_read / sizeof(int16_t) + i];
            sum += abs(sample);
            if (abs(sample) > max_audio) {
                max_audio = abs(sample);
            }
        }
    }

    // Check audio level
    float avg_audio = (float)sum / inference.n_samples;
    ei_printf("Audio stats - avg: %.2f, max: %d\n", avg_audio, max_audio);

    if (avg_audio < 10 || max_audio < 100) {
        ei_printf("Warning: Low audio level detected\n");
    }

    inference.buf_ready = 1;
    return true;
}

// Function to get audio data for the classifier
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr)
{
    numpy::int16_to_float(&inference.buffer[offset], out_ptr, length);

    return 0;
}

// Ensure the correct sensor type is set for the model
#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_MICROPHONE
#error "Invalid model for current sensor."
#endif