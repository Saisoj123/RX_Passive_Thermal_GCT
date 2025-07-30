# Hardware Setup Guide

## 🔧 **Component Overview**

### **Master Node (TX) Components:**
- **ESP32 NodeMCU-32S** development board
- **20x4 LCD Display** with I2C interface (address 0x27)
- **RTC DS3231** module for precise timekeeping
- **MicroSD card** (formatted FAT32) for data storage
- **NeoPixel RGB LED** for status indication
- **Push button** for logging control
- **5V power supply** (USB or external)

### **Servant Node (RX) Components (per unit):**
- **ESP32 NodeMCU-32S** development board
- **9x DS18B20** temperature sensors (waterproof recommended)
- **RTC DS3231** module for timestamping
- **MicroSD card** (formatted FAT32) for local logging
- **NeoPixel RGB LED** for status indication
- **4.7kΩ resistor** for OneWire bus pull-up
- **5V power supply** (USB or external)

## 📐 **Wiring Diagrams**

### **Master Node (TX) Connections:**

```
ESP32 NodeMCU-32S Master
┌─────────────────────────┐
│                         │
│  GPIO 21 ──────────────┼─── SDA (LCD + RTC)
│  GPIO 22 ──────────────┼─── SCL (LCD + RTC)
│  GPIO 5  ──────────────┼─── SD Card CS
│  GPIO 18 ──────────────┼─── SD Card SCK
│  GPIO 19 ──────────────┼─── SD Card MISO
│  GPIO 23 ──────────────┼─── SD Card MOSI
│  GPIO 2  ──────────────┼─── NeoPixel LED Data
│  GPIO 0  ──────────────┼─── Push Button (INPUT_PULLUP)
│  3.3V ────────────────┼─── VCC (RTC, LCD, LED)
│  GND ──────────────────┼─── GND (All components)
│                         │
└─────────────────────────┘
```

### **Servant Node (RX) Connections:**

```
ESP32 NodeMCU-32S Servant
┌─────────────────────────┐
│                         │
│  GPIO 4  ──────────────┼─── OneWire Bus (DS18B20 sensors)
│  GPIO 21 ──────────────┼─── SDA (RTC)
│  GPIO 22 ──────────────┼─── SCL (RTC)
│  GPIO 5  ──────────────┼─── SD Card CS
│  GPIO 18 ──────────────┼─── SD Card SCK
│  GPIO 19 ──────────────┼─── SD Card MISO
│  GPIO 23 ──────────────┼─── SD Card MOSI
│  GPIO 2  ──────────────┼─── NeoPixel LED Data
│  3.3V ────────────────┼─── VCC (RTC, LED)
│  GND ──────────────────┼─── GND (All components)
│                         │
└─────────────────────────┘
```

### **DS18B20 Temperature Sensor Wiring:**

```
DS18B20 OneWire Bus
┌─────────────┐
│  DS18B20 #1 │
│  ┌─ VDD ────┼─── 3.3V
│  │  DATA ───┼─── GPIO 4 (with 4.7kΩ pull-up to 3.3V)
│  └─ GND ────┼─── GND
└─────────────┘
│
├─── DS18B20 #2 (parallel connection)
├─── DS18B20 #3
├─── ...
└─── DS18B20 #9
```

## 🔌 **Power Requirements**

### **Power Consumption:**
- **ESP32**: ~240mA (during active WiFi), ~80mA (ESP-NOW only)
- **LCD Display**: ~20mA
- **RTC Module**: ~1mA
- **DS18B20 (each)**: ~1.5mA (9 sensors = ~13.5mA total)
- **NeoPixel LED**: ~20mA (white), ~60mA (maximum brightness)
- **SD Card**: ~100mA (during write operations)

### **Recommended Power Supply:**
- **Master Node**: 5V @ 1A minimum (for LCD and display operations)
- **Servant Node**: 5V @ 500mA minimum (for sensor array)
- **Field Deployment**: 12V to 5V buck converter for extended operation
- **Battery Operation**: 18650 Li-ion with boost converter (6-8 hours typical)

## 📦 **Assembly Instructions**

### **Master Node Assembly:**

1. **ESP32 Preparation:**
   - Flash firmware using appropriate USB driver
   - Verify serial communication at 115200 baud

