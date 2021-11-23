//espNow 관련 라이브러리 및 전역 설정
#include <esp_now.h>
#include <WiFi.h>
// Global copy of slave
#define NUMSLAVES 20
esp_now_peer_info_t slaves[NUMSLAVES] = {};
int SlaveCnt = 0;

#define CHANNEL 3
#define PRINTSCANRESULTS 0
//ETH setting (tricky!! Don't switch the order between libraries)
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12

// ETH (POE연결 위한) 라이브러리
#include <ETH.h>
// MQTT 라이브러리
#include <PubSubClient.h>
#include <ArduinoJson.h> // [Json 라이브러리 버전 : ArduinoJson 5]

// Variables about MQTT try and payload
int request = 0;
char *payloadString;

// Variables about MQTT connection
const char* mqttServerIp = "mqtt.farota.com"; // IP address of the MQTT broker
const short mqttServerPort = 1883; // IP port of the MQTT broker
const char* mqttClientName = "booth";
const char* mqttUsername = "farotadev";
const char* mqttPassword = "y!j7DD41-w*uQ!^7";

// MQTT topics
#define tempTopic "FS1001/1209B/4110"
#define xAxisTopic "FS1001/1209B/4120"
#define yAxisTopic "FS1001/1209B/4130"
#define zAxisTopic "FS1001/1209B/4140"
#define connTopic "FS1001/1209B/connection"
#define confTopic "FS1001/1209B/config"

#define dataCnt 2048
#define maxMqttBufferSize 16384

WiFiClient espClient;
PubSubClient mqttClient(espClient);
DynamicJsonBuffer jsonBuffer;


int16_t gain = 16; // default : ADXL345_RANGE_16G
int16_t sleepTime = 5; //default : 5 seconds
int32_t targetRssi;
String targetMacAddress;
String targetSerial;
int16_t targetBattery;
int16_t targetTemp;
int16_t targetBootCnt;
static bool eth_connected = false;
uint8_t data = 0;
boolean accessible = 1;
int16_t accessInfo [32] = {};

typedef struct struct_message {
  int16_t payload[32];                 // Because a maximum length of payload of ESPNOW is 250bytes
} struct_message;
struct_message myData;

int16_t mqttPayload[2048];
int16_t axis[1];

int count = 0;
int valNo = 0;
String axisName;
int axisIdx;
int slaveIdx = 0;

void setup() {
  Serial.begin(115200);

  /* generate chipId 이 부분이 슬래이브 setSerial탭에 들어가야 할지도...
  chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
  Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.
  */
  
  // This is the mac address of the Master in Station Mode
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
  // Init ESPNow with a fallback logic
  initESPNow(); //Init 실패 시, 카운팅 or 그냥 재부팅 [현재 재부팅ESP.restart()]
  
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  wifiMqttInit();  
}

/*  구 버전 루프함수, 멀티플 슬레이브므로 이더넷 연결 + 슬레이브 스캔 + 슬레이브 관리 + 변수 변경 처리 등 고려해서 통합해야 함
void loop() {
    // check if ethernet is connected
  if (eth_connected) {
  // now take care of MQTT client...
    if (!mqttClient.connected()) {
      reconnect();
    }
  }
}
*/

void loop() { // 주요 추가, 수정 파트
  // check if ethernet is connected
  if (eth_connected) {
  // now take care of MQTT client...
    mqttClient.loop();
    if (!mqttClient.connected()) {
      reconnect();
    } else {
      // If Slave is found, it would be populate in `slave` variable
      // We will check if `slave` is defined and then we proceed further
      if (SlaveCnt > 0) { // check if slave channel is defined
        if(SlaveCnt != slaveIdx){
          // `slave` is defined
          // Add slave as peer if it has not been added already
          manageSlave(slaveIdx);
          // pair success or already paired
          // Send data to device
          sendData(slaveIdx);
          slaveIdx++;
          delay(20000);
        } else {
          SlaveCnt = 0;
          slaveIdx = 0;
          WiFi.scanDelete();
          mqttPublishConfigInfo();
        }
        
      } else {
        // In the loop we scan for slave
        ScanForSlave();
      }
      Serial.print("sleep time : ");
      Serial.println(sleepTime);
      Serial.print("gain : ");
      Serial.println(gain);
    }
    
  }
}
