#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ETH.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SERIAL_NO             "test"
#define MQTT_SERVER           "mqtt.farota.com"

// topic list
#define CONNECT_TOPIC         "farota/eth/relay_v4/7a8b69caed5792e2/connection"
#define PING_TOPIC            "farota/eth/relay_v4/7a8b69caed5792e2/ping"
#define SUB_TOPIC             "farota/eth/relay_v4/7a8b69caed5792e2/cmd"
#define PUB_TOPIC             "farota/eth/relay_v4/7a8b69caed5792e2/data"

#define RELAY_TYPE 4

#define MAX_MQTT_BUFFER_SIZE  16384 // mqtt buffer size 설정

#define INTERVAL_TIME         6000

#define RELAY_1_PIN           4 // relay pin, GPIO 4
#define RELAY_2_PIN           12 // relay pin, GPIO 5
#define RELAY_3_PIN           13 // relay pin, GPIO 4
#define RELAY_4_PIN           32 // relay pin, GPIO 5

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<500> doc;

static bool eth_connected = false;

int idx_cnt = 0;
char message[200];
int count = 0;

// valve_control setup
int relay_status[RELAY_TYPE];

// FLOW METER VARIABLE
unsigned long intervalMillis = 0;

int reset_cnt = 0;

void setup() {
  Serial.begin(115200);
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  pin_setup();
  mqtt_setup();
}

void loop() {
  if(eth_connected){
    if (!client.connected()) {
      reconnect();
    } else {
      interval_loop();
      client.loop();
    }
  }
}
