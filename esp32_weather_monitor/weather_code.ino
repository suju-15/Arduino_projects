#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// --- Pin Definitions --- for esp32-c3
#define SDA_PIN       8
#define SCL_PIN       9
#define DHT_PIN       3
#define BUTTON_PIN    4

#define DHTTYPE       DHT22

// --- OLED Settings ---
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

// --- Config Credentials ---
const char* ssid     = "wifi_ssid";
const char* password = "wifi_pass";

String apiKey   = "open_weather api key";
String cityName = "City, IN"; //city with country 

// --- Objects & Variables ---
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHT_PIN, DHTTYPE);

// Weather Data
float indoorTemp = 0.0;
float indoorHum  = 0.0;
float outdoorTemp = 0.0;
float outdoorHum  = 0.0;
String outdoorMain = "Clear"; // e.g. "Clear", "Rain", "Clouds"

// Screen & Navigation Timers
int currentScreen = 0; // 0 = Indoor, 1 = Outdoor
unsigned long lastAutoSwitch = 0;
const unsigned long autoSwitchInterval = 5000; // 5 seconds per screen change this for desired transition 

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;

unsigned long lastWeatherUpdate = 0;
const unsigned long weatherInterval = 600000; // 10 mins

// Animation Frames
int animFrame = 0;
unsigned long lastAnimUpdate = 0;

void fetchOutdoorWeather() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/weather?q=" + cityName + "&appid=" + apiKey + "&units=metric";
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error) {
        outdoorTemp = doc["main"]["temp"];
        outdoorHum  = doc["main"]["humidity"];
        outdoorMain = doc["weather"][0]["main"].as<String>(); // e.g., Clear, Rain, Clouds, Drizzle
      }
    } else {
      outdoorMain = "Error";
    }
    http.end();
  } else {
    outdoorMain = "No Wi-Fi";
  }
}

// --- Dynamic Procedural Animations ---
void drawWeatherAnimation(int x, int y, String condition) {
  if (condition == "Clear") {
    // Animated Rotating Sun Rays
    display.drawCircle(x, y, 8, SSD1306_WHITE);
    int rayLength = 4;
    float offset = (animFrame % 4) * (M_PI / 8.0);
    
    for (int i = 0; i < 8; i++) {
      float angle = i * (M_PI / 4.0) + offset;
      int x1 = x + cos(angle) * 11;
      int y1 = y + sin(angle) * 11;
      int x2 = x + cos(angle) * (11 + rayLength);
      int y2 = y + sin(angle) * (11 + rayLength);
      display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
    }
  } 
  else if (condition == "Rain" || condition == "Drizzle" || condition == "Thunderstorm") {
    // Cloud dome
    display.fillCircle(x - 5, y - 2, 7, SSD1306_WHITE);
    display.fillCircle(x + 5, y - 2, 6, SSD1306_WHITE);
    display.fillRect(x - 10, y - 2, 20, 6, SSD1306_WHITE);

    // Falling Rain Streak Loop
    int dropOffset = (animFrame % 6);
    display.drawLine(x - 6, y + 6 + dropOffset, x - 8, y + 9 + dropOffset, SSD1306_WHITE);
    display.drawLine(x,     y + 4 + dropOffset, x - 2, y + 7 + dropOffset, SSD1306_WHITE);
    display.drawLine(x + 6, y + 6 + dropOffset, x + 4, y + 9 + dropOffset, SSD1306_WHITE);
  } 
  else { // Clouds / Mist / Snow / Default
    // Drift horizontal cloud layer
    int drift = (animFrame % 8) - 4;
    display.fillCircle(x - 6 + drift, y, 7, SSD1306_WHITE);
    display.fillCircle(x + 4 + drift, y - 2, 9, SSD1306_WHITE);
    display.fillCircle(x + 10 + drift, y + 1, 6, SSD1306_WHITE);
    display.fillRect(x - 10 + drift, y + 1, 20, 6, SSD1306_WHITE);
  }
}

void updateOLED() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  if (currentScreen == 0) {
    // --- Indoor Screen ---
    display.setTextSize(1);
    display.setCursor(24, 0);
    display.print("INDOOR CLIMATE");
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    // Larger Temperature Readout
    display.setCursor(0, 20);
    display.setTextSize(2);
    if (isnan(indoorTemp)) {
      display.print("T: Err");
    } else {
      display.print(indoorTemp, 1);
      display.setTextSize(1);
      display.print(" C");
    }

    // Larger Humidity Readout
    display.setCursor(0, 44);
    display.setTextSize(2);
    if (isnan(indoorHum)) {
      display.print("H: Err");
    } else {
      display.print(indoorHum, 0);
      display.setTextSize(1);
      display.print(" % RH");
    }

  } else {
    // --- Outdoor Screen ---
    display.setTextSize(1);
    display.setCursor(18, 0);
    display.print("OUTDOOR WEATHER");
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    // Render Animation top-right
    drawWeatherAnimation(105, 34, outdoorMain);

    // Condition Text
    display.setCursor(0, 16);
    display.setTextSize(1);
    display.print(outdoorMain);

    // Outdoor Temp (Size 2)
    display.setCursor(0, 28);
    display.setTextSize(2);
    display.print(outdoorTemp, 1);
    display.setTextSize(1);
    display.print(" C");

    // Outdoor Humidity (Size 2)
    display.setCursor(0, 48);
    display.setTextSize(2);
    display.print((int)outdoorHum);
    display.setTextSize(1);
    display.print(" %");
  }

  display.display();
}

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Wire.begin(SDA_PIN, SCL_PIN);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println(" Connecting Wi-Fi...");
  display.display();

  WiFi.begin(ssid, password);
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    retries++;
  }

  fetchOutdoorWeather();
  lastAutoSwitch = millis();
}

void loop() {
  // 1. Manual Toggle Button Interrupt
  if (digitalRead(BUTTON_PIN) == LOW) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      currentScreen = (currentScreen + 1) % 2;
      lastDebounceTime = millis();
      lastAutoSwitch = millis(); // Reset auto timer on button press
    }
  }

  // 2. Automatic Screen Transition (5 Sec)
  if (millis() - lastAutoSwitch >= autoSwitchInterval) {
    currentScreen = (currentScreen + 1) % 2;
    lastAutoSwitch = millis();
  }

  // 3. Increment Animation Frames (~10 FPS)
  if (millis() - lastAnimUpdate >= 100) {
    animFrame++;
    lastAnimUpdate = millis();
  }

  // 4. Periodically Read DHT22
  indoorTemp = dht.readTemperature();
  indoorHum  = dht.readHumidity();

  // 5. Periodically Fetch Weather Data
  if (millis() - lastWeatherUpdate >= weatherInterval) {
    fetchOutdoorWeather();
    lastWeatherUpdate = millis();
  }

  // 6. Draw Frame
  updateOLED();
  delay(20);
}
