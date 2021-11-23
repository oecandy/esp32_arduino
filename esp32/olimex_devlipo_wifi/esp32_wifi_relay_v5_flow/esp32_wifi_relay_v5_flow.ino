#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define MQTT_SERVER           "mqtt.farota.com"
#define FIRMWARE_VERSION      1 // 1: test

#define CHANNEL               6
#define RELAY_1_PIN           4
#define RELAY_2_PIN           16
#define RELAY_3_PIN           17
#define RELAY_4_PIN           5
#define RELAY_5_PIN           18
#define RELAY_6_PIN           19
#define FLOWMETER_PIN         35

#define INTERVAL_TIME         6000
#define MAX_MQTT_BUFFER_SIZE  16384 // mqtt buffer size 설정

#if FIRMWARE_VERSION == 1
  #define SSID                  "SMARF"
  #define PASSWORD              "43100900"
  #define SERIAL_NO             "6f13909360ea6e76" // 4
  #define CONNECT_TOPIC         "farota/wifi/relay_v5/6f13909360ea6e76/connection"
  #define PING_TOPIC            "farota/wifi/relay_v5/6f13909360ea6e76/ping"
  #define PUB_TOPIC             "farota/wifi/relay_v5/6f13909360ea6e76/data"
  #define SUB_TOPIC             "farota/wifi/relay_v5/6f13909360ea6e76/cmd"

#endif

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<1000> doc;

int idxCnt = 0;
char message[200];
int count = 0;

unsigned long intervalMillis = 0;
unsigned long flowCount = 0;

int relayStatus[CHANNEL] = {0,};

void setup() {
  pin_setup();
  Serial.begin(115200);
  wifi_setup();
  flow_setup();
  mqtt_setup();
}

void loop() {
  if(!client.connected()){
    if(count < 5){
      reconnect();
      count ++; 
    } else {
      count= 0;
      WiFi.reconnect();
    }
  } else {
    interval_loop();
    client.loop();
  }
}
