#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define MQTT_SERVER       "mqtt.farota.com"
#define PUB_INTERVAL      6000
#define CHANNEL           4

// MQTT Topics
#define MQTT_CONN_TOPIC   "farota/wifi/led/2002c5ab7db0ca8f/conn"
#define MQTT_SUB_TOPIC    "farota/wifi/led/2002c5ab7db0ca8f/cmd"
#define MQTT_PUB_TOPIC    "farota/wifi/led/2002c5ab7db0ca8f/data"

// Pin Setting
#define LED_1_PIN         4  // D2
#define LED_2_PIN         5  // D1
#define LED_3_PIN         12 // D6
#define LED_4_PIN         13 // D7

// WiFi connection information
#define SSID              "SMARF"
#define PASSWORD          "43100900"


// WiFi, MQTT, Json 라이브러리 셋업
WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<500> doc;

int reboot_cnt = 0;

unsigned long previousMillis = 0;
int relayStatus[CHANNEL] = {0,};

void pin_setup(){
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);
  pinMode(LED_4_PIN, OUTPUT);
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  pin_setup();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  } else {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= PUB_INTERVAL) {
      previousMillis = currentMillis;
      send_payload();
    }
  }
  client.loop();
}
