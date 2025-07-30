# Project Consolidation Implementation Plan

## 🎯 **Consolidation Overview**

This document outlines the step-by-step process to consolidate the TX_Passive_Thermal_GCT and RX_Passive_Thermal_GCT projects into a unified **Thermal_GCT_System** repository. This consolidation will improve maintainability, reduce code duplication, and provide a more cohesive development experience.

## 📁 **Proposed Directory Structure**

```
Thermal_GCT_System/
├── README.md                    # Main project documentation
├── SYSTEM_OVERVIEW.md          # System architecture and overview
├── platformio.ini              # Unified PlatformIO configuration
├── build_scripts/              # Build automation
│   ├── build_all.bat          # Build all environments
│   ├── upload_master.bat      # Upload master firmware
│   ├── upload_servant.bat     # Upload servant firmware
│   └── monitor_serial.bat     # Serial monitoring utilities
├── firmware/                   # Source code organization
│   ├── master/                # Master (TX) firmware
│   │   └── src/
│   │       └── main.cpp
│   ├── servant/               # Servant (RX) firmware
│   │   └── src/
│   │       └── main.cpp
│   └── shared/                # Shared libraries and headers
│       ├── include/
│       │   ├── thermal_config.h
│       │   ├── espnow_protocol.h
│       │   └── temperature_sensor.h
│       └── lib/
│           ├── ThermalGCT/
│           ├── ESPNowComm/
│           └── TemperatureSensor/
├── hardware/                   # Hardware documentation
│   ├── schematics/
│   ├── pcb_designs/
│   └── assembly_guides/
├── documentation/              # Comprehensive documentation
│   ├── Hardware_Setup.md
│   ├── LED_Status_Reference.md
│   ├── Action_IDs.txt
│   ├── Thermal_Calibration_Guide.md
│   └── API_Reference.md
├── tools/                      # Development utilities
│   ├── data_analysis/
│   ├── calibration_tools/
│   └── testing/
├── board_drivers/              # Hardware drivers
│   ├── CP210x_Universal_Windows_Driver/
│   └── Mac_OSX_VCP_Driver/
└── examples/                   # Usage examples
    ├── basic_deployment/
    ├── multi_gct_setup/
    └── calibration_workflow/
```

## 🔧 **Implementation Steps**

### **Phase 1: Repository Setup**

1. **Create New Repository:**
   ```powershell
   # Create new unified repository
   mkdir c:\repositories\tcts\Thermal_GCT_System
   cd c:\repositories\tcts\Thermal_GCT_System
   git init
   ```

2. **Preserve Git History:**
   ```powershell
   # Add existing repositories as remotes
   git remote add tx-origin c:\repositories\tcts\TX_Passive_Thermal_GCT
   git remote add rx-origin c:\repositories\tcts\RX_Passive_Thermal_GCT
   
   # Fetch both histories
   git fetch tx-origin
   git fetch rx-origin
   
   # Merge histories preserving branches
   git merge --allow-unrelated-histories tx-origin/main
   git merge --allow-unrelated-histories rx-origin/main
   ```

3. **Directory Restructuring:**
   ```powershell
   # Move TX content to firmware/master/
   mkdir firmware\master\src
   move src\main.cpp firmware\master\src\
   move include firmware\master\
   
   # Move RX content to firmware/servant/
   mkdir firmware\servant\src
   move RX_src\main.cpp firmware\servant\src\
   move RX_include firmware\servant\
   
   # Consolidate documentation
   mkdir documentation
   move doc\* documentation\
   move README.md documentation\README_TX.md
   move RX_README.md documentation\README_RX.md
   ```

### **Phase 2: PlatformIO Configuration**

1. **Unified platformio.ini:**
   ```ini
   [platformio]
   default_envs = tx-master-esp32
   
   [env]
   platform = espressif32
   board = nodemcu-32s
   framework = arduino
   monitor_speed = 115200
   lib_deps = 
       https://github.com/PaulStoffregen/OneWire
       https://github.com/milesburton/Arduino-Temperature-Control-Library
       adafruit/Adafruit SSD1306@^2.5.7
       adafruit/Adafruit GFX Library@^1.11.3
       adafruit/Adafruit BusIO@^1.14.1
   
   [env:tx-master-esp32]
   src_dir = firmware/master/src
   build_flags = 
       -D MASTER_DEVICE
       -D ESP_NOW_CHANNEL=1
       -D MAX_SERVANTS=4
   
   [env:rx-gct1-esp32]
   src_dir = firmware/servant/src
   build_flags = 
       -D SERVANT_DEVICE
       -D GCT_ID=1
       -D ESP_NOW_CHANNEL=1
   
   [env:rx-gct2-esp32]
   src_dir = firmware/servant/src
   build_flags = 
       -D SERVANT_DEVICE
       -D GCT_ID=2
       -D ESP_NOW_CHANNEL=1
   
   [env:rx-gct3-esp32]
   src_dir = firmware/servant/src
   build_flags = 
       -D SERVANT_DEVICE
       -D GCT_ID=3
       -D ESP_NOW_CHANNEL=1
   
   [env:rx-gct4-esp32]
   src_dir = firmware/servant/src
   build_flags = 
       -D SERVANT_DEVICE
       -D GCT_ID=4
       -D ESP_NOW_CHANNEL=1
   ```

