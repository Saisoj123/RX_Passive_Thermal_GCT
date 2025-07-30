# 🔧 Development Workflow Guide

## 🎯 **Overview**

This guide establishes standardized development workflows for the Thermal GCT System, covering everything from initial setup through deployment and maintenance.

## 🚀 **Quick Start Workflow**

### **New Developer Setup:**

1. **Clone Repository:**
   ```powershell
   git clone https://github.com/your-org/Thermal_GCT_System.git
   cd Thermal_GCT_System
   ```

2. **Install PlatformIO:**
   ```powershell
   # Install PlatformIO CLI
   pip install platformio
   
   # Or use VS Code extension
   # Install "PlatformIO IDE" extension in VS Code
   ```

3. **Install Dependencies:**
   ```powershell
   # PlatformIO will automatically install dependencies
   pio pkg install
   ```

4. **Verify Build System:**
   ```powershell
   cd build_scripts
   build_all.bat
   ```

## 🔨 **Development Workflows**

### **Feature Development:**

1. **Create Feature Branch:**
   ```powershell
   git checkout -b feature/new-thermal-algorithm
   ```

2. **Development Cycle:**
   ```powershell
   # Make changes to firmware/master/src/main.cpp or firmware/servant/src/main.cpp
   
   # Build and test
   pio run -e tx-master-esp32
   pio run -e rx-gct1-esp32
   
   # Upload and test on hardware
   cd build_scripts
   upload_master.bat
   upload_servant.bat
   ```

3. **Testing Protocol:**
   - Unit test individual functions
   - Integration test master-servant communication
   - Hardware test with actual temperature sensors
   - Validate thermal calibration accuracy

4. **Documentation Updates:**
   - Update relevant documentation in `documentation/`
   - Add code comments for new functions
   - Update API reference if interfaces changed
   - Test documentation accuracy

### **Bug Fix Workflow:**

1. **Issue Identification:**
   ```powershell
   # Create bug report branch
   git checkout -b bugfix/button-debounce-issue
   ```

2. **Debugging Process:**
   ```powershell
   # Enable debug output
   pio run -e tx-master-esp32 -t upload
   pio device monitor --port COM14 --baud 115200
   
   # Analyze serial output
   # Reproduce bug conditions
   # Identify root cause
   ```

3. **Fix Implementation:**
   - Implement minimal fix addressing root cause
   - Add validation to prevent regression
   - Test fix under various conditions
   - Update documentation if behavior changed

4. **Validation:**
   ```powershell
   # Test fix on master unit
   upload_master.bat
   
   # Test fix on servant units
   upload_servant.bat
   
   # Validate system operation
   # Document fix verification
   ```

### **Release Workflow:**

1. **Pre-Release Checklist:**
   - [ ] All tests pass
   - [ ] Documentation updated
   - [ ] Version numbers incremented
   - [ ] Changelog updated
   - [ ] Hardware compatibility verified

2. **Version Management:**
   ```cpp
   // Update firmware/shared/include/thermal_config.h
   #define THERMAL_GCT_VERSION "2.1.0"
   ```

3. **Release Process:**
   ```powershell
   # Merge to main branch
   git checkout main
   git merge feature/new-thermal-algorithm
   
   # Tag release
   git tag -a v2.1.0 -m "Release v2.1.0: Enhanced thermal algorithms"
   git push origin v2.1.0
   
   # Build release firmware
   build_all.bat
   
   # Package release files
   # Create release notes
   ```

## 🧪 **Testing Procedures**

### **Unit Testing:**

1. **Function-Level Tests:**
   ```cpp
   // Example test for temperature validation
   void test_temperature_validation() {
       assert(isValidTemperature(25.0) == true);
       assert(isValidTemperature(-999.0) == false);
       assert(isValidTemperature(150.0) == false);
   }
   ```

2. **Component Tests:**
   - Test ESP-NOW communication independently
   - Validate temperature sensor readings
   - Check button debounce logic
   - Verify LED status patterns

### **Integration Testing:**

1. **Master-Servant Communication:**
   ```
   Test Scenario: Connection Test
   1. Power on master unit
   2. Power on servant unit
   3. Verify connection established (LED indicators)
   4. Check master display shows servant connected
   5. Validate Action ID 1001 exchange
   ```

2. **System-Level Tests:**
   ```
   Test Scenario: Full Logging Cycle
   1. Deploy master + 3 servant units
   2. Verify all connections established
   3. Press button to start logging
   4. Confirm all units begin logging
   5. Wait 30 seconds
   6. Press button to stop logging
   7. Verify data files created on all units
   8. Validate data integrity and timestamps
   ```

### **Hardware Testing:**

1. **Environmental Tests:**
   - Temperature range: -10°C to +50°C
   - Humidity: 20% to 80% RH
   - Power supply variations: 4.5V to 5.5V
   - Communication range: 0 to 200 meters

2. **Endurance Tests:**
   - Continuous operation: 8+ hours
   - Repeated button cycles: 1000+ presses
   - Power cycling: 100+ cycles
   - Thermal cycling: -5°C to +45°C

## 📊 **Code Quality Standards**

