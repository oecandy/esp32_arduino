#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SLAVE_ID 1
#define RE_DE 19
#define product_type 2
#define protocol_version 10
#define device_count 24
#define register_size 600

// Update these with values suitable for your network.

const char* ssid = "SMARF";
const char* password = "43100900";
const char* mqtt_server = "lora.farota.com";

// topic list
const char* pub_topic = "farota/modbus/slave/978557fe3ae4917978557fe3ae4917e/connection";
const char* sub_topic = "farota/modbus/slave/978557fe3ae4917978557fe3ae4917e/#";
const char* data_topic = "farota/modbus/slave/978557fe3ae4917978557fe3ae4917e/data"; // 전제 데이터 mqtt message 데이터 저장 => 리셋시 데이터 백업 용도.mqtt message 데이터 저장 => 리셋시 데이터 백업 용도.
const char* weather_topic = "farota/modbus/slave/978557fe3ae4917978557fe3ae4917e/weather"; // 기상 sensor mqtt message 데이터 저장 => 리셋시 데이터 백업 용도.
const char* sensor_topic = "farota/modbus/slave/978557fe3ae4917978557fe3ae4917e/sensor"; // 각종 sensor mqtt message 데이터 저장 => 리셋시 데이터 백업 용도.
const char* soil_topic = "farota/modbus/slave/978557fe3ae4917978557fe3ae4917e/soil"; // soil sensor mqtt message 데이터 저장 => 리셋시 데이터 백업 용도.
// const char* switch_topic = "modbus/slave/switch"; // switch mqtt message 데이터 저장 => 리셋시 데이터 백업 용도.
const char* device_command_topic = "booth/eth/OpenMQTTGateway_ESP32_OLM_GTWE/cmd/MQTTto433"; // 장치 동작 커맨 토픽

WiFiClient espClient;
PubSubClient client(espClient);
DynamicJsonBuffer jsonBuffer;
ModbusRTU mb;

// address array {0,0,1,0,10,N} => 기관코드 0 회사코드 0 제품 타입 1(센서노드) 제품코드 0 프로토콜 버전 10 연결 장비수 N
uint16_t switch_status[2] = {}; // 스위치 상태 0 꺼짐 201 켜짐
uint16_t modbus_array[register_size] = {}; // 100~139(address 101 ~ 140) 까지 장비 코드 201~321 까지 센서 데이터 (3byte 씩 구분하여 저장 1byte 노드 상태 2byte 데이터)
// uint16_t data_convert_array[2] = {}; // data 배열 float 값을 byte array로 변환하여 리턴함. 0 1 순서로 저장하면 됨
// 제품타입 센서 노드 1, 구동기 노드 2
// 데이터 상태 값 정의 0 정상/OFF상태, 1 error 상태, 101 (센서) 센서 교체 요망, 102 (센서) 센서 교정 요망, 201 (스위치형 구동기)작동중(ON), 301(개폐기형 구동기) 여는중, 302(개폐기형 구동기) 닫는중

int idx;
char message[200];
int count = 0;

// valve control message
char valve_close_cmd[200] = "{\"value\":1001000,\"protocol\":1}";
char valve_open_cmd[200] = "{\"value\":1001111,\"protocol\":1}";

// pump control message
char pump_close_cmd[200] = "{\"value\":1002000,\"protocol\":1}";
char pump_open_cmd[200] = "{\"value\":1002111,\"protocol\":1}";

// fan control message
char fan_close_cmd[200] = "{\"value\":1003000,\"protocol\":1}";
char fan_open_cmd[200] = "{\"value\":1003111,\"protocol\":1}";





