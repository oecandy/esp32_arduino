//ETH setting (tricky!! Don't switch the order between libraries)
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12

#include <ETH.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SENSOR_SERIAL "b1c01a29ab356583"

#define RE_DE 32
#define register_size 10
#define SLAVE_ID 1

#define MAX_MQTT_BUFFER_SIZE 16384 // mqtt buffer size 설정

#define INTERVAL_TIME   6000

const char* mqtt_server = "mqtt.farota.com";

// topic list
const char* air_topic_prefix = "farota/modbus/air/";

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<1000> doc;
ModbusRTU mb;

unsigned long intervalMillis = 0;

static bool ethConnected = false;

int idx_cnt = 0;
char message[200];
int count = 0;

uint16_t air_reg_vals[register_size] = {0,};

typedef struct air_sensor {
  int temp;
  int humi;
} soil_sensor;

air_sensor air_sensor;

int reset_cnt = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1,2,4);
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  modbus_setup();
  mqtt_setup();
}

void loop() {
  if (ethConnected) {
    if (!client.connected()) {
      reconnect();
    } else {
      unsigned long currentMillis = millis();
      
      if (currentMillis - intervalMillis >= INTERVAL_TIME) {
          intervalMillis = currentMillis;
          send_sensor_data();
      }
      delay(100);
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
