# Thermal-Infrared Drone Calibration Guide

## 🎯 **Overview**

This guide explains how to use the Thermal Ground Control Target (GCT) system to calibrate thermal-infrared sensors on unmanned aerial vehicles (UAVs/drones). The GCT system provides synchronized ground truth temperature measurements that serve as reference data for correcting and validating aerial thermal imagery.

## 🌡️ **Thermal Calibration Principles**

### **Why Calibration is Essential:**

1. **Sensor Drift**: Thermal sensors experience calibration drift over time
2. **Environmental Factors**: Atmospheric conditions affect thermal measurements
3. **Altitude Effects**: Temperature readings vary with flight altitude
4. **Geometric Distortion**: Thermal images require spatial correction
5. **Absolute Temperature**: Converting relative thermal values to actual temperatures
6. **Material Emissivity**: Different surface materials emit thermal radiation differently

### **Calibration Target Design:**

The Thermal GCT system uses four distinct 50cm × 50cm aluminum plates, each designed to provide different thermal signatures for comprehensive calibration:

1. **GCT-1 (Water Tray)**: White aluminum tray filled with water
   - **Surface**: RAL 9016 (Traffic White) powder coating
   - High thermal mass provides temperature stability
   - Water acts as thermal buffer against rapid changes
   - Excellent for baseline temperature reference

2. **GCT-2 (Grey Plate)**: Grey powder-coated aluminum surface
   - **Surface**: RAL 7035 (Light Grey) powder coating
   - Moderate emissivity (~0.85-0.90 for powder coating)
   - Represents intermediate emissivity surfaces
   - Good calibration point between high and low emissivity

3. **GCT-3 (Black Plate)**: Black powder-coated aluminum surface
   - **Surface**: RAL 9005 (Jet Black) powder coating
   - High emissivity (~0.95)
   - Strong solar absorption during daytime
   - Represents high-emissivity natural surfaces

4. **GCT-4 (White Plate)**: White powder-coated aluminum surface
   - **Surface**: RAL 9016 (Traffic White) powder coating
   - Moderate emissivity (~0.85-0.90 for powder coating)
   - High solar reflectance
   - Provides white reference without water thermal mass

**Positioning Strategy**: All four plates are positioned adjacently to ensure they can be captured simultaneously by the UAV thermal sensor, enabling direct comparison of thermal readings across different surface properties.

### **Ground Truth Reference:**
- **Known Temperature Sources**: DS18B20 sensors provide ±0.5°C accuracy
- **Physical Targets**: 50cm x 50cm aluminum plates with different thermal properties:
  - **GCT-1**: White tray (RAL 9016) filled with water (high thermal mass, stable temperature)
  - **GCT-2**: Grey plate (RAL 7035) powder coating (moderate emissivity)
  - **GCT-3**: Black plate (RAL 9005) powder coating (high emissivity, solar absorption)
  - **GCT-4**: White plate (RAL 9016) powder coating (moderate emissivity, high reflectance)
- **Spatial Distribution**: Multiple GCT units positioned adjacently for UAV thermal sensor coverage
- **Temporal Synchronization**: Timestamped data correlates with flight logs
- **Environmental Context**: Local temperature conditions during imaging

## 🚁 **Calibration Workflow**

### **Pre-Flight Preparation:**

1. **Site Survey:**
   - Select calibration area with varied thermal signatures
   - Identify thermal features (hot/cold zones, materials, shading)
   - Plan GCT placement for optimal coverage
   - Consider sun angle and shadow patterns

2. **GCT Deployment:**
   ```
   Deployment Pattern Example:
   
   [Water] [Grey] [Black] [White]
   GCT-1 ─ GCT-2 ─ GCT-3 ─ GCT-4
     │       │       │       │
     │    50cm × 50cm Aluminum Plates    │
     │                                   │
     └─── Adjacent positioning for ──────┘
          simultaneous UAV coverage
   
   - Target Size: 50cm × 50cm aluminum plates
   - Arrangement: Adjacent placement for single-pass imaging
   - Coverage: All targets visible in single UAV thermal image
   - Accessibility: Easy setup and retrieval after flight
   ```

