#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ModbusRTU.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SLAVE_ID 1
#define RE_DE 19
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

WiFiClient espClient;
PubSubClient client(espClient);
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


void convert_byte(float x){
  
  int *pointer;
  pointer = (int*)&x;
  
  int n = *pointer;
  String num_str = String(n,HEX);
  String n_str = num_str.substring(0,4);
  String p_str = num_str.substring(4);
  char n_ch[] = {};
  char p_ch[] = {};
  
  n_str.toCharArray(n_ch, 5, 0);
  data_convert_array[1] = (uint16_t) strtol(n_ch, 0, 16);
  
  p_str.toCharArray(p_ch, 5, 0);
  data_convert_array[0] = (uint16_t) strtol(p_ch, 0, 16);
}


void set_temp_01(float temp){
  convert_byte(temp);
  modbus_array[204] = 0;
  mb.Hreg(204, modbus_array[204]);
  modbus_array[202] = data_convert_array[0];
  mb.Hreg(202, modbus_array[202]);
  modbus_array[203] = data_convert_array[1];
  mb.Hreg(203, modbus_array[203]);

  mqtt_array[204] = 0;
  mqtt_array[202] = data_convert_array[0];
  mqtt_array[203] = data_convert_array[1];
}

void set_temp_02(float temp){
  convert_byte(temp);
  modbus_array[207] = 0;
  mb.Hreg(207, modbus_array[207]);
  modbus_array[205] = data_convert_array[0];
  mb.Hreg(205, modbus_array[205]);
  modbus_array[206] = data_convert_array[1];
  mb.Hreg(206, modbus_array[206]);

  mqtt_array[207] = 0;
  mqtt_array[205] = data_convert_array[0];
  mqtt_array[206] = data_convert_array[1];
}

void set_temp_03(float temp){
  convert_byte(temp);
  modbus_array[210] = 0;
  mb.Hreg(210, modbus_array[210]);
  modbus_array[208] = data_convert_array[0];
  mb.Hreg(208, modbus_array[208]);
  modbus_array[209] = data_convert_array[1];
  mb.Hreg(209, modbus_array[209]);

  mqtt_array[210] = 0;
  mqtt_array[208] = data_convert_array[0];
  mqtt_array[209] = data_convert_array[1];
}

void set_temp_04(float temp){
  convert_byte(temp);
  modbus_array[261] = 0;
  mb.Hreg(261, modbus_array[261]);
  modbus_array[259] = data_convert_array[0];
  mb.Hreg(259, modbus_array[259]);
  modbus_array[260] = data_convert_array[1];
  mb.Hreg(260, modbus_array[260]);

  mqtt_array[261] = 0;
  mqtt_array[259] = data_convert_array[0];
  mqtt_array[260] = data_convert_array[1];
}

void set_temp_05(float temp){
  convert_byte(temp);
  modbus_array[264] = 0;
  mb.Hreg(264, modbus_array[264]);
  modbus_array[262] = data_convert_array[0];
  mb.Hreg(262, modbus_array[262]);
  modbus_array[263] = data_convert_array[1];
  mb.Hreg(263, modbus_array[263]);

  mqtt_array[264] = 0;
  mqtt_array[262] = data_convert_array[0];
  mqtt_array[263] = data_convert_array[1];
}

void set_temp_06(float temp){
  convert_byte(temp);
  modbus_array[267] = 0;
  mb.Hreg(267, modbus_array[267]);
  modbus_array[265] = data_convert_array[0];
  mb.Hreg(265, modbus_array[265]);
  modbus_array[266] = data_convert_array[1];
  mb.Hreg(266, modbus_array[266]);

  mqtt_array[267] = 0;
  mqtt_array[265] = data_convert_array[0];
  mqtt_array[266] = data_convert_array[1];
}

void set_temp_07(float temp){
  convert_byte(temp);
  modbus_array[270] = 0;
  mb.Hreg(270, modbus_array[270]);
  modbus_array[268] = data_convert_array[0];
  mb.Hreg(268, modbus_array[268]);
  modbus_array[269] = data_convert_array[1];
  mb.Hreg(269, modbus_array[269]);

  mqtt_array[270] = 0;
  mqtt_array[268] = data_convert_array[0];
  mqtt_array[269] = data_convert_array[1];
}

