# Mini Bot — Pin Connections

Complete net-level pin map for the ESP32 Bluetooth robot car. Every GPIO below was cross-checked against `mini_bot.ino`.

---

## 1. Power supply & regulation

| Net | Source | Destination |
|-----|--------|-------------|
| `BAT+` (≈7.4 V – 8.4 V) | 2 × 18650 Li-ion in series | DRV8833 `VM`, AMS1117-3.3 `VIN`, input decoupling caps |
| `GND` | Battery negative | Common system ground (all blocks) |
| `+3V3` | AMS1117-3.3 `VOUT` | ESP32 3.3 V, DRV8833 `STBY`, RGB anode, UART header 3.3 V, EN pull-up |

**AMS1117-3.3 (U2)**

| Pin | Connects to |
|-----|-------------|
| `VIN` | `BAT+` rail (with 10 µF + 0.1 µF to GND) |
| `VOUT` | `+3V3` rail (with 22 µF + 0.1 µF to GND) |
| `GND` | System ground |

---

## 2. ESP32 → DRV8833 (motor driver)

| DRV8833 pin | ESP32 GPIO | Function |
|-------------|:----------:|----------|
| `AIN1` | GPIO 22 | Motor A — PWM channel `CH_A1` |
| `AIN2` | GPIO 23 | Motor A — PWM channel `CH_A2` |
| `BIN1` | GPIO 18 | Motor B — PWM channel `CH_B1` |
| `BIN2` | GPIO 19 | Motor B — PWM channel `CH_B2` |

**DRV8833 configuration**

| Pin | Connects to |
|-----|-------------|
| `VM` | `BAT+` (≈7.4 V motor supply) |
| `STBY` / `EEP` / `SLP` | `+3V3` (tied high — always enabled) |
| `GND` | Common system ground |
| `OUT1` / `OUT2` | Motor M1 |
| `OUT3` / `OUT4` | Motor M2 |

PWM: `ledcSetup` @ 5000 Hz, 8-bit resolution.

---

## 3. RGB status indicator (common anode)

| LED pin | ESP32 GPIO | Notes |
|---------|:----------:|-------|
| Anode `+` | `+3V3` | common anode |
| Red cathode | GPIO 16 | via 220–330 Ω → PWM `CH_R` |
| Green cathode | GPIO 17 | via 220–330 Ω → PWM `CH_G` |
| Blue cathode | GPIO 4 | via 220–330 Ω → PWM `CH_B_` |

> Common anode: a colour lights when its GPIO is pulled **LOW**.

---

## 4. Reset & programming header

**Reset circuit (`EN` / `CHIP_PU`)**

| Part | Connection |
|------|------------|
| Tactile pushbutton (SW1) | `EN` ↔ `GND` |
| Pull-up resistor (10 kΩ) | `EN` ↔ `+3V3` |
| Debounce capacitor (0.1 µF) | `EN` ↔ `GND` |

**UART / bootloader header (1 × 5)**

| Header pin | Connects to |
|------------|-------------|
| 3.3V | `+3V3` rail |
| TX | ESP32 `U0TXD` / GPIO 1 → programmer RX |
| RX | ESP32 `U0RXD` / GPIO 3 ← programmer TX |
| GPIO 0 | breakout (button/jumper to GND — pull LOW at boot to flash) |
| GND | System ground |

---

## Quick reference (all GPIOs)

| GPIO | Used for |
|:----:|----------|
| 22 | DRV8833 `AIN1` (motor A) |
| 23 | DRV8833 `AIN2` (motor A) |
| 18 | DRV8833 `BIN1` (motor B) |
| 19 | DRV8833 `BIN2` (motor B) |
| 16 | RGB red cathode |
| 17 | RGB green cathode |
| 4 | RGB blue cathode |
| 1 | UART TX (`U0TXD`) |
| 3 | UART RX (`U0RXD`) |
| 0 | Boot / flash select |