2. **Build Scripts Creation:**

   **build_scripts/build_all.bat:**
   ```batch
   @echo off
   echo Building all Thermal GCT firmware...
   
   echo.
   echo Building Master firmware...
   pio run -e tx-master-esp32
   if %errorlevel% neq 0 (
       echo Master build failed!
       exit /b 1
   )
   
   echo.
   echo Building Servant firmwares...
   pio run -e rx-gct1-esp32
   pio run -e rx-gct2-esp32
   pio run -e rx-gct3-esp32
   pio run -e rx-gct4-esp32
   
   if %errorlevel% neq 0 (
       echo Servant build failed!
       exit /b 1
   )
   
   echo.
   echo All builds completed successfully!
   pause
   ```

   **build_scripts/upload_master.bat:**
   ```batch
   @echo off
   set /p PORT="Enter COM port for Master unit (e.g., COM14): "
   echo Uploading Master firmware to %PORT%...
   pio run -e tx-master-esp32 --target upload --upload-port %PORT%
   if %errorlevel% equ 0 (
       echo Master upload successful!
       echo Opening serial monitor...
       pio device monitor --port %PORT% --baud 115200
   ) else (
       echo Master upload failed!
   )
   pause
   ```

   **build_scripts/upload_servant.bat:**
   ```batch
   @echo off
   set /p GCT_ID="Enter GCT ID (1-4): "
   set /p PORT="Enter COM port for GCT-%GCT_ID% (e.g., COM15): "
   
   echo Uploading Servant firmware for GCT-%GCT_ID% to %PORT%...
   pio run -e rx-gct%GCT_ID%-esp32 --target upload --upload-port %PORT%
   
   if %errorlevel% equ 0 (
       echo GCT-%GCT_ID% upload successful!
       echo Opening serial monitor...
       pio device monitor --port %PORT% --baud 115200
   ) else (
       echo GCT-%GCT_ID% upload failed!
   )
   pause
   ```

### **Phase 3: Shared Libraries**

1. **Create Shared Configuration:**

   **firmware/shared/include/thermal_config.h:**
   ```cpp
   #ifndef THERMAL_CONFIG_H
   #define THERMAL_CONFIG_H
   
   // System Configuration
   #define THERMAL_GCT_VERSION "2.0.0"
   #define ESP_NOW_CHANNEL 1
   #define MAX_SERVANTS 4
   #define TEMPERATURE_SENSORS_PER_GCT 9
   
   // Timing Configuration
   #define LOGGING_INTERVAL_MS 10000  // 10 seconds
   #define BUTTON_DEBOUNCE_MS 50
   #define SERVANT_TIMEOUT_MS 5000
   #define DISPLAY_UPDATE_INTERVAL_MS 1000
   
   // Pin Configuration
   #define BUTTON_PIN 0
   #define LED_PIN 2
   #define ONE_WIRE_BUS_PIN 4
   #define OLED_SDA_PIN 21
   #define OLED_SCL_PIN 22
   
   // Action IDs (ESP-NOW Communication)
   #define ACTION_CONNECTION_TEST 1001
   #define ACTION_TEMPERATURE_DATA 2001
   #define ACTION_DATA_REQUEST 3001
   
   // LED Status Patterns
   #define LED_OFF 0
   #define LED_SLOW_BLINK 1
   #define LED_FAST_BLINK 2
   #define LED_SOLID 3
   
   // Data Validation
   #define MIN_VALID_TEMP -55.0
   #define MAX_VALID_TEMP 125.0
   #define DISCONNECTED_SENSOR_VALUE -999.0
   
   #endif // THERMAL_CONFIG_H
   ```

   **firmware/shared/include/espnow_protocol.h:**
   ```cpp
   #ifndef ESPNOW_PROTOCOL_H
   #define ESPNOW_PROTOCOL_H
   
   #include <WiFi.h>
   #include <esp_now.h>
   #include "thermal_config.h"
   
   // ESP-NOW Message Structures
   typedef struct {
       uint16_t action_id;
       uint8_t gct_id;
       uint8_t sender_mac[6];
       uint32_t timestamp;
       uint8_t data_length;
       uint8_t data[200];  // Flexible data payload
   } espnow_message_t;
   
   typedef struct {
       uint8_t gct_id;
       float temperatures[TEMPERATURE_SENSORS_PER_GCT];
       uint32_t timestamp;
       bool is_logging;
   } temperature_data_t;
   
   // Protocol Functions
   bool initESPNow();
   bool sendMessage(const uint8_t* recipient_mac, const espnow_message_t* message);
   void registerReceiveCallback(void (*callback)(const uint8_t*, const uint8_t*, int));
   void broadcastMessage(const espnow_message_t* message);
   
   #endif // ESPNOW_PROTOCOL_H
   ```