void set_temp_08(float temp){
  convert_byte(temp);
  modbus_array[273] = 0;
  mb.Hreg(273, modbus_array[273]);
  modbus_array[271] = data_convert_array[0];
  mb.Hreg(271, modbus_array[271]);
  modbus_array[272] = data_convert_array[1];
  mb.Hreg(272, modbus_array[272]);

  mqtt_array[273] = 0;
  mqtt_array[271] = data_convert_array[0];
  mqtt_array[272] = data_convert_array[1];
}

void set_temp_09(float temp){
  convert_byte(temp);
  modbus_array[276] = 0;
  mb.Hreg(276, modbus_array[276]);
  modbus_array[274] = data_convert_array[0];
  mb.Hreg(274, modbus_array[274]);
  modbus_array[275] = data_convert_array[1];
  mb.Hreg(275, modbus_array[275]);

  mqtt_array[276] = 0;
  mqtt_array[274] = data_convert_array[0];
  mqtt_array[275] = data_convert_array[1];
}

void set_temp_10(float temp){
  convert_byte(temp);
  modbus_array[279] = 0;
  mb.Hreg(279, modbus_array[279]);
  modbus_array[277] = data_convert_array[0];
  mb.Hreg(277, modbus_array[277]);
  modbus_array[278] = data_convert_array[1];
  mb.Hreg(278, modbus_array[278]);

  mqtt_array[279] = 0;
  mqtt_array[277] = data_convert_array[0];
  mqtt_array[278] = data_convert_array[1];
}

void set_humi_01(float humi){
  convert_byte(humi);
  modbus_array[213] = 0;
  mb.Hreg(213, modbus_array[213]);
  modbus_array[211] = data_convert_array[0];
  mb.Hreg(211, modbus_array[211]);
  modbus_array[212] = data_convert_array[1];
  mb.Hreg(212, modbus_array[212]);

  mqtt_array[213] = 0;
  mqtt_array[211] = data_convert_array[0];
  mqtt_array[212] = data_convert_array[1];
}

void set_humi_02(float humi){
  convert_byte(humi);
  modbus_array[282] = 0;
  mb.Hreg(282, modbus_array[282]);
  modbus_array[280] = data_convert_array[0];
  mb.Hreg(280, modbus_array[280]);
  modbus_array[281] = data_convert_array[1];
  mb.Hreg(281, modbus_array[281]);

  mqtt_array[282] = 0;
  mqtt_array[280] = data_convert_array[0];
  mqtt_array[281] = data_convert_array[1];
}

void set_humi_03(float humi){
  convert_byte(humi);
  modbus_array[285] = 0;
  mb.Hreg(285, modbus_array[285]);
  modbus_array[283] = data_convert_array[0];
  mb.Hreg(283, modbus_array[283]);
  modbus_array[284] = data_convert_array[1];
  mb.Hreg(284, modbus_array[284]);

  mqtt_array[285] = 0;
  mqtt_array[283] = data_convert_array[0];
  mqtt_array[284] = data_convert_array[1];
}

void set_dew_point_sensor_01(float dew_point){
  convert_byte(dew_point);
  modbus_array[216] = 0;
  mb.Hreg(216, modbus_array[216]);
  modbus_array[214] = data_convert_array[0];
  mb.Hreg(214, modbus_array[214]);
  modbus_array[215] = data_convert_array[1];
  mb.Hreg(215, modbus_array[215]);

  mqtt_array[216] = 0;
  mqtt_array[214] = data_convert_array[0];
  mqtt_array[215] = data_convert_array[1];
}

void set_rain_01(float rain){
  convert_byte(rain);
  modbus_array[219] = 0;
  mb.Hreg(219, modbus_array[219]);
  modbus_array[217] = data_convert_array[0];
  mb.Hreg(217, modbus_array[217]);
  modbus_array[218] = data_convert_array[1];
  mb.Hreg(218, modbus_array[218]);

  mqtt_array[219] = 0;
  mqtt_array[217] = data_convert_array[0];
  mqtt_array[218] = data_convert_array[1];
}