3. **System Initialization:**
   - Power on all GCT units (master + servants)
   - Verify connectivity on master display
   - Confirm temperature sensor readings are reasonable
   - Check SD card availability and file creation

### **Flight Operations:**

1. **Pre-Flight Checks:**
   ```
   Master Display Status:
   ┌────────────────────┐
   │ 12:34:56 01/30/25  │ ← Time sync verification
   │ S1=OK S2=OK S3=OK  │ ← All servants connected
   │ 24.1 24.3 24.2 -   │ ← Temperature readings
   │ Idle (ready to log) │ ← Ready for activation
   └────────────────────┘
   ```

2. **Logging Activation:**
   - Press button on master unit to start logging
   - Verify LED changes to solid green (logging active)
   - Confirm display shows "Logging: X seconds"
   - All servants automatically begin synchronized logging

3. **Flight Execution:**
   - Execute planned thermal imaging flight
   - Maintain adequate altitude for GCT visibility
   - Ensure thermal camera captures GCT locations
   - Record flight log with GPS coordinates and timestamps

4. **Data Collection:**
   - GCT system logs every 10 seconds (configurable)
   - Continue logging throughout entire flight
   - Press button again to stop logging when flight complete

### **Post-Flight Processing:**

1. **Data Retrieval:**
   ```
   GCT Data Files:
   - Master: /data_master.csv (all servants' data)
   - Servant 1: /data_GCT1.csv (local backup)
   - Servant 2: /data_GCT2.csv (local backup)
   - Servant 3: /data_GCT3.csv (local backup)
   ```

2. **Data Format:**
   ```csv
   timestamp,gct_id,sensor_no,temperature
   2025-07-30 14:23:10,1,1,24.37
   2025-07-30 14:23:10,1,2,24.44
   2025-07-30 14:23:10,1,3,24.63
   2025-07-30 14:23:10,2,1,26.12
   2025-07-30 14:23:10,2,2,26.31
   ```

## 📊 **Calibration Data Processing**

### **Temperature Data Analysis:**

1. **Data Validation:**
   - Remove invalid readings (NAN, -999 values)
   - Check for sensor malfunctions or disconnections
   - Verify timestamp continuity and synchronization
   - Identify outliers or anomalous readings

2. **Spatial Interpolation:**
   - Map GCT sensor locations to image coordinates
   - Interpolate temperature values across imaging area
   - Create temperature reference grid for calibration
   - Account for elevation and terrain variations

3. **Temporal Correlation:**
   ```python
   # Example correlation process:
   # 1. Match flight timestamps with GCT data
   flight_time = "2025-07-30 14:23:15"
   gct_window = gct_data[
       (gct_data.timestamp >= flight_time - 5) &
       (gct_data.timestamp <= flight_time + 5)
   ]
   
   # 2. Average temperature readings during image capture
   reference_temp = gct_window.groupby('gct_id').temperature.mean()
   ```

### **Thermal Image Calibration:**

1. **Pixel-to-Temperature Mapping:**
   - Identify GCT locations in thermal images
   - Extract thermal values at GCT pixel coordinates
   - Compare measured vs. actual temperatures
   - Calculate calibration coefficients

2. **Calibration Equation:**
   ```
   T_actual = f(DN_thermal, T_reference, emissivity, environmental_factors)
   
   Multi-Point Calibration:
   - Water Tray (GCT-1): RAL 9016 with water - stable reference temperature
   - Grey Plate (GCT-2): RAL 7035 - moderate emissivity baseline  
   - Black Plate (GCT-3): RAL 9005 - high emissivity, solar heating
   - White Plate (GCT-4): RAL 9016 - moderate emissivity, reflective
   
   Regression Model:
   T_corrected = a₀ + a₁*DN + a₂*T_ambient + a₃*emissivity + a₄*altitude
   
   Where:
   - T_corrected: Calibrated temperature for target of interest
   - DN: Digital number from UAV thermal sensor
   - T_ambient: Ambient temperature from GCT reference
   - emissivity: Estimated emissivity of target surface
   - altitude: Flight altitude (meters)
   - a₀, a₁, a₂, a₃, a₄: Regression coefficients derived from GCT data
   ```

