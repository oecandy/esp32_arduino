#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ModbusRtu.h>
#include <string.h>

#define re_de_pin 19
#define slave_id 1
#define product_type 1
#define protocol_version 10
#define device_count 40

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
// const char* lora_command_topic_1 = "application/3/device/92a97158c3281603/command/down"; // 스위치 대체 밸브 제어
// const char* lora_command_topic_2 = "application/3/device/92a97158c3281603/command/down"; // 개폐기 대체 밸브 제어
// const char* lora_soil_topic = "modbus/slave/lora_soil"; // {"lora_soil":["지온","지습","EC"]} lora 토양 센서 데이터 수집 토픽

WiFiClient espClient;
PubSubClient client(espClient);
DynamicJsonBuffer jsonBuffer;
Modbus slave;

// address array {0,0,1,0,10,N} => 기관코드 0 회사코드 0 제품 타입 1(센서노드) 제품코드 0 프로토콜 버전 10 연결 장비수 N
// uint16_t switch_status[2] = {}; // 스위치 상태 0 꺼짐 1 켜짐
uint16_t modbus_array[321] = {}; // 100~139(address 101 ~ 140) 까지 장비 코드 201~321 까지 센서 데이터 (3byte 씩 구분하여 저장 1byte 노드 상태 2byte 데이터)
uint16_t mqtt_array[321] = {};
uint16_t data_convert_array[2] = {}; // data 배열 float 값을 byte array로 변환하여 리턴함. 0 1 순서로 저장하면 됨
// 제품타입 센서 노드 1, 구동기 노드 2
// 데이터 상태 값 정의 0 정상/OFF상태, 1 error 상태, 101 (센서) 센서 교체 요망, 102 (센서) 센서 교정 요망, 201 (스위치형 구동기)작동중(ON), 301(개폐기형 구동기) 여는중, 302(개폐기형 구동기) 닫는중

int idx;
char message[200];
int count = 0;

// valve control message
// char valve_close_cmd[200] = "{\"fPort\":1,\"confirmed\":true,\"data\":\"VkFMVkUw\"}";
// char valve_open_cmd[200] = "{\"fPort\":1,\"confirmed\":true,\"data\":\"VkFMVkUx\"}";



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
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void convert_byte(float x){
  long *ptr;
  int *pointer;
  
  ptr = (long*)&x;
  pointer = (int*)&x;
  long m = *ptr;
  Serial.println(m, HEX);  //shows:3FAF5C29
  //---------------------
  int n = *pointer;
  //Serial.println(n, HEX); //shows: 5C29
  pointer++;
  //Serial.println(*pointer, HEX); //shows: 3FAF

  data_convert_array[1] = (uint16_t)*pointer;
  data_convert_array[0] = (uint16_t)n;
}


void set_temp_01(float temp){
  convert_byte(temp);
  modbus_array[204] = 0;
  modbus_array[202] = data_convert_array[0];
  modbus_array[203] = data_convert_array[1];

  mqtt_array[204] = 0;
  mqtt_array[202] = data_convert_array[0];
  mqtt_array[203] = data_convert_array[1];
}

void set_temp_02(float temp){
  convert_byte(temp);
  modbus_array[207] = 0;
  modbus_array[205] = data_convert_array[0];
  modbus_array[206] = data_convert_array[1];

  mqtt_array[207] = 0;
  mqtt_array[205] = data_convert_array[0];
  mqtt_array[206] = data_convert_array[1];
}

void set_temp_03(float temp){
  convert_byte(temp);
  modbus_array[210] = 0;
  modbus_array[208] = data_convert_array[0];
  modbus_array[209] = data_convert_array[1];

  mqtt_array[210] = 0;
  mqtt_array[208] = data_convert_array[0];
  mqtt_array[209] = data_convert_array[1];
}

void set_temp_04(float temp){
  convert_byte(temp);
  modbus_array[261] = 0;
  modbus_array[259] = data_convert_array[0];
  modbus_array[260] = data_convert_array[1];

  mqtt_array[261] = 0;
  mqtt_array[259] = data_convert_array[0];
  mqtt_array[260] = data_convert_array[1];
}

void set_temp_05(float temp){
  convert_byte(temp);
  modbus_array[264] = 0;
  modbus_array[262] = data_convert_array[0];
  modbus_array[263] = data_convert_array[1];

  mqtt_array[264] = 0;
  mqtt_array[262] = data_convert_array[0];
  mqtt_array[263] = data_convert_array[1];
}

