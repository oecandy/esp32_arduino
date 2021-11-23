#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Temperature MQTT Topics
#define MQTT_CONN_TOPIC "nbus/dht/001/connection"
#define MQTT_SUB_TOPIC "nbus/dht/001/cmd"
#define MQTT_PUB_TOPIC "nbus/dht/001/data"

// Digital pin connected to the DHT sensor
//#define DHTPIN 5
#define DHTPIN D4

//#define DHTTYPE DHT11
#define DHTTYPE DHT12


#define ARR_CNT 30

// WiFi connection information
const char* ssid = "SMARF";
const char* password = "43100900";

const char* mqtt_server = "mqtt.farota.com";

// WiFi, MQTT, Json 라이브러리 셋업
WiFiClient espClient;
PubSubClient client(espClient);
DynamicJsonBuffer jsonBuffer; // 아두이노 JSON 동적 버퍼 사용

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Variables to hold sensor readings
float temp;
float sumTemp;
int loopCnt = 0;
int count = 0;
int reboot_cnt = 0;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 10000;        // Interval at which to publish sensor readings

void setup() {
  Serial.begin(115200);
  setup_wifi();
  pinMode(DHTPIN, INPUT);
  dht.begin();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  } else {
    if(loopCnt < ARR_CNT){
      get_temp_array_unit();
      loopCnt++;
      delay(120000/ARR_CNT);
    } else {
      send_payload();
      loopCnt = 0;
    }
  }
  client.loop();
}
