#include <ETH.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ETH_TYPE        ETH_PHY_LAN8720
#define ETH_ADDR        1

#define ETH_CLK_MODE    ETH_CLOCK_GPIO0_IN
#define ETH_POWER_PIN   16
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18

#define SERIAL_NO             "e2804d094054df6d"
#define MQTT_SERVER           "mqtt.farota.com"

// topic list
#define CONNECT_TOPIC         "farota/eth/relay_v4/e2804d094054df6d/connection"
#define PING_TOPIC            "farota/eth/relay_v4/e2804d094054df6d/ping"
#define SUB_TOPIC             "farota/eth/relay_v4/e2804d094054df6d/cmd"
#define PUB_TOPIC             "farota/eth/relay_v4/e2804d094054df6d/data"

#define RELAY_TYPE 4

#define MAX_MQTT_BUFFER_SIZE  16384 // mqtt buffer size 설정

#define INTERVAL_TIME         6000
#define FLOW_METER_1_PIN      36 // flow data pin, GPIO 36
#define FLOW_METER_2_PIN      39 // flow data pin, GPIO 39
#define FLOW_METER_3_PIN      35 // flow data pin, GPIO 35

#define RELAY_1_PIN           2 // relay pin, GPIO 2
#define RELAY_2_PIN           4 // relay pin, GPIO 4
#define RELAY_3_PIN           32 // relay pin, GPIO 32
#define RELAY_4_PIN           33 // relay pin, GPIO 33

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
unsigned long flowCount3 = 0;
unsigned long flowMillis = 0;

int reset_cnt = 0;

void setup() {
  Serial.begin(115200);
  WiFi.onEvent(WiFiEvent);
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
  pin_setup();
  flow_setup();
  mqtt_setup();
}

void loop() {
  if (ethConnected) {
    if (!client.connected()) {
      reconnect();
    } else {
      flow_loop();
      client.loop();
    }
  } else {
    delay(1000);
    reset_cnt++;
    if(reset_cnt == 10){
      reset_cnt = 0;
      ESP.restart();
    }
  }
}