void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(count < 5){
      count ++;
    } else {
      count= 0;
      WiFi.reconnect();
    }
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println(topic);
  JsonObject& root = jsonBuffer.parse((char*)payload);
  if(root.success()){
  } else {
    Serial.println("parseObject() failed");
  }
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESPMODBUS-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(pub_topic, "connected");
      // ... and resubscribe
      client.subscribe(sub_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setModbusMetadata(){
  // 기관 정보 및 센서 정보 메타 데이터
  modbus_array[0]= 0;
  mb.Hreg(0, modbus_array[0]);
  modbus_array[1]= 0;
  mb.Hreg(1, modbus_array[1]);
  modbus_array[2]= product_type;
  mb.Hreg(2, modbus_array[2]);
  modbus_array[3]= 0;
  mb.Hreg(3, modbus_array[3]);
  modbus_array[4]= protocol_version;
  mb.Hreg(4, modbus_array[4]);
  modbus_array[5]= device_count;
  mb.Hreg(5, modbus_array[5]);

  //센서 분류 메타데이터 Addr : 101~ 140 까지 132부터는 예비 이므로 작성하지 않음
  modbus_array[100]= 102; // 스위치1
  mb.Hreg(100, modbus_array[100]);
  modbus_array[101]= 102; // 스위치2
  mb.Hreg(101, modbus_array[101]);
  modbus_array[102]= 102; // 스위치3
  mb.Hreg(102, modbus_array[102]);
  modbus_array[103]= 102; // 스위치4
  mb.Hreg(103, modbus_array[103]);
  modbus_array[104]= 102; // 스위치5
  mb.Hreg(104, modbus_array[104]);
  modbus_array[105]= 102; // 스위치6
  mb.Hreg(105, modbus_array[105]);
  modbus_array[106]= 102; // 스위치7
  mb.Hreg(106, modbus_array[106]);
  modbus_array[107]= 102; // 스위치8
  mb.Hreg(107, modbus_array[107]);
  modbus_array[108]= 102; // 스위치9
  mb.Hreg(108, modbus_array[108]);
  modbus_array[109]= 102; // 스위치10
  mb.Hreg(109, modbus_array[109]);
  modbus_array[110]= 102; // 스위치11
  mb.Hreg(110, modbus_array[110]);
  modbus_array[111]= 102; // 스위치12
  mb.Hreg(111, modbus_array[111]);
  modbus_array[112]= 102; // 스위치13
  mb.Hreg(112, modbus_array[112]);
  modbus_array[113]= 102; // 스위치14
  mb.Hreg(113, modbus_array[113]);
  modbus_array[114]= 102; // 스위치15
  mb.Hreg(114, modbus_array[114]);
  modbus_array[115]= 102; // 스위치16
  mb.Hreg(115, modbus_array[115]);
  modbus_array[116]= 112; // 개폐기1
  mb.Hreg(116, modbus_array[116]);
  modbus_array[117]= 112; // 개폐기2
  mb.Hreg(117, modbus_array[117]);
  modbus_array[118]= 112; // 개폐기3
  mb.Hreg(118, modbus_array[118]);
  modbus_array[119]= 112; // 개폐기4
  mb.Hreg(119, modbus_array[119]);
  modbus_array[120]= 112; // 개폐기5
  mb.Hreg(120, modbus_array[120]);
  modbus_array[121]= 112; // 개폐기6
  mb.Hreg(121, modbus_array[121]);
  modbus_array[122]= 112; // 개폐기7
  mb.Hreg(122, modbus_array[122]);
  modbus_array[123]= 112; // 개폐기8
  mb.Hreg(123, modbus_array[123]);

  
}

void setModbusAddress(){
  for(int i=0; i < register_size; i ++){
    mb.addHreg(i);
    mb.Hreg(i, 0);
  }
}

void loop_modbus(){
  mb.task();
  yield();
  // 밸브 장치제어 커맨드 전송
  if(mb.Hreg(203) == 201){
    client.publish(device_command_topic,valve_open_cmd);
    switch_status[0] = 1;
  }else if(mb.Hreg(203) == 0){
    client.publish(device_command_topic,valve_close_cmd);
    switch_status[0] = 0;
  }
  // 펌프 장치제어 커맨드 전송
  if(mb.Hreg(207) == 201 && switch_status[1] == 0){
    client.publish(device_command_topic,pump_open_cmd);
    switch_status[1] = 1;
  }else if(mb.Hreg(207) == 0 && switch_status[1] == 1){
    client.publish(device_command_topic,pump_close_cmd);
    switch_status[1] = 0;
  }
  // 유동 팬 장치제어 커맨드 전송
  if(mb.Hreg(211) == 201 && switch_status[1] == 0){
    client.publish(device_command_topic,pump_open_cmd);
    switch_status[1] = 1;
  }else if(mb.Hreg(211) == 0 && switch_status[1] == 1){
    client.publish(device_command_topic,pump_close_cmd);
    switch_status[1] = 0;
  }
  delay(1000);
  
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  mb.begin(&Serial2,RE_DE);;
  mb.slave(SLAVE_ID);
  setModbusAddress();
  setModbusMetadata();
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  count++;
  loop_modbus();
}