void set_temp_06(float temp){
  convert_byte(temp);
  modbus_array[267] = 0;
  modbus_array[265] = data_convert_array[0];
  modbus_array[266] = data_convert_array[1];

  mqtt_array[267] = 0;
  mqtt_array[265] = data_convert_array[0];
  mqtt_array[266] = data_convert_array[1];
}

void set_temp_07(float temp){
  convert_byte(temp);
  modbus_array[270] = 0;
  modbus_array[268] = data_convert_array[0];
  modbus_array[269] = data_convert_array[1];

  mqtt_array[270] = 0;
  mqtt_array[268] = data_convert_array[0];
  mqtt_array[269] = data_convert_array[1];
}

void set_temp_08(float temp){
  convert_byte(temp);
  modbus_array[273] = 0;
  modbus_array[271] = data_convert_array[0];
  modbus_array[272] = data_convert_array[1];

  mqtt_array[273] = 0;
  mqtt_array[271] = data_convert_array[0];
  mqtt_array[272] = data_convert_array[1];
}

void set_temp_09(float temp){
  convert_byte(temp);
  modbus_array[276] = 0;
  modbus_array[274] = data_convert_array[0];
  modbus_array[275] = data_convert_array[1];

  mqtt_array[276] = 0;
  mqtt_array[274] = data_convert_array[0];
  mqtt_array[275] = data_convert_array[1];
}

void set_temp_09(float temp){
  convert_byte(temp);
  modbus_array[279] = 0;
  modbus_array[277] = data_convert_array[0];
  modbus_array[278] = data_convert_array[1];

  mqtt_array[279] = 0;
  mqtt_array[277] = data_convert_array[0];
  mqtt_array[278] = data_convert_array[1];
}

void set_humi_01(float humi){
  convert_byte(humi);
  modbus_array[213] = 0;
  modbus_array[211] = data_convert_array[0];
  modbus_array[212] = data_convert_array[1];

  mqtt_array[213] = 0;
  mqtt_array[211] = data_convert_array[0];
  mqtt_array[212] = data_convert_array[1];
}

void set_humi_02(float humi){
  convert_byte(humi);
  modbus_array[282] = 0;
  modbus_array[280] = data_convert_array[0];
  modbus_array[281] = data_convert_array[1];

  mqtt_array[282] = 0;
  mqtt_array[280] = data_convert_array[0];
  mqtt_array[281] = data_convert_array[1];
}

void set_humi_03(float humi){
  convert_byte(humi);
  modbus_array[285] = 0;
  modbus_array[283] = data_convert_array[0];
  modbus_array[284] = data_convert_array[1];

  mqtt_array[285] = 0;
  mqtt_array[283] = data_convert_array[0];
  mqtt_array[284] = data_convert_array[1];
}

void set_dew_point_sensor_01(float dew_point){
  convert_byte(dew_point);
  modbus_array[216] = 0;
  modbus_array[214] = data_convert_array[0];
  modbus_array[215] = data_convert_array[1];

  mqtt_array[216] = 0;
  mqtt_array[214] = data_convert_array[0];
  mqtt_array[215] = data_convert_array[1];
}

void set_rain_01(float rain){
  convert_byte(rain);
  modbus_array[219] = 0;
  modbus_array[217] = data_convert_array[0];
  modbus_array[218] = data_convert_array[1];

  mqtt_array[219] = 0;
  mqtt_array[217] = data_convert_array[0];
  mqtt_array[218] = data_convert_array[1];
}

void set_flow_01(float flow){
  convert_byte(flow);
  modbus_array[222] = 0;
  modbus_array[220] = data_convert_array[0];
  modbus_array[221] = data_convert_array[1];

  mqtt_array[222] = 0;
  mqtt_array[220] = data_convert_array[0];
  mqtt_array[221] = data_convert_array[1];
}

void set_rain_02(float rain){
  convert_byte(rain);
  modbus_array[225] = 0;
  modbus_array[223] = data_convert_array[0];
  modbus_array[224] = data_convert_array[1];

  mqtt_array[225] = 0;
  mqtt_array[223] = data_convert_array[0];
  mqtt_array[224] = data_convert_array[1];
}

void set_sun_01(float sun){
  convert_byte(sun);
  modbus_array[228] = 0;
  modbus_array[226] = data_convert_array[0];
  modbus_array[227] = data_convert_array[1];

  mqtt_array[228] = 0;
  mqtt_array[226] = data_convert_array[0];
  mqtt_array[227] = data_convert_array[1];
}

void set_wind_speed_01(float wind_speed){
  convert_byte(wind_speed);
  modbus_array[231] = 0;
  modbus_array[229] = data_convert_array[0];
  modbus_array[230] = data_convert_array[1];

  mqtt_array[231] = 0;
  mqtt_array[229] = data_convert_array[0];
  mqtt_array[230] = data_convert_array[1];
}