void set_flow_01(float flow){
  convert_byte(flow);
  modbus_array[222] = 0;
  mb.Hreg(222, modbus_array[222]);
  modbus_array[220] = data_convert_array[0];
  mb.Hreg(220, modbus_array[220]);
  modbus_array[221] = data_convert_array[1];
  mb.Hreg(221, modbus_array[221]);

  mqtt_array[222] = 0;
  mqtt_array[220] = data_convert_array[0];
  mqtt_array[221] = data_convert_array[1];
}

void set_rain_02(float rain){
  convert_byte(rain);
  modbus_array[225] = 0;
  mb.Hreg(225, modbus_array[225]);
  modbus_array[223] = data_convert_array[0];
  mb.Hreg(223, modbus_array[223]);
  modbus_array[224] = data_convert_array[1];
  mb.Hreg(224, modbus_array[224]);

  mqtt_array[225] = 0;
  mqtt_array[223] = data_convert_array[0];
  mqtt_array[224] = data_convert_array[1];
}

void set_sun_01(float sun){
  convert_byte(sun);
  modbus_array[228] = 0;
  mb.Hreg(228, modbus_array[228]);
  modbus_array[226] = data_convert_array[0];
  mb.Hreg(226, modbus_array[226]);
  modbus_array[227] = data_convert_array[1];
  mb.Hreg(227, modbus_array[227]);

  mqtt_array[228] = 0;
  mqtt_array[226] = data_convert_array[0];
  mqtt_array[227] = data_convert_array[1];
}

void set_wind_speed_01(float wind_speed){
  convert_byte(wind_speed);
  modbus_array[231] = 0;
  mb.Hreg(231, modbus_array[231]);
  modbus_array[229] = data_convert_array[0];
  mb.Hreg(229, modbus_array[229]);
  modbus_array[230] = data_convert_array[1];
  mb.Hreg(230, modbus_array[230]);

  mqtt_array[231] = 0;
  mqtt_array[229] = data_convert_array[0];
  mqtt_array[230] = data_convert_array[1];
}

void set_wind_dir_01(float wind_dir){
  convert_byte(wind_dir);
  modbus_array[234] = 0;
  mb.Hreg(234, modbus_array[234]);
  modbus_array[232] = data_convert_array[0];
  mb.Hreg(232, modbus_array[232]);
  modbus_array[233] = data_convert_array[1];
  mb.Hreg(233, modbus_array[233]);

  mqtt_array[234] = 0;
  mqtt_array[232] = data_convert_array[0];
  mqtt_array[233] = data_convert_array[1];
}

void set_vol_01(float vol){
  convert_byte(vol);
  modbus_array[237] = 0;
  mb.Hreg(237, modbus_array[237]);
  modbus_array[235] = data_convert_array[0];
  mb.Hreg(235, modbus_array[235]);
  modbus_array[236] = data_convert_array[1];
  mb.Hreg(236, modbus_array[236]);

  mqtt_array[237] = 0;
  mqtt_array[235] = data_convert_array[0];
  mqtt_array[236] = data_convert_array[1];
}

void set_co2_01(float co2){
  convert_byte(co2);
  modbus_array[240] = 0;
  mb.Hreg(240, modbus_array[240]);
  modbus_array[238] = data_convert_array[0];
  mb.Hreg(238, modbus_array[238]);
  modbus_array[239] = data_convert_array[1];
  mb.Hreg(239, modbus_array[239]);

  mqtt_array[240] = 0;
  mqtt_array[238] = data_convert_array[0];
  mqtt_array[239] = data_convert_array[1];
}

void set_ec_01(float ec){
  convert_byte(ec);
  modbus_array[243] = 0;
  mb.Hreg(243, modbus_array[243]);
  modbus_array[241] = data_convert_array[0];
  mb.Hreg(241, modbus_array[241]);
  modbus_array[242] = data_convert_array[1];
  mb.Hreg(242, modbus_array[242]);

  mqtt_array[243] = 0;
  mqtt_array[241] = data_convert_array[0];
  mqtt_array[242] = data_convert_array[1];
}

