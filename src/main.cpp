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

uint8_t masterAdress[] = {0x24, 0x0A, 0xC4, 0x0A, 0x0B, 0x24}; //made up address

// Create a struct_message called data
struct_message TXdata;

void checkActionID(int actionID, float value) {
    switch (actionID) {
        case 1234:
            Serial.print("ActionID: 1234"); //later replace with actual action
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

void setup() {
    Serial.begin(115200);
    delay(1000);    // Delay to give time to Serial Monitor

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataReceived); // Register the receive callback

    // Set the values to send
    TXdata.actionID = 123;
    TXdata.value = 456;

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
}

void loop() {
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(masterAdress, (uint8_t *) &TXdata, sizeof(TXdata));
     
    if (result == ESP_OK) {
        Serial.println("Sent with success");
    }
    else {
        Serial.println("Error sending the TXdata");
    }
    delay(2000);
}