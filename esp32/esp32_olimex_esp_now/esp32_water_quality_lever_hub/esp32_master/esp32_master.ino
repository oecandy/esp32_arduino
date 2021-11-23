//espNow 관련 라이브러리 및 전역 설정
#include <esp_now.h>
#include <WiFi.h>
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12

// ETH (POE연결 위한) 라이브러리
#include <ETH.h>
// MQTT 라이브러리
#include <PubSubClient.h>
#include <ArduinoJson.h> // [Json 라이브러리 버전 : ArduinoJson 6]

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
esp_now_peer_info_t peerInfo = {};

// Variables about MQTT connection
const char* mqttServerIp = "mqtt.farota.com"; // IP address of the MQTT broker
const short mqttServerPort = 1883; // IP port of the MQTT broker
const char* mqttClientName = "booth";
const char* mqttUsername = "farotadev";
const char* mqttPassword = "y!j7DD41-w*uQ!^7";

// MQTT topics
#define DATA_TOPIC "farota/eth/wqwl/065a7d783bc93fa0/data"
#define PING_TOPIC "farota/eth/wqwl/065a7d783bc93fa0/ping"

#define SERIAL_NO   "065a7d783bc93fa0"

#define dataCnt 2048
#define maxMqttBufferSize 16384

unsigned long mqttMillis = 0;

WiFiClient espClient;
PubSubClient mqttClient(espClient);
StaticJsonDocument<1000> doc;

static bool eth_connected = false;

typedef struct struct_message {
  int device_no;
  uint16_t water_level;
  float water_quality;
} struct_message;
struct_message myData;

void setup() {
  Serial.begin(115200);
  
  // This is the mac address of the Master in Station Mode
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
  // Init ESPNow with a fallback logic
  initESPNow(); //Init 실패 시, 카운팅 or 그냥 재부팅 [현재 재부팅ESP.restart()]
  
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
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
