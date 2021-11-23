#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



#define INTERVAL_TIME         6000

#define RE_DE                 16
#define RXD_2                 4
#define TXD_2                 5
#define REGISTER_SIZE         10
#define HOUR_ARR_SIZE         60
#define DAY_ARR_SIZE          1440
#define SLAVE_ID              1

#define FIRMWARE_VERSION      1 // 1: test

#define MQTT_SERVER           "mqtt.farota.com"

#if FIRMWARE_VERSION == 1
  // test
  #define SSID                  "iPhone"
  #define PASSWORD              "43100900"
  
  #define SERIAL_NO             "test11" // 4
  #define CONNECT_TOPIC         "farota/wifi/weather/test11/connection"
  #define PING_TOPIC            "farota/wifi/weather/test11/ping"
  #define PUB_TOPIC             "farota/wifi/weather/test11/data"
  
#elif FIRMWARE_VERSION == 2
  
  #define SSID                  "SMARF"
  #define PASSWORD              "43100900"
  
  // 아이들 학교
  #define SERIAL_NO             "394b629963659ce5" // 4
  #define CONNECT_TOPIC         "farota/wifi/weather/394b629963659ce5/connection"
  #define PING_TOPIC            "farota/wifi/weather/394b629963659ce5/ping"
  #define PUB_TOPIC             "farota/wifi/weather/394b629963659ce5/data"
  
#elif FIRMWARE_VERSION == 3

  #define SSID                  "SMARF"
  #define PASSWORD              "43100900"
  
  // 아이들 교체
  #define SERIAL_NO             "9c5e294008393a5b" // 4
  #define CONNECT_TOPIC         "farota/wifi/weather/9c5e294008393a5b/connection"
  #define PING_TOPIC            "farota/wifi/weather/9c5e294008393a5b/ping"
  #define PUB_TOPIC             "farota/wifi/weather/9c5e294008393a5b/data"
#endif



#define MAX_MQTT_BUFFER_SIZE  16384 // mqtt buffer size 설정







WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<500> doc;
ModbusRTU mb;

unsigned long loopMillis = 0;

int idx_cnt = 0;
char message[200];
int count = 0;

typedef struct weather_data {
  float temperature;
  float humidity;
  float rain;
  float hourRain;
  float dayRain;
  uint16_t windDir;
  float windSpeed;
  float radiation;
  uint16_t uv;
} weather_data;
weather_data weatherData;

uint16_t registerDatas[REGISTER_SIZE] = {0,};

float hoursRainArr[HOUR_ARR_SIZE] = {0,};
int hourRainCnt = 0;

float daysRainArr[DAY_ARR_SIZE] = {0,};
int dayRainCnt = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1,RXD_2,TXD_2);
  wifi_setup();
  mqtt_setup();
  modbusSetup();
}

void loop() {
  // it will run the user scheduler as well
  if (!client.connected()) {
    reconnect();
  } else {
    sensorLoop();
    client.loop();
  }
}
