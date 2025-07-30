# LED Status Reference Guide

## 🚦 **Status LED System**

Each Thermal GCT unit (master and servants) includes a NeoPixel RGB LED that provides visual feedback about system status. This guide explains all LED patterns and their meanings.

## 🎨 **LED Status Codes**

### **Master Node (TX) LED Status**

#### **0 - OFF**
- **Color**: No light
- **Pattern**: Constant off
- **Meaning**: System idle or updating temperature readings
- **Duration**: Temporary during sensor operations

#### **1 - YELLOW (Constant)**
- **Color**: Yellow/Orange (255, 100, 0)
- **Pattern**: Solid, no blinking
- **Meaning**: System initialization or 1-2 servants connected during logging
- **Context**: 
  - During startup self-check
  - Logging active with limited servant connectivity

#### **2 - GREEN (Blinking)**
- **Color**: Green (0, 255, 0)
- **Pattern**: 1-second on/off cycle
- **Meaning**: System ready, servants connected, not logging
- **Context**: Normal operational state when logging is disabled

#### **3 - GREEN (Constant)**
- **Color**: Green (0, 255, 0)
- **Pattern**: Solid, no blinking
- **Meaning**: Logging active with 3+ servants connected
- **Context**: Optimal logging state with full servant connectivity

#### **4 - RED (Constant)**
- **Color**: Red (255, 0, 0)
- **Pattern**: Solid, no blinking
- **Meaning**: Critical system error
- **Context**: 
  - RTC initialization failure
  - WiFi connection timeout (when enabled)
  - System requires restart

#### **5 - RED (Blinking)**
- **Color**: Red (255, 0, 0)
- **Pattern**: 1-second on/off cycle
- **Meaning**: SD card error or storage failure
- **Context**:
  - SD card not detected
  - File write failures
  - Storage system requires attention

#### **6 - YELLOW (Blinking)**
- **Color**: Yellow/Orange (255, 100, 0)
- **Pattern**: 1-second on/off cycle
- **Meaning**: Communication issues or servant connectivity problems
- **Context**:
  - No servants responding
  - WiFi connection lost (when enabled)
  - Network communication degraded

### **Servant Node (RX) LED Status**

#### **0 - OFF**
- **Color**: No light
- **Pattern**: Constant off
- **Meaning**: System ready and operational
- **Context**: Normal state after successful initialization

#### **1 - YELLOW (Constant)**
- **Color**: Yellow/Orange (255, 100, 0)
- **Pattern**: Solid, no blinking
- **Meaning**: System initialization in progress
- **Context**: During startup self-check and sensor initialization

#### **2 - GREEN (Blinking)**
- **Color**: Green (0, 255, 0)
- **Pattern**: 1-second on/off cycle
- **Meaning**: Connected to master, not logging
- **Context**: Normal operational state, ready for data requests

#### **3 - GREEN (Constant)**
- **Color**: Green (0, 255, 0)
- **Pattern**: Solid, no blinking
- **Meaning**: Connected to master and actively logging
- **Context**: Data collection in progress, local SD logging enabled

#### **4 - RED (Constant)**
- **Color**: Red (255, 0, 0)
- **Pattern**: Solid, no blinking
- **Meaning**: Critical system error
- **Context**:
  - RTC initialization failure
  - Sensor system failure
  - System requires restart

#### **5 - RED (Blinking)**
- **Color**: Red (255, 0, 0)
- **Pattern**: 1-second on/off cycle
- **Meaning**: SD card error or storage failure
- **Context**:
  - SD card not detected
  - File write failures
  - Storage system malfunction

#### **6 - YELLOW (Blinking)**
- **Color**: Yellow/Orange (255, 100, 0)
- **Pattern**: 1-second on/off cycle
- **Meaning**: Communication lost with master
- **Context**:
  - Master not responding
  - ESP-NOW communication failure
  - Network connectivity issues

#### **7 - RED (Fast Blinking)**
- **Color**: Red (255, 0, 0)
- **Pattern**: 10 rapid blinks (100ms on/off)
- **Meaning**: System warning (e.g., RTC time invalid)
- **Context**:
  - Clock needs setting
  - Temporary warning condition
  - Returns to normal operation after warning sequence

## 🔍 **LED Troubleshooting Guide**

### **Master Node Issues:**

#### **Constant Red LED (Status 4):**
- **Problem**: Critical system failure
- **Check**: 
  - Serial console for specific error messages
  - RTC module connections (SDA=21, SCL=22)
  - Power supply stability
- **Solution**: 
  - Restart system
  - Check hardware connections
  - Verify RTC module functionality

#### **Blinking Red LED (Status 5):**
- **Problem**: SD card failure
- **Check**:
  - SD card properly inserted
  - SD card not corrupted
  - CS pin connection (pin 5)
- **Solution**:
  - Reinsert SD card
  - Format SD card (FAT32)
  - Check SPI connections

#### **Blinking Yellow LED (Status 6):**
- **Problem**: No servant connectivity
- **Check**:
  - Servant devices powered on
  - ESP-NOW channel configuration
  - MAC address configuration
- **Solution**:
  - Restart servant devices
  - Verify network configuration
  - Check communication range

### **Servant Node Issues:**

#### **Blinking Yellow LED (Status 6):**
- **Problem**: Lost connection to master
- **Check**:
  - Master device operational
  - Distance between master and servant
  - RF interference
- **Solution**:
  - Move closer to master
  - Restart both devices
  - Check for interference sources

#### **Blinking Red LED (Status 5):**
- **Problem**: Local SD card failure
- **Check**:
  - SD card inserted correctly
  - File system not corrupted
- **Solution**:
  - Reformat SD card
  - Replace SD card if persistent

## 📊 **LED Status Summary Table**

| Status | Master Color | Master Pattern | Servant Color | Servant Pattern | Meaning |
|--------|-------------|----------------|---------------|-----------------|---------|
| 0 | OFF | Constant | OFF | Constant | System idle/ready |
| 1 | Yellow | Constant | Yellow | Constant | Initialization |
| 2 | Green | Blinking | Green | Blinking | Ready, not logging |
| 3 | Green | Constant | Green | Constant | Active logging |
| 4 | Red | Constant | Red | Constant | Critical error |
| 5 | Red | Blinking | Red | Blinking | SD card error |
| 6 | Yellow | Blinking | Yellow | Blinking | Communication issues |
| 7 | N/A | N/A | Red | Fast blink | Warning condition |

## 🎯 **Optimal Operation Indicators**

### **Normal System State:**
- **Master**: Green blinking (ready) or green constant (logging)
- **Servants**: Green blinking (ready) or green constant (logging)
- **All units**: Synchronized status changes during logging transitions

### **Warning Signs:**
- **Any red LED**: Requires immediate attention
- **Yellow blinking**: Communication or connectivity issues
- **Mismatched states**: Units not synchronized properly

### **System Health Check:**
1. **Power on all units** - Should show yellow during initialization
2. **Wait for initialization** - Should transition to green
3. **Check master display** - Should show servant connections
4. **Test logging** - All units should show green constant during active logging

---

*LED status indicators provide immediate visual feedback for system health monitoring during thermal-infrared drone calibration operations.*
