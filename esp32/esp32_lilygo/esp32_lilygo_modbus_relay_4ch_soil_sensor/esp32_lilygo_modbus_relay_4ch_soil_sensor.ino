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
#define ETH_POWER_PIN 16
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18

#define SERIAL_NO "57fae99714be9e66"
#define SENSOR_SERIAL_1 "c6e1956af5280913"
#define SENSOR_SERIAL_2 "906e010c48a0adb0"
#define SENSOR_SERIAL_3 "305e6cd0db994117"
#define SENSOR_SERIAL_4 "8be72775167a49eb"

#define RE_DE 16
#define register_size 10
#define RELAY_TYPE 4
#define SLAVE_ID 1 // 4ch slave id = 1, 8ch slave id = 255

#define MAX_MQTT_BUFFER_SIZE 16384 // mqtt buffer size 설정

#define INTERVAL_TIME   6000
#define FLOW_METER_1_PIN  14 // flow data pin, GPIO 15
#define FLOW_METER_2_PIN  15 // flow data pin, GPIO 15
#define FLOW_METER_3_PIN  34 // flow data pin, GPIO 15
#define FLOW_METER_4_PIN  35 // flow data pin, GPIO 15

const char* mqtt_server = "mqtt.farota.com";

// topic list
const char* conn_topic = "farota/modbus/relay/57fae99714be9e66/connection";
const char* ping_topic = "farota/modbus/relay/57fae99714be9e66/ping";
const char* sub_topic = "farota/modbus/relay/57fae99714be9e66/cmd";
const char* data_topic = "farota/modbus/relay/57fae99714be9e66/data";
const char* soil_topic_prefix = "farota/modbus/soil/";

WiFiClient espClient;
PubSubClient client(espClient);
DynamicJsonBuffer jsonBuffer;
ModbusRTU mb;

static bool ethConnected = false;

int idx_cnt = 0;
char message[200];
int count = 0;

// modbus setup
bool coils[4];
bool valve_status[4];
bool command_status[4];

// FLOW METER VARIABLE
unsigned long flowCount1 = 0;
unsigned long flowCount2 = 0;
unsigned long flowCount3 = 0;
unsigned long flowCount4 = 0;
unsigned long flowMillis = 0;

uint16_t reg_vals[register_size] = {0,};

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

int sensor_cnt = 1;

int reset_cnt = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1,2,4);
  WiFi.onEvent(WiFiEvent);
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
  pin_setup();
  flow_setup();
  modbus_setup();
  mqtt_setup();
}

void loop() {
  if (ethConnected) {
    if (!client.connected()) {
      reconnect();
    } else {
      flow_loop();
      delay(100);
      loop_write_coil();
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
