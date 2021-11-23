#include "heltec.h"
#include <ArduinoJson.h>

// lora profiles
#define BAND            915E6  //you can set band here directly,e.g. 868E6,915E6,433E6
#define BUFFER_SIZE     250 // Define the payload size here

// device profiles
#define CHANNEL         1
#define VALVE_NO        5
#define DEVICE_TYPE     1 // 1: 1ch relay, 2: 2ch relay, 3: 6ch relay

#define RELAY_PIN       36 // available pins : 36 37 38 39
#define INTERVAL_TIME   6000


String txpacket;              // outgoing message
long lastSendTime = 0;        // last send time

// reserve buffer for Json structs
StaticJsonDocument<BUFFER_SIZE> doc;
int relayStatus[CHANNEL] = {};
int relayStatusChange = false;
int switchNo = 1;

void setup(){
  loraSetup();   // in [lora]
}

void loop(){
}
