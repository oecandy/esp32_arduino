#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define RE_DE 16
#define RXD_2                 4
#define TXD_2                 5
#define REGISTER_SIZE         10
#define SLAVE_ID              1

#define MQTT_SERVER           "mqtt.farota.com"
#define FIRMWARE_VERSION      1 // 1: test

#define MAX_MQTT_BUFFER_SIZE 16384 // mqtt buffer size 설정

#define INTERVAL_TIME   6000

#if FIRMWARE_VERSION == 1
  #define SSID                  "iPhone"
  #define PASSWORD              "43100900"
  #define SERIAL_NO             "test11" // 4
  #define CONNECT_TOPIC         "farota/wifi/gas/test344/connection"
  #define PING_TOPIC            "farota/wifi/gas/test344/ping"
  #define PUB_TOPIC             "farota/wifi/gas/test344/data"

#endif

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<1000> doc;
ModbusRTU mb;

unsigned long intervalMillis = 0;

static bool ethConnected = false;

int idxCnt = 0;
char message[200];
int count = 0;

uint16_t registerDatas[REGISTER_SIZE] = {0,};

typedef struct gas_sensor {
  float co;       // expand 100times ex) 0024(H)=36(D), CO=0.36ppm
  uint16_t co2;   // original value ex) 049A(H)=1178(D), CO2= 1178ppm
  float o2;       // expand 100 times ex) 0835(H)=2101(D), O2=21.01%VOL
  float h2s;      // expand 100 times ex) 0001(H)=1(D), H2S=0.01ppm
  uint16_t ch4;   // original value ex) 0001(H)=1(D), CH4=1ppm
} gas_sensor;

gas_sensor gasSensor;

int resetCnt = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1,RXD_2,TXD_2);
  wifi_setup();
  mqtt_setup();
  modbus_setup();
}

void loop() {
  if (!client.connected()) {
      if(count < 5){
        reconnect();
        count ++; 
      } else {
        count= 0;
        WiFi.reconnect();
      }
    } else {
      sensor_loop();
      client.loop();
  }
}
