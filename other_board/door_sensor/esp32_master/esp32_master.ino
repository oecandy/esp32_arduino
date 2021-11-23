#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> // [Json 라이브러리 버전 : ArduinoJson 5]
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ETH_TYPE        ETH_PHY_LAN8720
#define ETH_ADDR        1

#define ETH_CLK_MODE    ETH_CLOCK_GPIO17_OUT
#define ETH_POWER_PIN 16
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18
//espNow 관련 라이브러리 및 전역 설정
#include <esp_now.h>


#include <ETH.h>
// MQTT 라이브러리


uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
esp_now_peer_info_t peerInfo = {};

// Variables about MQTT connection
const char* mqttServerIp = "mqtt.farota.com"; // IP address of the MQTT broker
const short mqttServerPort = 1883; // IP port of the MQTT broker
const char* mqttClientName = "booth";
const char* mqttUsername = "farotadev";
const char* mqttPassword = "y!j7DD41-w*uQ!^7";

// MQTT topics
#define LASER_DATA_TOPIC "farota/eth/laser/5f9a2ab06952271b/data"
#define HALL_DATA_TOPIC "farota/eth/hall_door/750d9c00f98b15eb/data"
#define PING_TOPIC "farota/eth/relay_v3/491c7100c00706c8/ping"
#define RELAY_DATA_TOPIC "farota/eth/relay_v3/491c7100c00706c8/data"
#define SUB_TOPIC "farota/eth/relay_v3/491c7100c00706c8/cmd"

#define RELAY_SERIAL_NO "491c7100c00706c8"
#define CHANNEL 2

#define dataCnt 2048
#define maxMqttBufferSize 16384

WiFiClient espClient;
PubSubClient mqttClient(espClient);
DynamicJsonBuffer jsonBuffer;

static bool eth_connected = false;

typedef struct struct_message {
  int device_no;
  int sensor_type; // 1: hall sensor 2: laser sensor
  int sensor_data;
  int sensor_state;
  int relay_state[CHANNEL];
} struct_message;
struct_message recvData;

typedef struct send_message {
  int valve_no;
  int switch_no;
  int relay_state;
} send_message;
send_message valveData;

void setup() {
  Serial.begin(115200);
  
  // This is the mac address of the Master in Station Mode
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
  // Init ESPNow with a fallback logic
  initESPNow(); //Init 실패 시, 카운팅 or 그냥 재부팅 [현재 재부팅ESP.restart()]
  
  WiFi.onEvent(WiFiEvent);
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
  wifiMqttInit();  
}

void loop() { // 주요 추가, 수정 파트
  // check if ethernet is connected
  if (eth_connected) {
  // now take care of MQTT client...
    mqttClient.loop();
    if (!mqttClient.connected()) {
      reconnect();
    } else {
      mqttClient.loop();
    }
    
  }
}