2. **Create Shared Libraries:**
   
   **firmware/shared/lib/ThermalGCT/ThermalGCT.h:**
   ```cpp
   #ifndef THERMAL_GCT_H
   #define THERMAL_GCT_H
   
   #include "thermal_config.h"
   #include "espnow_protocol.h"
   
   class ThermalGCT {
   private:
       uint8_t gct_id;
       bool is_master;
       bool is_logging;
       unsigned long last_log_time;
       
   public:
       ThermalGCT(uint8_t id, bool master = false);
       void begin();
       void update();
       void startLogging();
       void stopLogging();
       bool isLogging() const;
       uint8_t getGCTId() const;
   };
   
   #endif // THERMAL_GCT_H
   ```

### **Phase 4: Documentation Migration**

1. **Consolidate Documentation:**
   ```powershell
   # Move all documentation to unified location
   mkdir documentation
   copy TX_Passive_Thermal_GCT\doc\* documentation\
   copy RX_Passive_Thermal_GCT\doc\* documentation\
   
   # Update internal references
   # (Update file paths in documentation to reflect new structure)
   ```

2. **Create Master README.md:**
   ```markdown
   # Thermal Ground Control Target (GCT) System
   
   A distributed ESP32-based thermal monitoring system for thermal-infrared 
   drone calibration using ESP-NOW communication protocol.
   
   ## Quick Start
   
   1. **Build all firmware:**
      ```
      cd build_scripts
      build_all.bat
      ```
   
   2. **Upload Master firmware:**
      ```
      upload_master.bat
      ```
   
   3. **Upload Servant firmware:**
      ```
      upload_servant.bat
      ```
   
   ## Documentation
   
   - [System Overview](documentation/SYSTEM_OVERVIEW.md)
   - [Hardware Setup](documentation/Hardware_Setup.md)
   - [Thermal Calibration Guide](documentation/Thermal_Calibration_Guide.md)
   - [LED Status Reference](documentation/LED_Status_Reference.md)
   - [Communication Protocol](documentation/Action_IDs.txt)
   
   ## Directory Structure
   
   - `firmware/master/` - Master unit firmware (coordinator)
   - `firmware/servant/` - Servant unit firmware (sensors)
   - `firmware/shared/` - Shared libraries and configuration
   - `build_scripts/` - Build and upload automation
   - `documentation/` - Comprehensive system documentation
   - `hardware/` - Schematics and assembly guides
   - `tools/` - Development and analysis utilities
   ```

### **Phase 5: Testing and Validation**

1. **Build Verification:**
   ```powershell
   # Test all build environments
   cd c:\repositories\tcts\Thermal_GCT_System
   pio run -e tx-master-esp32
   pio run -e rx-gct1-esp32
   pio run -e rx-gct2-esp32
   pio run -e rx-gct3-esp32
   pio run -e rx-gct4-esp32
   ```

2. **Functionality Testing:**
   - Upload master firmware and verify operation
   - Upload servant firmware to multiple units
   - Test ESP-NOW communication between units
   - Verify temperature logging functionality
   - Validate button operation and LED indicators

3. **Documentation Verification:**
   - Review all documentation for accuracy
   - Test build scripts on clean environment
   - Verify hardware setup instructions
   - Validate calibration workflow

## 📋 **Migration Checklist**

### **Pre-Migration:**
- [ ] Backup existing repositories
- [ ] Document current functionality state
- [ ] Identify shared code components
- [ ] Plan directory structure

### **Migration Execution:**
- [ ] Create unified repository
- [ ] Restructure directory layout
- [ ] Update PlatformIO configuration
- [ ] Create build automation scripts
- [ ] Migrate shared libraries
- [ ] Consolidate documentation
- [ ] Update file references

### **Post-Migration:**
- [ ] Test build system
- [ ] Verify firmware functionality
- [ ] Validate documentation accuracy
- [ ] Update development workflows
- [ ] Archive old repositories

## 🎯 **Benefits of Consolidation**

1. **Simplified Development:**
   - Single repository to clone and manage
   - Unified build system with automated scripts
   - Shared libraries eliminate code duplication
   - Consistent coding standards and practices

2. **Improved Maintenance:**
   - Single location for bug fixes and updates
   - Coordinated version management
   - Centralized documentation and issue tracking
   - Easier dependency management

3. **Enhanced Collaboration:**
   - Single point of entry for new developers
   - Comprehensive documentation in one location
   - Unified development workflow
   - Better visibility of system architecture

4. **Operational Efficiency:**
   - Streamlined build and deployment process
   - Automated testing and validation
   - Consistent hardware configuration
   - Simplified troubleshooting procedures

---

*This consolidation plan transforms the distributed TX/RX repositories into a cohesive, maintainable thermal calibration system suitable for professional drone thermal-infrared calibration workflows.*
