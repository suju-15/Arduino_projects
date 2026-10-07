# Arduino & ESP32 Projects

A growing collection of my Arduino and ESP32 sketches — practice code, small builds and the documentation that goes with them. Each project lives in its own folder, and every sketch folder is named to match its `.ino` file (the Arduino IDE convention).

---

## Projects

### 🌦️ ESP32 Weather Monitor — [`esp32_weather_monitor/`](./esp32_weather_monitor/)
An ESP32-C3 weather station on a 128×64 SSD1306 OLED:
- **Indoor** temperature & humidity from a DHT22 sensor
- **Outdoor** conditions pulled live from the OpenWeatherMap API
- Screens auto-cycle every 5 seconds, with a push button to switch manually
- Connects over Wi-Fi and refreshes the weather every 10 minutes

Sketch: [`esp32_weather_monitor.ino`](./esp32_weather_monitor/esp32_weather_monitor.ino)

### 🚗 NodeMCU WiFi Robot Car — [`nodemcu_wifi_car/`](./nodemcu_wifi_car/)
A Wi-Fi controlled robot car on an ESP8266 NodeMCU with an L298N motor driver. The board hosts its own Wi-Fi access point and web server, so you can drive it from the *ESP8266 WiFi Robot Car* Android app.

Sketch: [`nodemcu_wifi_car.ino`](./nodemcu_wifi_car/nodemcu_wifi_car.ino)

### 🔢 LCD Binary Counter — [`lcd_binary_counter/`](./lcd_binary_counter/)
A small demo that counts a byte upward on a 16×2 LCD, showing each value in both decimal and binary, and flashing a "Bit !!" message on overflow.

Sketch: [`lcd_binary_counter.ino`](./lcd_binary_counter/lcd_binary_counter.ino) · Wiring: [`wiring_diagram.png`](./lcd_binary_counter/wiring_diagram.png)

---

## Repository structure

```
Arduino_projects/
├── esp32_weather_monitor/
│   └── esp32_weather_monitor.ino
├── lcd_binary_counter/
│   ├── lcd_binary_counter.ino
│   └── wiring_diagram.png
├── nodemcu_wifi_car/
│   └── nodemcu_wifi_car.ino
└── README.md
```

---

## Hardware used

- ESP32-C3 and ESP8266 NodeMCU
- SSD1306 OLED (I2C) and a 16×2 LCD (parallel) with a contrast potentiometer
- DHT22 sensor, L298N motor driver

---

## Getting started

Each project is a self-contained Arduino sketch:

1. Open the project's `.ino` file in the Arduino IDE.
2. Install the libraries it lists in its `#include` lines (e.g. `Adafruit_SSD1306`, `DHT`, `ArduinoJson`, `ESP8266WiFi`, `LiquidCrystal`).
3. Fill in your own Wi-Fi credentials and API keys where required before flashing.
4. Select the right board (ESP32-C3 / ESP8266) and upload.