3. **Validation:**
   - Apply calibration to reference points
   - Calculate root mean square error (RMSE)
   - Verify accuracy across temperature ranges
   - Test calibration on independent dataset

## 🎯 **Calibration Scenarios**

### **Agricultural Applications:**

**Plant Canopy Temperature Correction:**
- **Primary Use Case**: Correcting UAV thermal measurements of experimental crop plots
- **Reference Standards**: GCT plates provide known temperature references adjacent to study plots
- **Emissivity Matching**: Different plate colors help establish emissivity relationships for vegetation
- **Environmental Correction**: Water tray provides stable temperature reference unaffected by solar heating

### **Standard Calibration (Single Flight):**
- **Duration**: 10-30 minutes
- **GCT Count**: 4 plates (water, grey, black, white)
- **Coverage**: Single thermal imaging area with all plates visible
- **Purpose**: Basic sensor calibration and emissivity characterization

### **Extended Calibration (Multiple Flights):**
- **Duration**: Multiple flights over days/weeks
- **GCT Count**: 4+ plate sets across different locations
- **Coverage**: Multiple sites and environmental conditions
- **Purpose**: Comprehensive calibration across varying conditions and crop types

### **Real-Time Calibration:**
- **Application**: Continuous crop monitoring missions
- **Method**: Persistent GCT deployment with ongoing correction
- **Benefit**: Dynamic calibration adjustment for changing environmental conditions

## 📋 **Best Practices**

### **Environmental Considerations:**

1. **Weather Conditions:**
   - Avoid extreme temperature gradients during flight
   - Consider wind effects on temperature measurements
   - Account for solar heating and shading patterns
   - Monitor humidity effects on thermal readings

2. **Surface Materials:**
   - **GCT Plate Specifications**: 50cm × 50cm aluminum plates ensure adequate thermal mass
   - **Surface Treatments**: RAL powder coating system for consistent optical properties
     - **RAL 9016 (Traffic White)**: Used for water tray and white reference plate
     - **RAL 7035 (Light Grey)**: Intermediate emissivity reference
     - **RAL 9005 (Jet Black)**: High emissivity and solar absorption
   - **Emissivity Range**: Powder coatings provide emissivity values from 0.85 to 0.95
   - **Thermal Properties**: Water tray provides thermal stability, coated plates respond to environmental changes
   - **Target Positioning**: Adjacent placement ensures all surfaces experience similar environmental conditions
   - **Agricultural Relevance**: Emissivity range closely matches typical vegetation values (0.95-0.98)

### **Data Quality Assurance:**

1. **Redundancy:**
   - Deploy more GCT units than minimum required
   - Use multiple sensors per GCT for averaging
   - Backup data on both master and servant units
   - Record manual temperature readings for validation

2. **Calibration Validation:**
   - Include known temperature references (ice, hot water)
   - Cross-validate between different GCT units
   - Compare with meteorological data when available
   - Perform periodic sensor calibration checks

### **Operational Procedures:**

1. **Setup Checklist:**
   - [ ] All GCT units (4 plates) powered and connected
   - [ ] Temperature readings stabilized (allow 10+ minutes after setup)
   - [ ] SD cards formatted and functional
   - [ ] Master unit shows all servants connected
   - [ ] Flight plan coordinates include GCT plate locations
   - [ ] Plates positioned adjacently for single-image coverage
   - [ ] Water tray filled to appropriate level (if using GCT-1)
   - [ ] Plate surfaces clean and representative of target conditions

2. **During Flight:**
   - [ ] Logging activated before takeoff
   - [ ] Continuous monitoring of GCT status
   - [ ] Flight pattern ensures all 4 plates captured in single thermal image
   - [ ] Thermal camera resolution adequate to distinguish individual plates
   - [ ] Flight altitude appropriate for plate visibility (typically 30-100m AGL)
   - [ ] Emergency procedures for GCT malfunction
   - [ ] Multiple passes over plates for statistical validation