void set_photon_01(float photon){
  convert_byte(photon);
  modbus_array[246] = 0;
  mb.Hreg(246, modbus_array[246]);
  modbus_array[244] = data_convert_array[0];
  mb.Hreg(244, modbus_array[244]);
  modbus_array[245] = data_convert_array[1];
  mb.Hreg(245, modbus_array[245]);

  mqtt_array[246] = 0;
  mqtt_array[244] = data_convert_array[0];
  mqtt_array[245] = data_convert_array[1];
}

void set_soil_moi_01(float soil_moi){
  convert_byte(soil_moi);
  modbus_array[249] = 0;
  mb.Hreg(249, modbus_array[249]);
  modbus_array[247] = data_convert_array[0];
  mb.Hreg(247, modbus_array[247]);
  modbus_array[248] = data_convert_array[1];
  mb.Hreg(248, modbus_array[248]);

  mqtt_array[249] = 0;
  mqtt_array[247] = data_convert_array[0];
  mqtt_array[248] = data_convert_array[1];
}

void set_soil_water_ten_01(float soil_water_ten){
  convert_byte(soil_water_ten);
  modbus_array[252] = 0;
  mb.Hreg(252, modbus_array[252]);
  modbus_array[250] = data_convert_array[0];
  mb.Hreg(250, modbus_array[250]);
  modbus_array[251] = data_convert_array[1];
  mb.Hreg(251, modbus_array[251]);

  mqtt_array[252] = 0;
  mqtt_array[250] = data_convert_array[0];
  mqtt_array[251] = data_convert_array[1];
}

void set_ph_01(float ph){
  convert_byte(ph);
  modbus_array[255] = 0;
  mb.Hreg(255, modbus_array[255]);
  modbus_array[253] = data_convert_array[0];
  mb.Hreg(253, modbus_array[253]);
  modbus_array[254] = data_convert_array[1];
  mb.Hreg(254, modbus_array[254]);

  mqtt_array[255] = 0;
  mqtt_array[253] = data_convert_array[0];
  mqtt_array[254] = data_convert_array[1];
}

void set_soil_temp_01(float soil_temp){
  convert_byte(soil_temp);
  modbus_array[258] = 0;
  mb.Hreg(258, modbus_array[258]);
  modbus_array[256] = data_convert_array[0];
  mb.Hreg(256, modbus_array[256]);
  modbus_array[257] = data_convert_array[1];
  mb.Hreg(257, modbus_array[257]);

  mqtt_array[258] = 0;
  mqtt_array[256] = data_convert_array[0];
  mqtt_array[257] = data_convert_array[1];
}


void set_weight_01(float weight){
  convert_byte(weight);
  modbus_array[288] = 0;
  mb.Hreg(288, modbus_array[288]);
  modbus_array[286] = data_convert_array[0];
  mb.Hreg(286, modbus_array[286]);
  modbus_array[287] = data_convert_array[1];
  mb.Hreg(287, modbus_array[287]);

  mqtt_array[288] = 0;
  mqtt_array[286] = data_convert_array[0];
  mqtt_array[287] = data_convert_array[1];
}

