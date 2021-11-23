#include <esp_now.h>
#include <WiFi.h>

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


#define SLAVE_ID 1
#define RE_DE 32
#define product_type 1
#define protocol_version 10
#define device_count 40
#define register_size 400 // 현재 버퍼 사이즈 락이 걸려있어서 255 가 맥스로 설정되어 있음 이 부분을 해결할 방법이 필요함.

// Update these with values suitable for your network.

const char* ssid = "SMARF";
const char* password = "43100900";
const char* mqtt_server = "mqtt.farota.com";

// topic list
const char* conn_topic = "farota/modbus/slave//connection";
const char* sub_topic = "farota/modbus/slave/#";
const char* weather_topic = "farota/modbus/slave/7274e03946b51fcb/weather";
const char* ph_topic = "farota/modbus/slave/pHSensor/soil"; 
const char* soil_topic = "farota/modbus/slave/C47C8D6643AE/soil"; 
const char* ec_topic = "farota/modbus/slave/ecSensor/soil";
const char* pf_topic = "farota/modbus/slave/pFSensor/soil";

// Initializations of network clients
WiFiClient espClient;
PubSubClient mqttClient(espClient);
static bool eth_connected = false;
uint64_t chipid; //chip id based on MAC address
DynamicJsonBuffer jsonBuffer;
ModbusRTU mb;
// address array {0,0,1,0,10,N} => 기관코드 0 회사코드 0 제품 타입 1(센서노드) 제품코드 0 프로토콜 버전 10 연결 장비수 N
// uint16_t switch_status[2] = {}; // 스위치 상태 0 꺼짐 1 켜짐
uint16_t modbus_array[register_size] = {}; // 100~139(address 101 ~ 140) 까지 장비 코드 201~321 까지 센서 데이터 (3byte 씩 구분하여 저장 1byte 노드 상태 2byte 데이터)
uint16_t mqtt_array[register_size] = {};
uint16_t data_convert_array[2] = {}; // data 배열 float 값을 byte array로 변환하여 리턴함. 0 1 순서로 저장하면 됨
// 제품타입 센서 노드 1, 구동기 노드 2
// 데이터 상태 값 정의 0 정상/OFF상태, 1 error 상태, 101 (센서) 센서 교체 요망, 102 (센서) 센서 교정 요망, 201 (스위치형 구동기)작동중(ON), 301(개폐기형 구동기) 여는중, 302(개폐기형 구동기) 닫는중

int idx;
char message[200];
int count = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1,2,4);
  mb.begin(&Serial2,RE_DE);
  mb.slave(SLAVE_ID);
  espNowInit();
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  wifiMqttInit();
  
  setModbusAddress();
  setModbusMetadata();
  setMqttMetadata();
}

void loop() {
  if (eth_connected) {
    if (!mqttClient.connected()) {
      reconnect();
    } else {
    mqttClient.loop();
    count++;
    }
  }
  loop_modbus();
}
