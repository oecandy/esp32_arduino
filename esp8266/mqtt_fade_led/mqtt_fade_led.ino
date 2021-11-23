#include  <ESP8266WiFi.h> // ESP 8285 보드 사용
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Temperature MQTT Topics
#define MQTT_CONN_TOPIC "nbus/led/006/connection"
#define MQTT_SUB_TOPIC "nbus/led/006/cmd"
#define MQTT_PUB_TOPIC "nbus/led/006/data"

#define LED_PIN_1 5 // led1
#define LED_PIN_2 15 // led2
#define BRIGHT_SPEED 10
#define PUB_DELAY 10000

const char* ssid = "SMARF_2G";
const char* password = "43100900";

const char* mqtt_server = "mqtt.farota.com";

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<500> doc;
int count = 0;
int wifiCount = 0;

int brightness1 = 0; // led1 밝기 상수
int brightness2 = 0; // led2 밝기 상수

int led1Switch = 0;
int led2Switch = 0;

void pin_setting(){
  pinMode(LED_PIN_1,OUTPUT);
  pinMode(LED_PIN_2,OUTPUT);
}


void setup() {
  Serial.begin(115200);
  pin_setting();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(led1Switch){
    bright_1();
  } else {
    no_bright_1();
  }

  if(led2Switch){
    bright_2();
  } else {
    no_bright_2();
  }
  delay(BRIGHT_SPEED);
}
