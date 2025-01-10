# PH Sensor Data Logger with Favoriot Integration

## Overview
This Arduino sketch connects an ESP8266-based device to a WiFi network, reads data from a pH sensor, displays the pH value on an OLED screen, and sends the data to the Favoriot platform via an HTTP POST request. The code ensures accurate pH readings by averaging sensor data and formatting it in JSON before sending it to Favoriot.

## Components Used
1. **ESP8266**: The microcontroller used for WiFi connectivity and data processing.
2. **pH Sensor**: Measures the pH value of a solution.
3. **OLED Display (128x64)**: Displays the pH value locally.
4. **Favoriot Platform**: A cloud-based IoT platform to store and analyze sensor data.

## Libraries Used
- `SPI.h`: Provides SPI communication.
- `ESP8266WiFi.h`: Handles WiFi connectivity for ESP8266.
- `Adafruit_GFX.h`: Provides graphical functions for OLED display.
- `Adafruit_SSD1306.h`: Specific driver for the SSD1306 OLED display.

## Code Explanation

### WiFi Configuration
The code starts by configuring WiFi credentials:
```cpp
char ssid[] = "xxxxx";      // WiFi SSID
char pass[] = "xxxxx";      // WiFi Password
```
It establishes a connection to the WiFi network and prints the connection status to the serial monitor.

### OLED Initialization
The OLED display is initialized using the `Adafruit_SSD1306` library. If initialization fails, the code enters an infinite loop:
```cpp
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
}
```

### pH Sensor Data Processing
The code reads data from the pH sensor connected to analog pin `A0`. To improve accuracy:
1. 10 samples are collected.
2. The samples are sorted to remove noise.
3. The average of the 6 center samples is calculated.
4. The analog readings are converted to pH values using calibration factors:
```cpp
float phValue = (float)avgValue * 5.0 / 1024 / 6; // Convert analog to millivolt
phValue = 3.5 * phValue;  // Convert millivolt to pH
phValue = phValue - 4;    // Apply offset
```

### Displaying Data on OLED
The pH value is displayed on the OLED screen using the Adafruit GFX functions:
```cpp
display.setTextSize(2);
display.setCursor(20, 0);
display.print("PH Value");

display.setTextSize(3);
display.setCursor(30, 30);
display.print(phValue);
display.display();
```

### Sending Data to Favoriot
The pH value is sent to the Favoriot platform in JSON format:
```cpp
String json = "{\"device_developer_id\":\"" + device_developer_id + "\",\"data\":{\"PH Value:\":\"" + phValue + "\"}}";
```
The HTTP POST request includes:
- API Key
- Content-Type header
- JSON payload

### Favoriot API Integration
The API key and device developer ID must be configured before running the code:
```cpp
const String device_developer_id = "deviceDefault@favoriot"; // Replace with your device ID
const apikey = "rw-apikey/accesstoken";                      // Replace with your Favoriot API key
```

## How It Works
1. **Setup**: Connects to WiFi, initializes the OLED, and displays a startup message.
2. **Loop**:
   - Reads and processes pH sensor data.
   - Displays the pH value on the OLED.
   - Sends the data to Favoriot in JSON format.

## Key Notes
- Ensure proper calibration of the pH sensor for accurate readings.
- Replace the placeholder WiFi credentials and Favoriot API key with actual values.
- Check the OLED I2C address (`0x3c`) to match your hardware configuration.

## Example Output
### Serial Monitor
```
Connecting...
......
3
Connected!
    pH:7.45
{"device_developer_id":"deviceDefault@favoriot","data":{"PH Value:":"7.45"}}
```

### OLED Display
```
PH Value
  7.45
```

## Dependencies
Install the following libraries from the Arduino Library Manager:
- Adafruit GFX Library
- Adafruit SSD1306
- ESP8266WiFi

## Future Improvements
- Add error handling for network issues.
- Implement dynamic calibration for the pH sensor.
- Secure sensitive data (API key, WiFi credentials) using a configuration file or encryption.

---
This README serves as a comprehensive guide to understanding, setting up, and running the pH sensor logger integrated with the Favoriot platform.

