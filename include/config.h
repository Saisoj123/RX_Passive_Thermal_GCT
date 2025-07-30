#ifndef CONFIG_H
#define CONFIG_H

/*
 * Project Configuration Header - RX Servant ESP32
 * 
 * Original code base: https://github.com/MoritzNelle/RX_Passive_Thermal_GCT
 * Enhanced by Josias Kern using GitHub Copilot (GPT-4)
 * 
 * This file contains all configurable parameters for the RX Servant ESP32
 */

// ===== DEVICE CONFIGURATION =====
#ifndef GCTID
    #define GCTID               1           // Default GCT ID (1-4), can be overridden by build flags
#endif

#define FIRMWARE_VERSION        "1.2.0"
#define BUILD_DATE              __DATE__ " " __TIME__

// ===== TIMING CONFIGURATION =====
#define PING_INTERVAL_MS        1000        // Ping interval (500 to inf)
#define SENSOR_READ_TIMEOUT_MS  2000        // Temperature sensor read timeout
#define CONNECTION_TIMEOUT_MS   5000        // Master connection timeout

// ===== HARDWARE CONFIGURATION =====
#define ONE_WIRE_BUS            4           // OneWire bus pin for DS18B20
#define SD_CS_PIN               5           // SD Card Chip Select
#define LED_PIN                 2           // Status LED pin
#define SPI_SCK_PIN             18          // SPI Clock pin
#define SPI_MISO_PIN            19          // SPI MISO pin
#define SPI_MOSI_PIN            23          // SPI MOSI pin
#define I2C_SDA_PIN             21          // I2C SDA pin
#define I2C_SCL_PIN             22          // I2C SCL pin

// ===== SENSOR CONFIGURATION =====
#define NUM_SENSORS             9           // Number of DS18B20 sensors
#define SENSOR_RESOLUTION       12          // DS18B20 resolution (9-12 bits)
#define TEMP_ERROR_VALUE        -999.0      // Value to indicate sensor error
#define TEMP_MIN_VALID          -55.0       // Minimum valid temperature
#define TEMP_MAX_VALID          125.0       // Maximum valid temperature

// ===== MASTER ESP32 MAC ADDRESS =====
// This should be the MAC address of the TX (Master) ESP32
// You can get this from the serial monitor when the master boots up
// or from the upload log when flashing the master firmware
#define MASTER_MAC_ADDRESS      {0x48, 0xE7, 0x29, 0x8C, 0x6B, 0x5C}  // Update with actual master MAC

// ===== WATCHDOG CONFIGURATION =====
#define WATCHDOG_TIMEOUT_SEC    30          // Watchdog timeout

// ===== SD CARD CONFIGURATION =====
#define SD_RETRY_COUNT          5           // SD card mount retry attempts
#define SD_RETRY_DELAY_MS       1000        // Delay between SD retry attempts

// ===== ESP-NOW ACTION IDs =====
#define ACTION_CONNECTION_TEST  1001
#define ACTION_START_LOGGING    1002
#define ACTION_STOP_LOGGING     1003
#define ACTION_TEMP_REQUEST     3001
#define ACTION_TEMP_RESPONSE    2001

// ===== FILE CONFIGURATION =====
// Filename will be generated automatically based on GCTID
// Format: "/data_GCT{GCTID}.csv"
#define CSV_HEADER              "timestamp,gct_id,sensor_no,temperature"

// ===== STATUS LED CONFIGURATION =====
#define LED_OFF                 0
#define LED_YELLOW_SOLID        1
#define LED_GREEN_BLINK         2
#define LED_GREEN_SOLID         3
#define LED_RED_SOLID           4
#define LED_RED_BLINK           5
#define LED_YELLOW_BLINK        6
#define LED_RED_WARNING         7

// ===== ERROR RETRY CONFIGURATION =====
#define SENSOR_INIT_RETRIES     5           // Sensor initialization retry count
#define SENSOR_RETRY_DELAY_MS   500         // Delay between sensor retries

// ===== DEBUG CONFIGURATION =====
#ifdef DEBUG
    #define DEBUG_PRINT(x)      Serial.print(x)
    #define DEBUG_PRINTLN(x)    Serial.println(x)
    #define DEBUG_PRINTF(...)   Serial.printf(__VA_ARGS__)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
    #define DEBUG_PRINTF(...)
#endif

#endif // CONFIG_H
