#include <WiFi.h>
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12

#include <stdio.h>
#include <string.h>    // strtok 함수가 선언된 헤더 파일

// ETH (POE연결 위한) 라이브러리
#include <ETH.h>
#include <PubSubClient.h>

#include "painlessMesh.h"

#define   MESH_PREFIX       "SKEP-DongSungRoh"
#define   MESH_PASSWORD     "43100900"
#define   MESH_PORT         5555

#define VALVE_2CH_TOPIC     "farota/eth/relay_v3/7db413eaf8645acd/data"
#define VALVE_2CH_SUB_TOPIC "farota/eth/relay_v3/7db413eaf8645acd/cmd"

#define VALVE_2CH_SERIAL_NO "7db413eaf8645acd"

#define CHANNEL 1
#define maxMqttBufferSize 16384

WiFiClient espClient;
PubSubClient mqttClient(espClient);
StaticJsonDocument<500> doc;

static bool eth_connected = false;

// Variables about MQTT connection
const char* mqttServerIp = "mqtt.farota.com"; // IP address of the MQTT broker
const short mqttServerPort = 1883; // IP port of the MQTT broker
const char* mqttClientName = "booth";
const char* mqttUsername = "farotadev";
const char* mqttPassword = "y!j7DD41-w*uQ!^7";

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

int restartCnt = 0;

void setup() {
  Serial.begin(115200);
  initEspMesh();
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  wifiMqttInit();  
}

void loop() {
  // check if ethernet is connected
  if (eth_connected) {
  // now take care of MQTT client...
    mqttClient.loop();
    if (!mqttClient.connected()) {
      reconnect();
    } else {
      // it will run the user scheduler as well
      mesh.update();
      mqttClient.loop();
    }
  }
}
