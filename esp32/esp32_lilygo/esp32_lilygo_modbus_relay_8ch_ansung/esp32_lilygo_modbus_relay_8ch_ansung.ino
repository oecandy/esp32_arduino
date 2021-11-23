#include <ETH.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SERIAL_NO             "3ee7d99b8908e729"

#define RE_DE                 16
#define RXD2                  12
#define TXD2                  14
#define register_size         10
#define RELAY_TYPE            5
#define SLAVE_ID              255 // 4ch slave id = 1, 8ch slave id = 255

#define MAX_MQTT_BUFFER_SIZE  16384 // mqtt buffer size 설정

#define INTERVAL_TIME         6000
#define FLOW_METER_1_PIN      18 // flow data pin, GPIO 15
#define FLOW_METER_2_PIN      19 // flow data pin, GPIO 15
#define FLOW_METER_3_PIN      20 // flow data pin, GPIO 15
#define FLOW_METER_4_PIN      21 // flow data pin, GPIO 15

const char* mqtt_server = "mqtt.farota.com";

// topic list
// 안성s
const char* conn_topic = "farota/modbus/relay_v1/57fae99714be9e66/connection";
const char* ping_topic = "farota/modbus/relay_v1/57fae99714be9e66/ping";
const char* sub_topic = "farota/modbus/relay_v1/57fae99714be9e66/cmd";
const char* data_topic = "farota/modbus/relay_v1/57fae99714be9e66/data";

// 오창a
//const char* conn_topic = "farota/modbus/relay_v1/57fae99714be9e66/connection";
//const char* ping_topic = "farota/modbus/relay_v1/57fae99714be9e66/ping";
//const char* sub_topic = "farota/modbus/relay_v1/57fae99714be9e66/cmd";
//const char* data_topic = "farota/modbus/relay_v1/57fae99714be9e66/data";

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<1000> doc;
ModbusRTU mb;

static bool ethConnected = false;

int idx_cnt = 0;
char message[200];
int count = 0;

// modbus setup
bool coils[RELAY_TYPE];
bool valve_status[RELAY_TYPE];
bool command_status[RELAY_TYPE];

// FLOW METER VARIABLE
unsigned long flowCount1 = 0;
unsigned long flowCount2 = 0;
unsigned long flowCount3 = 0;
unsigned long flowCount4 = 0;
unsigned long flowMillis = 0;

uint16_t reg_vals[register_size] = {0,};

int reset_cnt = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1,RXD2,TXD2);
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
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
