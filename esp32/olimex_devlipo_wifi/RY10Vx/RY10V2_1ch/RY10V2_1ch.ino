#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define MQTT_SERVER           "mqtt.farota.com"

#define SSID                  "zlan_test"
#define PASSWORD              "12345"

// 음성 밸브
#define SERIAL_NO             "7fbdf64d5c38a27f"
#define CONNECT_TOPIC         "farota/wifi/relay/1/connection"
#define PING_TOPIC            "farota/wifi/relay/1/ping"
#define SUB_TOPIC             "farota/wifi/relay/1/cmd"
#define PUB_TOPIC             "farota/wifi/relay/1/data"

// 아이들 학교1
//#define SERIAL_NO             "e74800620161643f"
//#define CONNECT_TOPIC         "farota/wifi/relay/e74800620161643f/connection"
//#define PING_TOPIC            "farota/wifi/relay/e74800620161643f/ping"
//#define SUB_TOPIC             "farota/wifi/relay/e74800620161643f/cmd"
//#define PUB_TOPIC             "farota/wifi/relay/e74800620161643f/data"

// 아이들 학교2
//#define SERIAL_NO             "b8b6e9fa4af2a18d"
//#define CONNECT_TOPIC         "farota/wifi/relay/b8b6e9fa4af2a18d/connection"
//#define PING_TOPIC            "farota/wifi/relay/b8b6e9fa4af2a18d/ping"
//#define SUB_TOPIC             "farota/wifi/relay/b8b6e9fa4af2a18d/cmd"
//#define PUB_TOPIC             "farota/wifi/relay/b8b6e9fa4af2a18d/data"

// 아이들 학교3
//#define SERIAL_NO             "05c4af232e8e458b"
//#define CONNECT_TOPIC         "farota/wifi/relay/05c4af232e8e458b/connection"
//#define PING_TOPIC            "farota/wifi/relay/05c4af232e8e458b/ping"
//#define SUB_TOPIC             "farota/wifi/relay/05c4af232e8e458b/cmd"
//#define PUB_TOPIC             "farota/wifi/relay/05c4af232e8e458b/data"

#define RELAY_TYPE 1

#define MAX_MQTT_BUFFER_SIZE  16384 // mqtt buffer size 설정

#define INTERVAL_TIME         6000
#define FLOW_METER_1_PIN      18 // flow data pin, GPIO 18

#define RELAY_1_PIN           4 // relay pin, GPIO 4

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<500> doc;

static bool ethConnected = false;

int idx_cnt = 0;
char message[200];
int count = 0;

// valve_control setup
int relay_status[RELAY_TYPE];

// FLOW METER VARIABLE
unsigned long flowCount1 = 0;
unsigned long flowMillis = 0;

int reset_cnt = 0;

void setup() {
  Serial.begin(115200);
  wifi_setup();
  pin_setup();
  flow_setup();
  mqtt_setup();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  } else {
    flow_loop();
    client.loop();
  }

}