void set_wind_dir_01(float wind_dir){
  convert_byte(wind_dir);
  modbus_array[234] = 0;
  modbus_array[232] = data_convert_array[0];
  modbus_array[233] = data_convert_array[1];

  mqtt_array[234] = 0;
  mqtt_array[232] = data_convert_array[0];
  mqtt_array[233] = data_convert_array[1];
}

void set_vol_01(float vol){
  convert_byte(volvol);
  modbus_array[237] = 0;
  modbus_array[238] = data_convert_array[0];
  modbus_array[236] = data_convert_array[1];

  mqtt_array[237] = 0;
  mqtt_array[235] = data_convert_array[0];
  mqtt_array[236] = data_convert_array[1];
}

void set_co2_01(float co2){
  convert_byte(co2);
  modbus_array[240] = 0;
  modbus_array[238] = data_convert_array[0];
  modbus_array[239] = data_convert_array[1];

  mqtt_array[240] = 0;
  mqtt_array[238] = data_convert_array[0];
  mqtt_array[239] = data_convert_array[1];
}

void set_co2_01(float co2){
  convert_byte(co2);
  modbus_array[240] = 0;
  modbus_array[238] = data_convert_array[0];
  modbus_array[239] = data_convert_array[1];

  mqtt_array[240] = 0;
  mqtt_array[238] = data_convert_array[0];
  mqtt_array[239] = data_convert_array[1];
}

void set_ec_01(float ec){
  convert_byte(ec);
  modbus_array[243] = 0;
  modbus_array[241] = data_convert_array[0];
  modbus_array[242] = data_convert_array[1];

  mqtt_array[243] = 0;
  mqtt_array[241] = data_convert_array[0];
  mqtt_array[242] = data_convert_array[1];
}

void set_photon_01(float photon){
  convert_byte(photon);
  modbus_array[246] = 0;
  modbus_array[244] = data_convert_array[0];
  modbus_array[245] = data_convert_array[1];

  mqtt_array[246] = 0;
  mqtt_array[244] = data_convert_array[0];
  mqtt_array[245] = data_convert_array[1];
}

void set_soil_moi_01(float soil_moi){
  convert_byte(soil_moi);
  modbus_array[249] = 0;
  modbus_array[247] = data_convert_array[0];
  modbus_array[248] = data_convert_array[1];

  mqtt_array[249] = 0;
  mqtt_array[247] = data_convert_array[0];
  mqtt_array[248] = data_convert_array[1];
}

void set_soil_water_ten_01(float soil_water_ten){
  convert_byte(soil_water_ten);
  modbus_array[252] = 0;
  modbus_array[250] = data_convert_array[0];
  modbus_array[251] = data_convert_array[1];

  mqtt_array[252] = 0;
  mqtt_array[250] = data_convert_array[0];
  mqtt_array[251] = data_convert_array[1];
}

void set_ph_01(float ph){
  convert_byte(ph);
  modbus_array[255] = 0;
  modbus_array[253] = data_convert_array[0];
  modbus_array[254] = data_convert_array[1];

  mqtt_array[255] = 0;
  mqtt_array[253] = data_convert_array[0];
  mqtt_array[254] = data_convert_array[1];
}

void set_soil_temp_01(float soil_temp){
  convert_byte(soil_temp);
  modbus_array[258] = 0;
  modbus_array[257] = data_convert_array[0];
  modbus_array[258] = data_convert_array[1];

  mqtt_array[258] = 0;
  mqtt_array[256] = data_convert_array[0];
  mqtt_array[257] = data_convert_array[1];
}


void set_weight_01(float weight){
  convert_byte(weight);
  modbus_array[288] = 0;
  modbus_array[286] = data_convert_array[0];
  modbus_array[287] = data_convert_array[1];

  mqtt_array[288] = 0;
  mqtt_array[286] = data_convert_array[0];
  mqtt_array[287] = data_convert_array[1];
}

void set_weight_02(float weight){
  convert_byte(weight);
  modbus_array[291] = 0;
  modbus_array[289] = data_convert_array[0];
  modbus_array[290] = data_convert_array[1];

  mqtt_array[291] = 0;
  mqtt_array[289] = data_convert_array[0];
  mqtt_array[290] = data_convert_array[1];
}

