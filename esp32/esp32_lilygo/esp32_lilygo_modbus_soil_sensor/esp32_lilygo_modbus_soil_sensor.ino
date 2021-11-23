#include <ETH.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ETH_TYPE        ETH_PHY_LAN8720
#define ETH_ADDR        1

#define ETH_CLK_MODE    ETH_CLOCK_GPIO17_OUT
#define ETH_POWER_PIN   16
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18

#define SENSOR_SERIAL_1 "c6e1956af5280913"
#define SENSOR_SERIAL_2 "906e010c48a0adb0"
#define SENSOR_SERIAL_3 "305e6cd0db994117"
#define SENSOR_SERIAL_4 "8be72775167a49eb"
#define SENSOR_SERIAL_5 "b1c01a29ab356583"
//#define SENSOR_SERIAL_1 "test1"
//#define SENSOR_SERIAL_2 "test2"
//#define SENSOR_SERIAL_3 "test3"
//#define SENSOR_SERIAL_4 "test4"

#define RE_DE 4
#define register_size 10
#define SLAVE_ID 1 // 4ch slave id = 1, 8ch slave id = 255

#define MAX_MQTT_BUFFER_SIZE 16384 // mqtt buffer size 설정

#define INTERVAL_TIME   6000

const char* mqtt_server = "mqtt.farota.com";

// topic list
const char* soil_topic_prefix = "farota/modbus/soil/";
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

uint16_t reg_vals[register_size] = {0,};
uint16_t air_reg_vals[register_size] = {0,};

typedef struct soil_sensor {
  int temp;
  int humi;
  int salt;
  int ec;
  int nitro;
  int phosp;
  int potas;
} soil_sensor;

soil_sensor soil_sensor_1_1;
soil_sensor soil_sensor_1_2;
soil_sensor soil_sensor_1_3;
soil_sensor soil_sensor_2_1;
soil_sensor soil_sensor_2_2;
soil_sensor soil_sensor_2_3;
soil_sensor soil_sensor_3_1;
soil_sensor soil_sensor_3_2;
soil_sensor soil_sensor_3_3;
soil_sensor soil_sensor_4_1;
soil_sensor soil_sensor_4_2;
soil_sensor soil_sensor_4_3;
soil_sensor air_sensor;



int sensor_cnt = 1;

int reset_cnt = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1,2,32);
  WiFi.onEvent(WiFiEvent);
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
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