3. **Post-Flight:**
   - [ ] Data retrieved from all units
   - [ ] File integrity verification
   - [ ] Backup data to secure storage
   - [ ] Initial data quality assessment
   - [ ] Equipment inspection and maintenance

## 📈 **Expected Results**

### **Calibration Accuracy:**
- **Temperature Accuracy**: ±1°C after calibration (compared to ±5°C uncalibrated)
- **Spatial Accuracy**: <2 pixel error in thermal-to-spatial registration
- **Temporal Accuracy**: ±30 seconds synchronization between thermal and GCT data
- **Emissivity Correction**: ±0.05 emissivity unit accuracy across different surface types

### **Performance Metrics:**
- **Coverage**: 100% of imaging area covered by adjacent plate arrangement
- **Data Completeness**: >95% successful temperature measurements
- **Connectivity**: >90% servant connectivity during flight operations
- **Reliability**: <5% data loss or corruption rate
- **Plate Temperature Range**: Typically 5-15°C difference between black and white plates

## 🌾 **Agricultural Applications**

### **Plant Canopy Temperature Correction:**

The primary application of this thermal calibration system is correcting UAV-based thermal measurements of experimental agricultural plots. The four-plate GCT system enables accurate temperature measurement of plant canopies for agricultural research and precision farming applications.

**Calibration Workflow for Crop Studies:**

1. **Pre-Flight Setup:**
   - Position GCT plates adjacent to experimental crop plots
   - Ensure plates experience similar environmental conditions as crops
   - Allow thermal equilibration (15-30 minutes)
   - Verify all plates visible in planned flight path

2. **Flight Execution:**
   - Capture thermal images including both GCT plates and crop plots
   - Maintain consistent flight altitude and camera settings
   - Multiple passes for statistical validation
   - Document environmental conditions (wind, solar radiation, humidity)

3. **Data Processing:**
   ```python
   # Example correction workflow for plant canopy temperatures
   
   # 1. Extract thermal values from GCT plates
   gct_thermal_values = extract_plate_temperatures(thermal_image, plate_locations)
   gct_ground_truth = load_gct_sensor_data(timestamp)
   
   # 2. Establish calibration relationship
   calibration_model = fit_regression(gct_thermal_values, gct_ground_truth, 
                                    emissivity_values=[0.90, 0.88, 0.95, 0.90],  # RAL coating emissivities
                                    surface_types=['water_tray', 'grey_ral7035', 'black_ral9005', 'white_ral9016'])
   
   # 3. Apply correction to crop plot temperatures
   crop_thermal_raw = extract_crop_temperatures(thermal_image, plot_boundaries)
   crop_thermal_corrected = calibration_model.predict(crop_thermal_raw, 
                                                     emissivity=0.97)  # Typical vegetation
   ```

4. **Validation and Quality Control:**
   - Compare corrected temperatures with ground-truth measurements
   - Verify calibration stability across different lighting conditions
   - Cross-validate with meteorological data
   - Document correction coefficients for future flights

**Research Applications:**
- **Crop Stress Detection**: Early identification of water stress through canopy temperature elevation
- **Irrigation Management**: Precision irrigation scheduling based on accurate plant temperatures
- **Breeding Programs**: Thermal phenotyping for drought tolerance selection
- **Yield Prediction**: Correlation between canopy temperature patterns and final yield
- **Disease Detection**: Thermal signatures of plant disease progression

**Advantages of Four-Plate System:**
- **Comprehensive Emissivity Coverage**: Brackets vegetation emissivity values
- **Environmental Reference**: Water tray provides stable temperature baseline
- **Real-time Correction**: Immediate calibration during data collection
- **Spatial Consistency**: All references captured in single image frame
- **Reproducibility**: Standardized reference targets across different studies

### **Expected Accuracy for Agricultural Applications:**
- **Canopy Temperature**: ±0.5°C absolute accuracy after calibration
- **Relative Temperature**: ±0.2°C for temperature differences between plots
- **Spatial Resolution**: Sub-meter temperature mapping capability
- **Temporal Stability**: <0.1°C drift per hour under stable conditions

---

*This calibration procedure enables precise thermal-infrared measurements from aerial platforms using the Thermal GCT system as a distributed ground truth reference network.*
