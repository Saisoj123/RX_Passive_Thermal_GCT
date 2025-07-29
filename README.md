# RX Passive Thermal GCT - Servant ESP32

## Project Overview
This is a servant ESP32 device for the Thermal Ground Contact Temperature (GCT) monitoring system. It reads temperatures from up to 9 DS18B20 sensors and transmits data to the master ESP32 via ESP-NOW protocol.

## Credits and Attribution
**Original Code Base**: This project is based on the initial implementation by MoritzNelle:
- Original RX repository: https://github.com/MoritzNelle/RX_Passive_Thermal_GCT
- Original TX repository: https://github.com/MoritzNelle/TX_Passive_Thermal_GCT

**Development**: Enhanced and extended by **Josias Kern** using **GitHub Copilot** (GPT-4 model) for:
- Automatic filename generation based on device ID
- System robustness improvements (watchdog timers, error handling)
- Sensor validation and error recovery mechanisms
- Automated build configurations and project infrastructure
- Comprehensive documentation and setup automation

## Features
- **DS18B20 Temperature Sensors**: Supports up to 9 OneWire temperature sensors
- **ESP-NOW Communication**: Wireless communication with master device
- **SD Card Logging**: Local data backup when logging is active
- **RTC Timekeeping**: Real-time clock for accurate timestamps
- **Status LED**: Visual system status indication
- **Watchdog Timer**: System reliability and auto-recovery
- **Robust Error Handling**: Graceful handling of sensor and component failures
- **Auto-Configuration**: Filename generation based on device ID

## Hardware Requirements
- ESP32 development board (NodeMCU-32S compatible)
- Up to 9x DS18B20 temperature sensors
- SD card module
- DS3231 RTC module
- NeoPixel LED (WS2812B)
- 4.7kΩ pullup resistor for OneWire bus
- MicroSD card

## Pin Configuration
| Component | Pin | Notes |
|-----------|-----|-------|
| OneWire Bus | GPIO 4 | DS18B20 data line + 4.7kΩ pullup |
| SD Card CS | GPIO 5 | SPI Chip Select |
| Status LED | GPIO 2 | NeoPixel Data |
| RTC SDA | GPIO 21 | I2C Data |
| RTC SCL | GPIO 22 | I2C Clock |
| SPI SCK | GPIO 18 | SD Card Clock |
| SPI MISO | GPIO 19 | SD Card Data Out |
| SPI MOSI | GPIO 23 | SD Card Data In |

## Configuration
Edit `include/config.h` to customize:
- Device GCTID (1-4)
- Master device MAC address
- Sensor configuration
- Hardware pin assignments

## Building and Uploading

### Standard Build (GCTID set in code)
```bash
pio run -e rx-servant-esp32
pio run -e rx-servant-esp32 --target upload
```

### Device-Specific Builds (GCTID set by build flag)
```bash
# For GCT Device 1
pio run -e rx-gct1 --target upload

# For GCT Device 2
pio run -e rx-gct2 --target upload

# For GCT Device 3
pio run -e rx-gct3 --target upload

# For GCT Device 4
pio run -e rx-gct4 --target upload
```

### Debug Build
```bash
pio run -e rx-servant-debug
pio run -e rx-servant-debug --target upload
```

## Operation
1. **Startup**: Device initializes sensors, SD card, and RTC
2. **Sensor Reading**: Continuously monitors all DS18B20 sensors
3. **Connection Monitoring**: Maintains connection with master device
4. **Data Transmission**: Sends temperature data when requested
5. **Local Logging**: Saves data to SD card when logging is active

## Status LED Indicators
- **Off**: System ready
- **Yellow Solid**: Initializing
- **Green Blink**: Connected to master, not logging
- **Green Solid**: Connected to master, logging active
- **Yellow Blink**: Connection lost with master
- **Red Blink**: System error (SD card, sensor, etc.)
- **Red Solid**: Critical error
- **Red Fast Blink**: RTC time warning

## Device Configuration
Each servant device must have a unique GCTID (1-4). This can be set in two ways:

### Method 1: Code Configuration
Edit `include/config.h`:
```c
#define GCTID 1  // Change to 2, 3, or 4 for other devices
```

### Method 2: Build Flag Configuration
Use the pre-configured environments in `platformio.ini`:
- `rx-gct1`: GCTID = 1, filename = `/data_GCT1.csv`
- `rx-gct2`: GCTID = 2, filename = `/data_GCT2.csv`
- `rx-gct3`: GCTID = 3, filename = `/data_GCT3.csv`
- `rx-gct4`: GCTID = 4, filename = `/data_GCT4.csv`

## Sensor Wiring
DS18B20 sensors should be wired in parallel on the OneWire bus:
```
ESP32 GPIO4 ----+---- DS18B20 #1 Data
                |
                +---- DS18B20 #2 Data
                |
                +---- DS18B20 #3 Data
                |
                ... (up to 9 sensors)
                |
                +---- 4.7kΩ Resistor ---- 3.3V

All DS18B20 VDD pins ---- 3.3V
All DS18B20 GND pins ---- GND
```

## File Structure
```
RX_Passive_Thermal_GCT/
├── include/
│   └── config.h           # Configuration header
├── src/
│   └── main.cpp          # Main application code
├── platformio.ini        # PlatformIO configuration
└── README.md            # This file
```

## Data Format
CSV data logged to `/data_GCT{GCTID}.csv`:
```
timestamp,gct_id,sensor_no,temperature
2025-07-29 14:30:15,1,1,23.5
2025-07-29 14:30:15,1,2,24.1
...
```

## Troubleshooting
- **Sensor Not Found**: Check OneWire wiring and pullup resistor
- **SD Card Error**: Check card format (FAT32), connection, and card health
- **No Master Connection**: Verify master MAC address in config.h
- **RTC Error**: Check I2C connections and RTC battery
- **Temperature Reading -999**: Sensor disconnected or failed

## Version History
- **v1.2.0**: Added auto-filename generation, improved error handling, watchdog timer
- **v1.1.0**: Enhanced sensor validation, SD card robustness
- **v1.0.0**: Initial release with basic functionality