### **Coding Style:**

1. **Naming Conventions:**
   ```cpp
   // Variables: snake_case
   int temperature_sensor_count = 9;
   
   // Functions: camelCase
   bool isValidTemperature(float temp);
   
   // Constants: UPPER_SNAKE_CASE
   #define MAX_SERVANTS 4
   
   // Classes: PascalCase
   class ThermalGCT;
   ```

2. **Documentation Requirements:**
   ```cpp
   /**
    * @brief Validates temperature reading from DS18B20 sensor
    * @param temp Temperature value in degrees Celsius
    * @return true if temperature is within valid range, false otherwise
    * 
    * Valid range is -55°C to +125°C per DS18B20 specifications.
    * Returns false for -999.0 (disconnected sensor indicator).
    */
   bool isValidTemperature(float temp) {
       return (temp >= MIN_VALID_TEMP && temp <= MAX_VALID_TEMP && temp != DISCONNECTED_SENSOR_VALUE);
   }
   ```

3. **Error Handling:**
   ```cpp
   // Always check return values
   if (!sensors.begin()) {
       Serial.println("Error: Failed to initialize temperature sensors");
       return false;
   }
   
   // Use meaningful error messages
   if (esp_now_init() != ESP_OK) {
       Serial.println("Error: ESP-NOW initialization failed");
       Serial.println("Check WiFi configuration and restart device");
       return false;
   }
   ```

### **Performance Guidelines:**

1. **Memory Management:**
   - Avoid dynamic memory allocation in loop()
   - Use stack variables for temporary data
   - Clear arrays before reuse: `memset(data, 0, sizeof(data))`
   - Monitor stack usage with deep call chains

2. **Timing Considerations:**
   - Use millis() for timing, not delay()
   - Implement non-blocking state machines
   - Minimize time in interrupt handlers
   - Batch operations where possible

3. **Communication Efficiency:**
   - Minimize ESP-NOW message frequency
   - Use appropriate data structures
   - Implement timeout handling
   - Batch multiple sensor readings

## 📋 **Maintenance Procedures**

### **Regular Maintenance:**

1. **Monthly Tasks:**
   - [ ] Review system logs for errors
   - [ ] Check temperature sensor calibration
   - [ ] Verify GPS synchronization accuracy
   - [ ] Update firmware if patches available

2. **Quarterly Tasks:**
   - [ ] Full system calibration check
   - [ ] Hardware inspection and cleaning
   - [ ] Battery level assessment
   - [ ] Communication range verification

3. **Annual Tasks:**
   - [ ] Complete sensor calibration
   - [ ] Hardware component replacement assessment
   - [ ] System performance benchmarking
   - [ ] Documentation review and updates

### **Troubleshooting Workflow:**

1. **Issue Identification:**
   ```
   Symptom Analysis:
   - LED patterns indicate system state
   - Serial output provides detailed diagnostics
   - Display shows connection status
   - Data files reveal logging issues
   ```

2. **Diagnostic Procedures:**
   ```powershell
   # Enable verbose debugging
   pio run -e tx-master-esp32 -D DEBUG_LEVEL=5 -t upload
   pio device monitor --port COM14
   
   # Capture diagnostic data
   # Save serial output to file
   # Document environmental conditions
   ```

3. **Common Issues:**
   
   **ESP-NOW Communication Failure:**
   ```
   Symptoms: Servants show disconnected, no data exchange
   Causes: Channel mismatch, MAC address conflicts, range issues
   Solutions: Verify channel configuration, check MAC addresses, reduce distance
   ```
   
   **Temperature Sensor Issues:**
   ```
   Symptoms: -999.0 readings, inconsistent data
   Causes: Wiring problems, sensor failure, power issues
   Solutions: Check connections, replace sensors, verify power supply
   ```
   
   **Button Response Problems:**
   ```
   Symptoms: Button presses not detected, multiple triggers
   Causes: Debounce timing, electrical noise, hardware failure
   Solutions: Adjust debounce parameters, check wiring, replace button
   ```

## 🎯 **Best Practices**

### **Development Best Practices:**

1. **Version Control:**
   - Commit early and often
   - Write descriptive commit messages
   - Use feature branches for development
   - Tag releases appropriately

2. **Documentation:**
   - Update documentation with code changes
   - Include examples in documentation
   - Test documentation procedures
   - Keep API reference current

3. **Testing:**
   - Test on actual hardware regularly
   - Validate under realistic conditions
   - Document test procedures
   - Automate testing where possible

### **Deployment Best Practices:**

1. **Pre-Deployment:**
   - Verify all builds complete successfully
   - Test critical functionality
   - Review configuration parameters
   - Prepare rollback procedures

2. **Deployment Process:**
   - Deploy to test environment first
   - Monitor system behavior closely
   - Document deployment steps
   - Validate post-deployment functionality

3. **Post-Deployment:**
   - Monitor system performance
   - Collect user feedback
   - Address issues promptly
   - Plan next iteration

---

*This workflow guide ensures consistent, high-quality development practices for the Thermal GCT System, supporting reliable thermal-infrared drone calibration operations.*
