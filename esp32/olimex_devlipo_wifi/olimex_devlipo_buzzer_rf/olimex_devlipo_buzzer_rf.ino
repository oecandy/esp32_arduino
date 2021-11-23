#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <RCSwitch.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SERIAL_NO             "test"
#define MQTT_SERVER           "mqtt.farota.com"

#define SSID                  "SMARF"
#define PASSWORD              "43100900"

// topic list
#define CONNECT_TOPIC         "farota/wifi/fire/test/connection"
#define PING_TOPIC            "farota/wifi/fire/test/ping"
#define PUB_TOPIC             "farota/wifi/fire/test/data"

#define MAX_MQTT_BUFFER_SIZE  16384 // mqtt buffer size 설정

#define INTERVAL_TIME         6000
#define ALARM_INIT_TIME       10000 // 지나면 알람 상태 0 으로 돌림
#define RF_RX_PIN             22

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<500> doc;

static bool ethConnected = false;

// RC sensor
RCSwitch recvSensor = RCSwitch();

unsigned long codeSensorAlarm = 0000000; //decimal RF433 code

char message[200];
int count = 0;

int sensorState = 0;

// HELTHCHECK VARIABLE
unsigned long pingMillis = 0;
unsigned long sensorMillis = 0;

void setup() {
  Serial.begin(115200);
  wifi_setup();
  mqtt_setup();
  rc_sensor_setup();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  } else {
    ping_loop();
    recive_rc_message();
    client.loop();
  }

}
