#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTClib.h>
#include <esp_now.h>
#include <WiFi.h>

// Structure to send data, Must match the receiver structure
typedef struct struct_message {
    int actionID;
    float value;
} struct_message;
struct_message TXdata; // Create a struct_message called data


typedef struct temp {
  int actionID;
  float sens1;
  float sens2;
  float sens3;
  float sens4;
  float sens5;
  float sens6;
  float sens7;
  float sens8;
  float sens9;
} temp;
temp tempData; // Create a struct_message called data

//MARK: USER VARIABLES
int GCTID   =       0;

//MARK: PIN DEFINITIONS
#define oneWireBus  4
#define SD_CS       5

int SD_CS_PIN   = 5;    // Chip Select pin //MARK: SPI_PIN
int SCK_PIN     = 18;   // Clock pin
int MISO_PIN    = 19;   // Master In Slave Out pin
int MOSI_PIN    = 23;   // Master Out Slave In pin


//MARK: SYSTEM VARIABLES
//Do not touch these!!!
char filename[25] = "";
#define NUM_SENSORS 9
char timestamp[20];  // Make this a global variable
char line[1000];
#define numMasters 1

uint8_t masterAdress[] = {0x48, 0xE7, 0x29, 0x8C, 0x73, 0x18};
esp_now_peer_info_t peerInfo[numMasters];

RTC_DS3231 rtc;


struct SensorData {
  float temperature;
};

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Create an array of SensorData structures
SensorData sensorData[NUM_SENSORS];


void get_temperature() {
  sensors.requestTemperatures();
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorData[i].temperature = sensors.getTempCByIndex(i);
  }
}

const char* get_timestamp() {
    DateTime now = rtc.now();
    sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
    return timestamp;
}

const char* create_file_chunck() {
  const char* temp_timestamp = get_timestamp();
  get_temperature();

  sprintf(line, "%s,%d,1,%.4f\n%s,%d,2,%.4f\n%s,%d,3,%.4f\n%s,%d,4,%.4f\n%s,%d,5,%.4f\n%s,%d,6,%.4f\n%s,%d,7,%.4f\n%s,%d,8,%.4f\n%s,%d,9,%.4f\n", temp_timestamp, GCTID, sensorData[0].temperature, temp_timestamp, GCTID, sensorData[1].temperature, temp_timestamp, GCTID, sensorData[2].temperature, temp_timestamp, GCTID, sensorData[3].temperature, temp_timestamp, GCTID, sensorData[4].temperature, temp_timestamp, GCTID, sensorData[5].temperature, temp_timestamp, GCTID, sensorData[6].temperature, temp_timestamp, GCTID, sensorData[7].temperature, temp_timestamp, GCTID, sensorData[8].temperature);
  return line;
}

//-------------------MARK: DEBUG-PRINTS-------------------

void print_temperature() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(" temperature: ");
    Serial.println(sensorData[i].temperature);
  }
  Serial.println();
}

void send_data(int actionID, float value){
  TXdata.actionID = actionID;
  TXdata.value = value;
  esp_err_t result = esp_now_send(masterAdress, (uint8_t *) &TXdata, sizeof(TXdata));
  Serial.print("Send: ");
  Serial.print(actionID);
  Serial.print(", ");
  Serial.println(value);
  //delay(1000);
}

void sendTempData(){ //MARK: SEND TEMPERATURE DATA
  get_temperature();
  tempData.actionID = 2001;
  tempData.sens1 = sensorData[0].temperature;
  tempData.sens2 = sensorData[1].temperature;
  tempData.sens3 = sensorData[2].temperature;
  tempData.sens4 = sensorData[3].temperature;
  tempData.sens5 = sensorData[4].temperature;
  tempData.sens6 = sensorData[5].temperature;
  tempData.sens7 = sensorData[6].temperature;
  tempData.sens8 = sensorData[7].temperature;
  tempData.sens9 = sensorData[8].temperature;
  esp_err_t result = esp_now_send(masterAdress, (uint8_t *) &tempData, sizeof(tempData));


  


  // get_temperature();
  // for (int i = 0; i < NUM_SENSORS; i++) {
  //   send_data(2001 + i, sensorData[i].temperature);
  // }

  // send_data(2001, sensorData[0].temperature);
  // send_data(2002, sensorData[1].temperature);
  // send_data(2003, sensorData[2].temperature);
  // send_data(2004, sensorData[3].temperature);
  // send_data(2005, sensorData[4].temperature);
  // send_data(2006, sensorData[5].temperature);
  // send_data(2007, sensorData[6].temperature);
  // send_data(2008, sensorData[7].temperature);
  // send_data(2009, sensorData[8].temperature);
}

void checkActionID(int actionID){
  switch (actionID) {
    case 3001://Full temperature data request
      Serial.println("Full temperature data request");
      sendTempData();
      break;
    case 2:
      Serial.println("Action 2");
      break;
    case 3:
      Serial.println("Action 3");
      break;
    case 4:
      Serial.println("Action 4");
      break;
    case 5:
      Serial.println("Action 5");
      break;
    case 6:
      Serial.println("Action 6");
      break;
    case 7:
      Serial.println("Action 7");
      break;
    case 8:
      Serial.println("Action 8");
      break;
    case 9:
      Serial.println("Action 9");
      break;
    default:
      Serial.println("Action not found");
      break;
  }
}


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
    int receivedData;
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    Serial.print("Received: ");
    Serial.println(receivedData);
    checkActionID(receivedData);
}


void setup() { //MARK: SETUP
  Serial.begin(115200);   // Start the Serial Monitor


  //--------------- ESP NOW - INIT - END -----------------
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv); // Register callbacks

    for (int i = 0; i < numMasters; i++) {
        memcpy(peerInfo[i].peer_addr, masterAdress, 6);
        peerInfo[i].channel = 0;  
        peerInfo[i].encrypt = false;
        
        if (esp_now_add_peer(&peerInfo[i]) != ESP_OK){
            Serial.println("Failed to add peer");
            return;
        }
    }
  //--------------- ESP NOW - INIT - END -----------------


  sensors.begin();        // Start the DS18B20 sensors

  if (! rtc.begin()) {
    Serial.println("Could not find RTC!");
    while (1);
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //uncomment to set the RTC to the compile time

}

void loop(){
  //Serial.println(create_file_chunck());
  //delay(10000);
}