2. **I2C Bus Setup:**
   - Connect SDA (GPIO 21) to LCD and RTC modules
   - Connect SCL (GPIO 22) to LCD and RTC modules
   - Use 4.7kΩ pull-up resistors on SDA/SCL lines if needed

3. **SPI Bus Configuration:**
   - Wire SD card module using SPI pins (5, 18, 19, 23)
   - Ensure reliable connections for data logging

4. **Display Connection:**
   - Connect 20x4 LCD with I2C backpack
   - Verify I2C address (typically 0x27)
   - Test display functionality

5. **Button Integration:**
   - Connect push button between GPIO 0 and GND
   - Use internal pull-up (no external resistor needed)

### **Servant Node Assembly:**

1. **ESP32 Configuration:**
   - Flash firmware with appropriate GCT_ID (1-4)
   - Verify unique device identification

2. **Temperature Sensor Array:**
   - Wire all 9 DS18B20 sensors in parallel on OneWire bus
   - Use 4.7kΩ pull-up resistor from data line to 3.3V
   - Ensure waterproof connections for outdoor deployment

3. **RTC and Storage:**
   - Connect RTC module for local timestamping
   - Install formatted microSD card
   - Verify file write capability

4. **Status Indication:**
   - Wire NeoPixel LED to GPIO 2
   - Position for clear visibility during operation

## 🔧 **Configuration Steps**

### **Software Configuration:**

1. **Environment Setup:**
   ```bash
   # Master firmware upload
   pio run -e tx-master-esp32 --target upload --upload-port COMX
   
   # Servant firmware upload (specify GCT ID)
   pio run -e rx-gct1 --target upload --upload-port COMY  # For GCT ID 1
   pio run -e rx-gct2 --target upload --upload-port COMZ  # For GCT ID 2
   # ... etc for GCT IDs 3 and 4
   ```

2. **Network Configuration:**
   - All devices automatically use WiFi Channel 1
   - ESP-NOW protocol enables mesh communication
   - No additional network setup required

3. **Time Synchronization:**
   - RTC modules should be set to current time
   - Master can sync via NTP (when WiFi enabled)
   - Servants sync with master during operation

### **Field Deployment Configuration:**

1. **Physical Placement:**
   - Master unit: Central location with clear LCD visibility
   - Servant units: Distributed across calibration area
   - Maximum range: ~200m line-of-sight between units

2. **Power Management:**
   - Connect all units to stable power source
   - For battery operation, monitor voltage levels
   - Consider solar charging for extended deployments

3. **Environmental Protection:**
   - Use weatherproof enclosures for outdoor use
   - Ensure temperature sensors remain exposed
   - Protect LCD display from direct sunlight

## ✅ **System Verification**

### **Pre-Deployment Checklist:**

1. **Power-On Test:**
   - [ ] All units boot successfully (yellow LED during init)
   - [ ] LCD displays system information
   - [ ] Serial debug output confirms initialization

2. **Communication Test:**
   - [ ] Master displays servant connection status
   - [ ] Connection count shows expected number
   - [ ] LED indicators show proper status (green when connected)

3. **Sensor Validation:**
   - [ ] All temperature sensors report reasonable values
   - [ ] No "-999" or "NAN" readings from connected sensors
   - [ ] Temperature readings update during system operation

4. **Logging Test:**
   - [ ] Button press activates logging mode
   - [ ] LED changes to solid green during logging
   - [ ] SD cards contain timestamped data files
   - [ ] Data format matches expected CSV structure

5. **Range Test:**
   - [ ] Test communication at maximum deployment distance
   - [ ] Verify stable connection during operation
   - [ ] Check for RF interference sources

### **Troubleshooting Common Issues:**

#### **Connection Problems:**
- Verify all units on same WiFi channel (1)
- Check MAC address configuration
- Ensure adequate power supply
- Test at closer range first

#### **Sensor Issues:**
- Verify OneWire bus wiring and pull-up resistor
- Check sensor power connections
- Test individual sensors with diagnostic code

#### **Display Problems:**
- Confirm I2C address and connections
- Check contrast adjustment
- Verify 5V power supply to LCD

#### **Logging Failures:**
- Format SD cards as FAT32
- Check file permissions
- Verify SPI connections to SD card module

---

*This hardware setup enables reliable thermal reference data collection for thermal-infrared drone calibration across distributed measurement points.*
