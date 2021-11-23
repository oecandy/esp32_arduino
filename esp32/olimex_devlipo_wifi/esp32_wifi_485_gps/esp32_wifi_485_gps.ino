#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



#define INTERVAL_TIME         600000

#define RE_DE                 16
#define RXD_2                 4
#define TXD_2                 5
#define REGISTER_SIZE         8
#define GPS_SLAVE_ID          1

#define FIRMWARE_VERSION      2 // 1: test

#define MQTT_SERVER           "mqtt.farota.com"

#if FIRMWARE_VERSION == 1
  #define SSID                  "SMARF"
  #define PASSWORD              "43100900"
  
  #define SERIAL_NO             "4e5d88c1d93a026b" // 4
  #define CONNECT_TOPIC         "farota/wifi/gps/4e5d88c1d93a026b/connection"
  #define PING_TOPIC            "farota/wifi/gps/4e5d88c1d93a026b/ping"
  #define PUB_TOPIC             "farota/wifi/gps/4e5d88c1d93a026b/data"

#elif FIRMWARE_VERSION == 2 // idle gps 11/17일 업로드
  #define SSID                  "SMARF"
  #define PASSWORD              "43100900"
  
  #define SERIAL_NO             "fa2089655408f7b7"
  #define CONNECT_TOPIC         "farota/wifi/gps/fa2089655408f7b7/connection"
  #define PING_TOPIC            "farota/wifi/gps/fa2089655408f7b7/ping"
  #define PUB_TOPIC             "farota/wifi/gps/fa2089655408f7b7/data"
#endif

#define MAX_MQTT_BUFFER_SIZE  16384 // mqtt buffer size 설정

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<500> doc;
ModbusRTU mb;

unsigned long loopMillis = 0;

int idx_cnt = 0;
char message[200];
int count = 0;

typedef struct gps_data {
  float latitude;
  float longitude;
} gps_data;

gps_data gpsData;

uint16_t registerDatas[REGISTER_SIZE] = {0,};

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1,RXD_2,TXD_2);
  wifi_setup();
  mqtt_setup();
  modbusSetup();
}

void loop() {
  // it will run the user scheduler as well
  if (!client.connected()) {
    reconnect();
  } else {
    sensorLoop();
    client.loop();
  }
}
