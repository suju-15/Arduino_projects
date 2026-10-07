# Arduino & ESP32 Projects

A growing collection of my Arduino and ESP32 sketches — practice code, small builds and the documentation that goes with them.

---

## Projects

### 🌦️ ESP32 Weather Monitor — `esp32_weather_monitor/`
An ESP32-C3 weather station on a 128×64 SSD1306 OLED:
- **Indoor** temperature & humidity from a DHT22 sensor
- **Outdoor** conditions pulled live from the OpenWeatherMap API
- Screens auto-cycle every 5 seconds, with a push button to switch manually
- Connects over Wi-Fi and refreshes the weather every 10 minutes

### 🚗 NodeMCU WiFi Robot Car — `nodemcu_car_v2.ino`
A Wi-Fi controlled robot car on an ESP8266 NodeMCU with an L298N motor driver. The board hosts its own Wi-Fi access point and web server, so you can drive it from the *ESP8266 WiFi Robot Car* Android app.

### 🔢 LCD Binary Counter — `binary_counter.ino`
A small demo that counts a byte upward on a 16×2 LCD, showing each value in both decimal and binary, and flashing a "Bit !!" message on overflow.

---

## Hardware used

- ESP32-C3 and ESP8266 NodeMCU
- SSD1306 OLED (I2C) and a 16×2 LCD (parallel)
- DHT22 sensor, L298N motor driver

---

## Notes

Each sketch lives in its own file or folder and is meant to be opened in the Arduino IDE. Fill in your own Wi-Fi credentials and API keys before flashing.
