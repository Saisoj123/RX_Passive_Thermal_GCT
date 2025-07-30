# Thermal Ground Control Target (GCT) System

## 🎯 **Purpose: Thermal-Infrared Drone Calibration**

This system provides **ground truth temperature data** for calibrating thermal-infrared imaging systems on unmanned aerial vehicles (UAVs/drones). The Thermal GCT system serves as a **reference standard** for validating and correcting thermal imagery collected from aerial platforms.

### **Thermal-IR Calibration Application:**
- **Ground Reference Points**: Distributed temperature sensors provide known thermal signatures
- **Calibration Targets**: Multiple thermal zones for sensor validation across temperature ranges  
- **Real-time Data**: Synchronized temperature logging during drone thermal imaging flights
- **Spatial Distribution**: Up to 4 separate GCT units for wide-area calibration coverage
- **Data Correlation**: Timestamped temperature data matches drone flight logs for precise calibration

## 🏗️ **System Architecture**

### **Master-Servant Network:**
```
┌─────────────────┐    ESP-NOW     ┌──────────────────┐
│   TX Master     │◄──────────────►│   RX Servant 1   │
│   (Data Logger) │                │   (GCT ID: 1)    │
│   - WiFi/NTP    │                │   - 9 Sensors    │
│   - SD Logging  │    ESP-NOW     │   - Local Log    │
│   - LCD Display │◄──────────────►│   - Status LED   │
│   - Button UI   │                └──────────────────┘
│   - LED Status  │    ESP-NOW     ┌──────────────────┐
│                 │◄──────────────►│   RX Servant 2   │
│                 │                │   (GCT ID: 2)    │
│                 │    ESP-NOW     └──────────────────┘
│                 │◄──────────────►┌──────────────────┐
│                 │                │   RX Servant 3   │
│                 │                │   (GCT ID: 3)    │
│                 │                └──────────────────┘
│                 │    ESP-NOW     ┌──────────────────┐
│                 │◄──────────────►│   RX Servant 4   │
│                 │                │   (GCT ID: 4)    │
└─────────────────┘                └──────────────────┘
```

### **Hardware Components:**
- **ESP32 NodeMCU-32S** microcontrollers (1 master + up to 4 servants)
- **DS18B20 temperature sensors** (9 per servant = up to 36 total sensors)
- **RTC DS3231 modules** for precise timestamping
- **MicroSD cards** for local data storage
- **20x4 LCD display** on master for system status
- **NeoPixel RGB LEDs** for visual status indication
- **Push button** for logging control

## 📊 **Data Collection Process**

### **Thermal Calibration Workflow:**
1. **Pre-flight Setup**: Deploy GCT units in thermal imaging area
2. **System Activation**: Master coordinates all servants via ESP-NOW
3. **Temperature Logging**: Synchronized data collection at configurable intervals
4. **Drone Flight**: Thermal imaging while GCT system logs reference temperatures
5. **Data Correlation**: Match timestamped GCT data with drone thermal imagery
6. **Calibration Processing**: Use ground truth data to correct aerial thermal measurements

### **Data Format:**
```csv
timestamp,gct_id,sensor_no,temperature
2025-07-30 12:34:56,1,1,24.37
2025-07-30 12:34:56,1,2,24.44
2025-07-30 12:34:56,1,3,24.63
...
```

## 🔧 **Key Features**

### **Reliable Operation:**
- **Watchdog timers** prevent system lockups during long deployments
- **Automatic reconnection** for intermittent communication
- **Robust SD card handling** with automatic remount capability
- **Sensor validation** with error detection and reporting

### **User Interface:**
- **LCD status display** shows connection status and logging state
- **Button control** for starting/stopping data collection
- **LED status indicators** for quick system health assessment
- **Serial debug output** for detailed system monitoring

### **Flexible Deployment:**
- **Configurable logging intervals** (default 10 seconds)
- **Selective servant operation** (works with 1-4 servants connected)
- **Individual device addressing** via GCT ID configuration
- **Network topology resilience** continues operation with partial connectivity

## 🚀 **Current Status**

### **Recent Enhancements:**
- ✅ **WiFi/NTP synchronization** for accurate timestamping
- ✅ **Button control system** with proper debounce logic  
- ✅ **Flexible logging** operates with partial servant connectivity
- ✅ **Enhanced debugging** with comprehensive status messages
- ✅ **Data integrity fixes** preventing cross-contamination between servants
- ✅ **Improved error handling** for timeout and connection management

### **System Reliability:**
- **Proven operation** with 2-3 servants simultaneously
- **Stable communication** via ESP-NOW protocol on WiFi channel 1
- **Accurate data logging** with timestamp synchronization
- **Robust error recovery** from temporary disconnections

## 📚 **Documentation Structure**

### **Project Organization:**
- **TX_Passive_Thermal_GCT/**: Master node firmware and configuration
- **RX_Passive_Thermal_GCT/**: Servant node firmware and configuration
- **doc/**: System documentation and reference materials
- **board_driver/**: USB driver files for ESP32 programming

### **Getting Started:**
1. **Hardware Assembly**: Connect temperature sensors and peripherals
2. **Firmware Upload**: Program master and servant ESP32 devices
3. **System Configuration**: Set GCT IDs and network parameters
4. **Field Deployment**: Position units for thermal calibration coverage
5. **Data Collection**: Use button control to start/stop logging during drone flights

## 🎯 **Thermal-IR Drone Calibration Benefits**

### **Improved Accuracy:**
- **Ground truth validation** ensures thermal measurement precision
- **Multi-point calibration** across temperature gradients
- **Temporal correlation** with synchronized data logging
- **Spatial distribution** for wide-area calibration coverage

### **Operational Advantages:**
- **Field-deployable** system for remote calibration
- **Real-time monitoring** during calibration flights
- **Automated data collection** reduces human error
- **Standardized reference** for consistent calibration procedures

---

*This system enables precise thermal-infrared sensor calibration for aerial imaging applications, providing the ground truth temperature data necessary for accurate thermal analysis from drone platforms.*
