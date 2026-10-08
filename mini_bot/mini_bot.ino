#include <BluetoothSerial.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
BluetoothSerial SerialBT;

// ====== Motor Pins ======
const int AIN1 = 22;
const int AIN2 = 23;
const int BIN1 = 18;
const int BIN2 = 19;

// ====== RGB LED Pins ======
const int LED_R = 16;
const int LED_G = 17;
const int LED_B = 4;

// ====== PWM Config ======
const int PWM_FREQ = 5000;
const int PWM_RES = 8;

const int CH_A1 = 0;
const int CH_A2 = 1;
const int CH_B1 = 2;
const int CH_B2 = 3;
const int CH_R = 4;
const int CH_G = 5;
const int CH_B_ = 6; // avoid naming conflict

// ====== Speed Control ======
int speedLevels[4] = {0, 120, 180, 255};
int currentSpeed = 180;
bool connected = false;
bool moving = false;

void setup() {
 // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector (debug only)

  Serial.begin(115200);
  SerialBT.begin("ESP32_Car"); // Bluetooth name
  Serial.println("ESP32 Car Ready - Waiting for Connection...");

  // ===== Setup Motor Channels =====
  ledcSetup(CH_A1, PWM_FREQ, PWM_RES);
  ledcSetup(CH_A2, PWM_FREQ, PWM_RES);
  ledcSetup(CH_B1, PWM_FREQ, PWM_RES);
  ledcSetup(CH_B2, PWM_FREQ, PWM_RES);

  ledcAttachPin(AIN1, CH_A1);
  ledcAttachPin(AIN2, CH_A2);
  ledcAttachPin(BIN1, CH_B1);
  ledcAttachPin(BIN2, CH_B2);

  // ===== Setup RGB LED Channels =====
  ledcSetup(CH_R, PWM_FREQ, PWM_RES);
  ledcSetup(CH_G, PWM_FREQ, PWM_RES);
  ledcSetup(CH_B_, PWM_FREQ, PWM_RES);

  ledcAttachPin(LED_R, CH_R);
  ledcAttachPin(LED_G, CH_G);
  ledcAttachPin(LED_B, CH_B_);

  stopMotors();
  setLED(255, 0, 0); // 🔴 waiting for connection
}

void loop() {
  if (SerialBT.hasClient()) {
    if (!connected) {
      connected = true;
      Serial.println("Bluetooth Connected ✅");
      setLED(0, 255, 0); // 🟢 connected but idle
    }
  } else {
    if (connected) {
      connected = false;
      Serial.println("Bluetooth Disconnected ❌");
      stopMotors();
      setLED(255, 0, 0); // 🔴 disconnected
    }
  }

  if (connected && SerialBT.available()) {
    char cmd = SerialBT.read();
    handleCommand(cmd);
  }
  delay(2);

}

// ====== Motor Control Functions ======
void forward() {
  ledcWrite(CH_A1, currentSpeed);
  ledcWrite(CH_A2, 0);
  ledcWrite(CH_B1, currentSpeed);
  ledcWrite(CH_B2, 0);
  setLED(0, 0, 255); // 🔵 moving
  moving = true;
}

void backward() {
  ledcWrite(CH_A1, 0);
  ledcWrite(CH_A2, currentSpeed);
  ledcWrite(CH_B1, 0);
  ledcWrite(CH_B2, currentSpeed);
  setLED(0, 0, 255);
  moving = true;
}

void left() {
  ledcWrite(CH_A1, 0);
  
  ledcWrite(CH_A2, currentSpeed);
  ledcWrite(CH_B1, currentSpeed);
  ledcWrite(CH_B2, 0);
  setLED(0, 0, 255);
  moving = true;
}

void right() {
  ledcWrite(CH_A1, currentSpeed);
  ledcWrite(CH_A2, 0);
  ledcWrite(CH_B1, 0);
  ledcWrite(CH_B2, currentSpeed);
  setLED(0, 0, 255);
  moving = true;
}

void stopMotors() {
  ledcWrite(CH_A1, 0);
  ledcWrite(CH_A2, 0);
  ledcWrite(CH_B1, 0);
  ledcWrite(CH_B2, 0);
  setLED(0, 255, 0); // 🟢 stopped
  moving = false;
}

// ====== Handle Commands ======
void handleCommand(char c) {
  switch (c) {
    case 'F': forward(); break;
    case 'B': backward(); break;
    case 'L': left(); break;
    case 'R': right(); break;
    case 'S': stopMotors(); break;
    case '1': currentSpeed = speedLevels[1]; break;
    case '2': currentSpeed = speedLevels[2]; break;
    case '3': currentSpeed = speedLevels[3]; break;
  }
  Serial.print("Command: ");
  Serial.println(c);
}

// ====== RGB LED Helper ======
void setLED(int r, int g, int b) {
  // Invert values if using common anode LED
  ledcWrite(CH_R, r);
  ledcWrite(CH_G, g);
  ledcWrite(CH_B_, b);
}