void setModbusMetadata(){
  // 기관 정보 및 센서 정보 메타 데이터
  modbus_array[0]= 0;
  modbus_array[1]= 0;
  modbus_array[2]= product_type;
  modbus_array[3]= 0;
  modbus_array[4]= protocol_version;
  modbus_array[5]= device_count;

  //센서 분류 메타데이터 Addr : 101~ 140 까지 132부터는 예비 이므로 작성하지 않음
  modbus_array[100]= 1; // 온도 1
  modbus_array[101]= 1; // 온도 2
  modbus_array[102]= 1; // 온도 3
  modbus_array[103]= 2; // 습도 1
  modbus_array[104]= 3; // 이슬점 센서
  modbus_array[105]= 4; // 감우 센서
  modbus_array[106]= 5; // 유량 센서
  modbus_array[107]= 6; // 강우 센서
  modbus_array[108]= 7; // 일사 센서
  modbus_array[109]= 8; // 풍속 센서
  modbus_array[110]= 9; // 풍향 센서
  modbus_array[111]= 10; // 전압 센서
  modbus_array[112]= 11; // CO2 센서
  modbus_array[113]= 12; // EC 센서
  modbus_array[114]= 13; // 광자양 센서
  modbus_array[115]= 14; // 토양 함수율 센서
  modbus_array[116]= 15; // 토양 수분 장력 센서
  modbus_array[117]= 16; // PH 센서
  modbus_array[118]= 17; // 지온 센서
  modbus_array[119]= 1; // 온도4 센서
  modbus_array[120]= 1; // 온도5 센서
  modbus_array[121]= 1; // 온도6 센서
  modbus_array[122]= 1; // 온도7 센서
  modbus_array[123]= 1; // 온도8 센서
  modbus_array[124]= 1; // 온도9 센서
  modbus_array[125]= 1; // 온도10 센서
  modbus_array[126]= 2; // 습도2 센서
  modbus_array[127]= 2; // 습도3 센서
  modbus_array[128]= 19; // 무게1 센서
  modbus_array[129]= 19; // 무게2 센서

  
}

