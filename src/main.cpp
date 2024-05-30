#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTClib.h>
#include <esp_now.h>
#include <WiFi.h>
#include <SD.h>
#include <Adafruit_NeoPixel.h>


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
char fileName[25] = "/data_GCT1.csv"; //file name for the data file on the SD card
int pingInterval = 1000;

//MARK: PIN DEFINITIONS
#define oneWireBus  4
#define SD_CS       5
#define LED_PIN     2

int SD_CS_PIN   = 5;    // Chip Select pin //MARK: SPI_PIN
int SCK_PIN     = 18;   // Clock pin
int MISO_PIN    = 19;   // Master In Slave Out pin
int MOSI_PIN    = 23;   // Master Out Slave In pin


//MARK: SYSTEM VARIABLES
//Do not touch these!!!
char filename[25] = "";
#define NUM_SENSORS 9
char timestamp[19];  // Make this a global variable
char line[1000];
#define numMasters 1
File file;
unsigned long sinceLastConnection = 0;

Adafruit_NeoPixel strip(1, LED_PIN, NEO_GRB + NEO_KHZ800);  // Create an instance of the Adafruit_NeoPixel class

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


void blinkLED(int red, int green, int blue) {
    static unsigned long previousMillis = 0;
    static bool ledState = false;
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 1000) {
        previousMillis = currentMillis;
        ledState = !ledState;

        if (ledState) {
            strip.setPixelColor(0, strip.Color(red, green, blue));
        } else {
            strip.setPixelColor(0, strip.Color(0, 0, 0));
        }

        strip.show();
    }
}


void updateStatusLED(int status){ //MARK: Update status LED
    switch (status)
    {

    case 0:
        strip.setPixelColor(0, strip.Color(0, 0, 0)); // Turn off the LED
        break;
    case 1:
        strip.setPixelColor(0, strip.Color(255, 100, 0));   // Constant yellow
        break;
    
    case 2:
        blinkLED(0, 255, 0);    // Blink the LED in green
        break;
    
    case 3:
        strip.setPixelColor(0, strip.Color(0, 255, 0)); // Constant green
        break;

    case 4:
        strip.setPixelColor(0, strip.Color(255, 0, 0)); // Constant red
        break;

    case 5:
        blinkLED(255, 0, 0);    // Blink the LED in red
        break;

    case 6:
        blinkLED(255, 100, 0);  // Blink the LED in yellow
        break;

    case 7:
        strip.setPixelColor(0, strip.Color(0, 0, 255)); // Constant blue
        break;
    
    default:
        break;
    }

    strip.show();
}


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


String tempToString(String timestamp) {//MARK: To String
    String data = "";
    data += timestamp + "," + String(GCTID) + ",1," + String(tempData.sens1) + "\n";
    data += timestamp + "," + String(GCTID) + ",2," + String(tempData.sens2) + "\n";
    data += timestamp + "," + String(GCTID) + ",3," + String(tempData.sens3) + "\n";
    data += timestamp + "," + String(GCTID) + ",4," + String(tempData.sens4) + "\n";
    data += timestamp + "," + String(GCTID) + ",5," + String(tempData.sens5) + "\n";
    data += timestamp + "," + String(GCTID) + ",6," + String(tempData.sens6) + "\n";
    data += timestamp + "," + String(GCTID) + ",7," + String(tempData.sens7) + "\n";
    data += timestamp + "," + String(GCTID) + ",8," + String(tempData.sens8) + "\n";
    data += timestamp + "," + String(GCTID) + ",9," + String(tempData.sens9) + "\n";
    return data;
    Serial.println(data);
}


void writeToSD(String dataString) { //MARK: Write to SD
    //Serial.print (dataString);
    file = SD.open(fileName, FILE_APPEND); // Open the file in append mode
    file.print(dataString);
    file.close();
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
    writeToSD(tempToString(get_timestamp()));
    esp_err_t result = esp_now_send(masterAdress, (uint8_t *) &tempData, sizeof(tempData));
}


void checkActionID(int actionID){
  switch (actionID) {
    case 3001://Full temperature data request
      Serial.println("Full temperature data request");
      sendTempData();
      break;

    case 2:
      Serial.println("Dummy Action 2");
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
    sinceLastConnection = millis(); // reset the timer for the last connection
}


void setup() { //MARK: SETUP
  Serial.begin(115200);   // Start the Serial Monitor

  //------------------ NEOPIXEL - INIT - BEGIN ------------------
  strip.begin(); // Initialize the NeoPixel library
  strip.show();  // Initialize all pixels to 'off'
  //------------------ NEOPIXEL - INIT - END ------------------
  updateStatusLED(1); 
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

  //--------------- SD CARD - INIT - START -----------------
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Card Mount Failed");
    return;
  }
  //--------------- SD CARD - INIT - END  ------------------

  //--------------- DS18B20 - INIT - START -----------------
  sensors.begin();        // Start the DS18B20 sensors
  //--------------- DS18B20 - INIT - END -----------------

  //--------------- RTC - INIT - START -----------------
  if (! rtc.begin()) {
    Serial.println("Could not find RTC!");
    while (1);
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //uncomment to set the RTC to the compile time
  //--------------- RTC - INIT - END -----------------
  updateStatusLED(0);
}

void loop(){
  unsigned long currentConection = millis();
  if (currentConection - sinceLastConnection > pingInterval+1000) {
    updateStatusLED(5);
  }else{
      updateStatusLED(2);
  }
  Serial.println(currentConection - sinceLastConnection);
}