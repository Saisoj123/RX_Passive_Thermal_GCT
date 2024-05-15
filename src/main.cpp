#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Structure to send data
// Must match the receiver structure
typedef struct struct_message {
    int actionID;
    float value;
} struct_message;

struct_message TXdata; // Create a struct_message called data

uint8_t masterAdress[] = {0x24, 0x0A, 0xC4, 0x0A, 0x0B, 0x24}; //made up address MARK: MAC ADRESS

//MARK: PIN DEFINITIONS
const int oneWireBus = 4;
OneWire oneWire(oneWireBus);        // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature sensor 


//MARK: USER VARIABLES
#define NUM_SENSORS 9   //number of DS18B20 sensors, connected to the oneWireBus

void sendTemperatureReadings() {
    float* readings = getTemperatureReadings();

    for (int i = 0; i < NUM_SENSORS; i++) {
        sendAction(i+2001, readings[i]);
    }
}

void checkActionID(int actionID, float value) { //MARK: ACTION IDs
    switch (actionID) {
        case 8362:
            Serial.print("ActionID: 8362 (Hard Rest)"); //later replace with actual action
            ESP.restart();
            break;

        case 2345:
            Serial.print("ActionID: 2345"); //later replace with actual action
            break;
        
        case 3456:
            Serial.print("ActionID: 3456"); //later replace with actual action
            break;
        
        default:
            Serial.print("Invalid actionID: ");
            Serial.println(actionID);
            break;
    }
}

// Callback when data is sent
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

float* getTemperatureReadings() {
    float readings[NUM_SENSORS];
    sensors.requestTemperatures();  //caution: non-blocking

    for (int i = 0; i < NUM_SENSORS; i++) {
        readings[i] = sensors.getTempCByIndex(i);
    }

    return readings;
}

void sendAction(int actionID, float value) {
    struct_message TXdata;
    TXdata.actionID = actionID;
    TXdata.value = value;

    esp_err_t result = esp_now_send(masterAdress, (uint8_t *) &TXdata, sizeof(TXdata));

    if (result == ESP_OK) {
        Serial.println("Action sent with success");
    }
    else {
        Serial.println("Error sending the action");
    }
}

void setup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);  // Set device as a Wi-Fi Station

    sensors.begin(); // Start up the library for the temperature sensors

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
}



void loop() {



}