void setMqttMetadata(){
  mqtt_array[0]= 0; 
  mqtt_array[1]= 0;
  mqtt_array[2]= product_type;
  mqtt_array[3]= 0;
  mqtt_array[4]= protocol_version;
  mqtt_array[5]= device_count;

    //센서 분류 메타데이터 Addr : 101~ 140 까지 131부터는 예비 이므로 작성하지 않음
  mqtt_array[100]= 1; // 온도 1
  mqtt_array[101]= 1; // 온도 2
  mqtt_array[102]= 1; // 온도 3
  mqtt_array[103]= 2; // 습도 1
  mqtt_array[104]= 3; // 이슬점 센서
  mqtt_array[105]= 4; // 감우 센서
  mqtt_array[106]= 5; // 유량 센서
  mqtt_array[107]= 6; // 강우 센서
  mqtt_array[108]= 7; // 일사 센서
  mqtt_array[109]= 8; // 풍속 센서
  mqtt_array[110]= 9; // 풍향 센서
  mqtt_array[111]= 10; // 전압 센서
  mqtt_array[112]= 11; // CO2 센서
  mqtt_array[113]= 12; // EC 센서
  mqtt_array[114]= 13; // 광자양 센서
  mqtt_array[115]= 14; // 토양 함수율 센서
  mqtt_array[116]= 15; // 토양 수분 장력 센서
  mqtt_array[117]= 17; // PH 센서
  mqtt_array[118]= 18; // 지온 센서
  mqtt_array[119]= 1; // 온도4 센서
  mqtt_array[120]= 1; // 온도5 센서
  mqtt_array[121]= 1; // 온도6 센서
  mqtt_array[122]= 1; // 온도7 센서
  mqtt_array[123]= 1; // 온도8 센서
  mqtt_array[124]= 1; // 온도9 센서
  mqtt_array[125]= 1; // 온도10 센서
  mqtt_array[126]= 2; // 습도2 센서
  mqtt_array[127]= 2; // 습도3 센서
  mqtt_array[128]= 19; // 무게1 센서
  mqtt_array[129]= 19; // 무게2 센서
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
//    if (String(topic).equals(String(switch_topic))){
//      mqtt_array[101] = (uint16_t) root["switch"][0];
//      modbus_array[101] = (uint16_t) root["switch"][0];
//      mqtt_array[111] = (uint16_t) root["switch"][1];
//      modbus_array[111] = (uint16_t) root["switch"][1];
//    }else if 
    if (String(topic).equals(String(weather_topic))){
      for(int i = 0; i < 9; i++) {
        if(i != 11 || i != 12 || i != 17){
          idx = (int)i;
          mqtt_array[idx] = (uint16_t) root["weather"][i];
          modbus_array[idx] = (uint16_t) root["weather"][i];
        }
      }
    } else if (String(topic).equals(String(sensor_topic))){
      for(int i = 0; i < 3; i++) {
        if(i != 2){
          idx = (int)(i+9);
          mqtt_array[idx] = (uint16_t) root["sensor"][i];
          modbus_array[idx] = (uint16_t) root["sensor"][i];
        } else {
          idx = (int)(i+16);
          mqtt_array[idx] = (uint16_t) root["sensor"][i];
          modbus_array[idx] = (uint16_t) root["sensor"][i];
        }
      }
    } else if (String(topic).equals(String(soil_topic))){
      for(int i = 11; i < 18; i++) {
        idx = (int)i;
        mqtt_array[idx] = (uint16_t) root["soil"][i-11];
        modbus_array[idx] = (uint16_t) root["soil"][i-11];
      }
    } else if (String(topic).equals(String(data_topic))){
      for(int i = 0; i < root["data"].size(); i++) {
        idx = ((int)root["data"][i]["address"] - 1);
        mqtt_array[idx] = (uint16_t) root["data"][i]["value"];
        modbus_array[idx] = (uint16_t) root["data"][i]["value"];
      }
    } else if(String(topic).equals(String(lora_soil_topic))){
      modbus_array[17] = (uint16_t) root["lora_soil"][0];
      mqtt_array[17] = (uint16_t) root["lora_soil"][0];
      modbus_array[12] = (uint16_t) root["lora_soil"][1];
      mqtt_array[12] = (uint16_t) root["lora_soil"][1];
      modbus_array[11] = (uint16_t) root["lora_soil"][2];
      mqtt_array[11] = (uint16_t) root["lora_soil"][2];
    }
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

void loop_modbus(){
  slave.poll(modbus_array,321);       //Used to receive or write value from Master
//  if(modbus_array[101] == 1 && switch_status[0] == 0){
//    client.publish(lora_command_topic_1,valve_open_cmd);
//    switch_status[0] = 1;
//  }else if(modbus_array[101] == 0 && switch_status[0] == 1){
//    client.publish(lora_command_topic_1,valve_close_cmd);
//    switch_status[0] = 0;
//  }
//  if(modbus_array[111] == 1 && switch_status[1] == 0){
//    client.publish(lora_command_topic_2,valve_open_cmd);
//    switch_status[1] = 1;
//  }else if(modbus_array[111] == 0 && switch_status[1] == 1){
//    client.publish(lora_command_topic_2,valve_close_cmd);
//    switch_status[1] = 0;
//  }
  delay(1000);
//  for(int i = 0; i < 19; i++){
//    String message = "mqtt_array[" + String(i) + "] : ";
//    Serial.print(message);
//    Serial.println(mqtt_array[i]);
//  }
  
}

void publish_weather(){
  snprintf(message, 200, "{\"weather\":[%u,%u,%u,%u,%u,%u,%u,%u,%u]}",modbus_array[0],modbus_array[1],modbus_array[2],modbus_array[3],modbus_array[4],modbus_array[5],modbus_array[6],modbus_array[7],modbus_array[8]);
  client.publish(weather_topic,message,true);
}

void publish_soil(){
  snprintf(message, 200, "{\"soil\":[%u,%u,%u,%u,%u,%u,%u]}",modbus_array[11],modbus_array[12],modbus_array[13],modbus_array[14],modbus_array[15],modbus_array[16],modbus_array[17]);
  client.publish(soil_topic,message,true);
}

void publish_sensor(){
  snprintf(message, 200, "{\"sensor\":[%u,%u,%u]}",modbus_array[9],modbus_array[10],modbus_array[18]);
  client.publish(sensor_topic,message,true);
}

//void publish_switch(){
//  snprintf(message, 200, "{\"switch\":[%u,%u]}",modbus_array[101],modbus_array[111]);
//  client.publish(switch_topic,message,true);
//}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
//   communicate with Modbus slave ID 1 over Serial (TX RX)
// Modbus({slave id}, {Serial, Serial1, Serial2 ...}, {pin connected to RE & DE pin of MAX-485 module(GPIO 핀)})
  slave = Modbus(slave_id,Serial2,re_de_pin);
  slave.begin(9600);
  setModbusMetadata();
  setMqttMetadata();
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if(count == 10) {
    publish_weather();
  } else if (count == 20) {
    publish_sensor();
  } else if (count == 30) {
    publish_soil();
  } 
//  else if (count == 40) {
//    publish_switch();
//    count = 0;
//  }
  count++;
  loop_modbus();
}