void set_weight_02(float weight){
  convert_byte(weight);
  modbus_array[291] = 0;
  mb.Hreg(291, modbus_array[291]);
  modbus_array[289] = data_convert_array[0];
  mb.Hreg(289, modbus_array[289]);
  modbus_array[290] = data_convert_array[1];
  mb.Hreg(290, modbus_array[290]);

  mqtt_array[291] = 0;
  mqtt_array[289] = data_convert_array[0];
  mqtt_array[290] = data_convert_array[1];
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
  modbus_array[100]= 1; // 온도 1
  mb.Hreg(100, modbus_array[100]);
  modbus_array[101]= 1; // 온도 2
  mb.Hreg(101, modbus_array[101]);
  modbus_array[102]= 1; // 온도 3
  mb.Hreg(102, modbus_array[102]);
  modbus_array[103]= 2; // 습도 1
  mb.Hreg(103, modbus_array[103]);
  modbus_array[104]= 3; // 이슬점 센서
  mb.Hreg(104, modbus_array[104]);
  modbus_array[105]= 4; // 감우 센서
  mb.Hreg(105, modbus_array[105]);
  modbus_array[106]= 5; // 유량 센서
  mb.Hreg(106, modbus_array[106]);
  modbus_array[107]= 6; // 강우 센서
  mb.Hreg(107, modbus_array[107]);
  modbus_array[108]= 7; // 일사 센서
  mb.Hreg(108, modbus_array[108]);
  modbus_array[109]= 8; // 풍속 센서
  mb.Hreg(109, modbus_array[109]);
  modbus_array[110]= 9; // 풍향 센서
  mb.Hreg(110, modbus_array[110]);
  modbus_array[111]= 10; // 전압 센서
  mb.Hreg(111, modbus_array[111]);
  modbus_array[112]= 11; // CO2 센서
  mb.Hreg(112, modbus_array[112]);
  modbus_array[113]= 12; // EC 센서
  mb.Hreg(113, modbus_array[113]);
  modbus_array[114]= 13; // 광자양 센서
  mb.Hreg(114, modbus_array[114]);
  modbus_array[115]= 14; // 토양 함수율 센서
  mb.Hreg(115, modbus_array[115]);
  modbus_array[116]= 15; // 토양 수분 장력 센서
  mb.Hreg(116, modbus_array[116]);
  modbus_array[117]= 16; // PH 센서
  mb.Hreg(117, modbus_array[117]);
  modbus_array[118]= 17; // 지온 센서
  mb.Hreg(118, modbus_array[118]);
  modbus_array[119]= 1; // 온도4 센서
  mb.Hreg(119, modbus_array[119]);
  modbus_array[120]= 1; // 온도5 센서
  mb.Hreg(120, modbus_array[120]);
  modbus_array[121]= 1; // 온도6 센서
  mb.Hreg(121, modbus_array[121]);
  modbus_array[122]= 1; // 온도7 센서
  mb.Hreg(122, modbus_array[122]);
  modbus_array[123]= 1; // 온도8 센서
  mb.Hreg(123, modbus_array[123]);
  modbus_array[124]= 1; // 온도9 센서
  mb.Hreg(124, modbus_array[124]);
  modbus_array[125]= 1; // 온도10 센서
  mb.Hreg(125, modbus_array[125]);
  modbus_array[126]= 2; // 습도2 센서
  mb.Hreg(126, modbus_array[126]);
  modbus_array[127]= 2; // 습도3 센서
  mb.Hreg(127, modbus_array[127]);
  modbus_array[128]= 19; // 무게1 센서
  mb.Hreg(128, modbus_array[128]);
  modbus_array[129]= 19; // 무게2 센서
  mb.Hreg(129, modbus_array[129]);

  
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
  Serial.println("");
  JsonObject& root = jsonBuffer.parse((char*)payload);
  if(root.success()){
    if (String(topic).equals(String(weather_topic))){
      set_temp_01((float) root["outTemp"]);
      set_humi_01((float) root["outHumidity"]);
      set_dew_point_sensor_01((float) root["dewpoint"]);
      set_rain_01((float) root["rain"]);
      set_rain_02((float) root["rain"]);
      set_sun_01((float) root["radiation"]);
      set_wind_speed_01((float) root["windSpeed"]);
      set_wind_dir_01((float) root["windDir"]);
      
    } else if (String(topic).equals(String(ec_topic))){
      set_ec_01((float) root["ec"]);
      
    } else if (String(topic).equals(String(soil_topic))){
      if(root["moi"] != NULL){
        set_soil_moi_01((float) root["moi"]);
      }
      if(root["tem"] != NULL){
        set_soil_temp_01((float) root["tem"]);
      }
    } else if (String(topic).equals(String(ph_topic))){
      set_ph_01((float) root["pH"]);
    } else if (String(topic).equals(String(pf_topic))){
      set_soil_water_ten_01((float) root["pF"]);
    }
  } else {
    Serial.println("parseObject() failed");
  }
  
}

void setModbusAddress(){
  for(int i=0; i < register_size; i ++){
    mb.addHreg(i);
    mb.Hreg(i, 0);
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
      client.publish(conn_topic, "connected");
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
  mb.task();
  yield();
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
  setMqttMetadata();
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
//  if(count == 10) {
//    publish_weather();
//  } else if (count == 20) {
//    publish_sensor();
//  } else if (count == 30) {
//    publish_soil();
//  } 
//  else if (count == 40) {
//    publish_switch();
//    count = 0;
//  }
  count++;
  loop_modbus();
}
