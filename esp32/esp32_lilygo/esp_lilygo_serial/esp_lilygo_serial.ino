#include <ETH.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_MQTT_BUFFER_SIZE 16384 // mqtt buffer size 설정
//#define SERIAL_NO "4c762cb1455fe7a8" //인삼공사
#define SERIAL_NO "4bd0adacb8829f63" // 이민순 대표


// Update these with values suitable for your network.
const char* ssid = "SMARF";
const char* password = "43100900";
const char* mqttServer = "mqtt.farota.com";

// topic list
const char* connTopic = "farota/eth/relay/4bd0adacb8829f63/connection";
const char* pingTopic = "farota/eth/relay/4bd0adacb8829f63/ping";
const char* subTopic = "farota/eth/relay/4bd0adacb8829f63/cmd";
const char* dataTopic = "farota/eth/relay/4bd0adacb8829f63/data";


// Initializations of network clients
WiFiClient espClient;
PubSubClient mqttClient(espClient);
DynamicJsonBuffer jsonBuffer;
static bool ethConnected = false;

bool recvStart = false;
int recvCount = 0;
char message[200];

//command vals
bool commandFlag = false;
char cmdBuf[20];

typedef struct valveStruct {
    int valveNo;
    int relayState;
    int flowCount;
} valveStruct;

valveStruct commandData;

valveStruct valveData;


String resultStr = "";

void setup() {
  Serial.begin(115200);
//  Serial2.begin(115200, SERIAL_8N1,15,14); // lyligo
  Serial2.begin(115200, SERIAL_8N1,32,33); // olimex poe

  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  wifiMqttInit();
}

void loop() {
  if (ethConnected) {
    if (!mqttClient.connected()) {
      reconnect();
    } else {
      mqttClient.loop();
      comunicateSerialData();
      commandSendSerial();
    }
  }
}
