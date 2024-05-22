#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTClib.h>
#include <SD.h>

// Structure to send data
// Must match the receiver structure
typedef struct struct_message {
    int actionID;
    float value;
} struct_message;

struct_message TXdata; // Create a struct_message called data

uint8_t masterAdress[] = {0x24, 0x0A, 0xC4, 0x0A, 0x0B, 0x24}; //made up address MARK: MAC ADRESS

//MARK: USER VARIABLES
int numSens =       9;   //number of DS18B20 sensors, connected to the oneWireBus
int GCTID   =       1;

//MARK: PIN DEFINITIONS
#define oneWireBus  4
#define SD_CS       5

//MARK: SYSTEM VARIABLES
//Do not touch these!!!
char filename[25] = "";

OneWire oneWire(oneWireBus);        // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature sensor 

RTC_DS3231 rtc; // Create a RTC object




void sendAction(int actionID, float value) {
    struct_message TXdata;
    TXdata.actionID = actionID;
    TXdata.value = value;

    esp_err_t result = esp_now_send(masterAdress, (uint8_t *) &TXdata, sizeof(TXdata));

    if (result == ESP_OK) {
        Serial.println("SUCCESS: Action ID: " + String(actionID) + " with value: " + String(value));
    }
    else {
        Serial.println("Error sending the action");
    }
}

float* getTemperatureReadings() {
    float readings[numSens];
    sensors.requestTemperatures();  //caution: non-blocking

    for (int i = 0; i < numSens; i++) {
        readings[i] = sensors.getTempCByIndex(i);
    }

    return readings;
}

void sendTemperatureReadings() {
    float* readings = getTemperatureReadings();

    for (int i = 0; i < numSens; i++) {
        sendAction(i+2001, readings[i]);
    }
}

const char* updateTimeStamp() {
    DateTime now = rtc.now();
    char timestamp[20];
    sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
    return(timestamp);
}

void logToSD(const char* timestamp, float* listOfValues) { //MARK: LOG TO SD
    File dataFile = SD.open(filename, FILE_WRITE);

    if (dataFile) {
        for (int i = 0; i < numSens; i++) {
            dataFile.print(timestamp);          //timestamp
            dataFile.print(",");                //comma
            dataFile.print(GCTID);              //GCTID
            dataFile.print(",");                //comma
            dataFile.print(i);                  //sensorID
            dataFile.print(",");                //comma
            dataFile.print(listOfValues[i]);    //temperature value
        }
        dataFile.println(); //new line
        dataFile.close();
    } else {
        Serial.println("ERROR: opening data on SD card");
    }
        dataFile.close();
}

void checkActionID(int actionID, float value) { //MARK: CHECK ACTION IDs
    switch (actionID) {
        case 8362:
            Serial.println("ActionID: 8362 (Hard Rest)"); //later replace with actual action
            ESP.restart();
            break;

        case 3001:
            Serial.println("ActionID: 3001 (Temperature Request)"); //later replace with actual action
            sendTemperatureReadings();
            logToSD(updateTimeStamp(), getTemperatureReadings());
            break;
        
        case 3456:
            Serial.println("ActionID: 3456"); //later replace with actual action
            break;
        
        default:
            Serial.print("ERROR: Invalid actionID: ");
            Serial.println(actionID);
            break;
    }
}


// Callback when data is sent MARK:CALLBACKS
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback when data is received
void OnDataReceived(const uint8_t *mac_addr, const uint8_t *RXdata_param, int RXdata_len) {
    struct_message local_RXdata;
    memcpy(&local_RXdata, RXdata_param, RXdata_len);

    // Process the received data
    Serial.print("Received actionID: ");
    Serial.println(local_RXdata.actionID);
    Serial.print("Received value: ");
    Serial.println(local_RXdata.value);

    checkActionID(local_RXdata.actionID, local_RXdata.value);
}


void setup() { // MARK: SETUP
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);  // Set device as a Wi-Fi Station

    sensors.begin(); // Start up the library for the temperature sensors

    // Begin RTC Init -----------------------
    Wire.begin();
    if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //uncomment to set the RTC to the compile time
    // End RTC Init -----------------------


    // Begin SD Card Init -----------------------
    if (!SD.begin(SD_CS)) {  // Change this to the correct CS pin!
        Serial.println("SD-Card Initialization failed!");
        return;
    }
    Serial.println("SD-Card Initialization done.");
    // End SD Card Init -----------------------


    // Begin Init ESP-NOW -----------------------
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataReceived); // Register callbacks

    // Peer info
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, masterAdress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;

    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    // End Init ESP-NOW -----------------------

    sprintf(filename, "%s.txt", updateTimeStamp()); //create filename with timestamp of boot
    Serial.print("Filename: "); Serial.println(filename);
}


void loop() { // MARK: LOOP

}