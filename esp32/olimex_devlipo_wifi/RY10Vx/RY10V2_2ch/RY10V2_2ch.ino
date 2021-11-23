#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SERIAL_NO             "test"
#define MQTT_SERVER           "mqtt.farota.com"

#define SSID                  "SMARF"
#define PASSWORD              "43100900"

// topic list
#define CONNECT_TOPIC         "farota/wifi/relay/test/connection"
#define PING_TOPIC            "farota/wifi/relay/test/ping"
#define SUB_TOPIC             "farota/wifi/relay/test/cmd"
#define PUB_TOPIC             "farota/wifi/relay/test/data"

#define RELAY_TYPE 2

#define MAX_MQTT_BUFFER_SIZE  16384 // mqtt buffer size 설정

#define INTERVAL_TIME         6000
#define FLOW_METER_1_PIN      18 // flow data pin, GPIO 18
#define FLOW_METER_2_PIN      19 // flow data pin, GPIO 19

#define RELAY_1_PIN           4 // relay pin, GPIO 4
#define RELAY_2_PIN           5 // relay pin, GPIO 5

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
unsigned long flowCount2 = 0;
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
