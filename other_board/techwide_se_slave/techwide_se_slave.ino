                                                  #include <esp_now.h>
#include <WiFi.h>

//(가속도) 센서 라이브러리
#include<Wire.h>
#include<ADXL345_WE.h>
#include "Adafruit_MCP9808.h"
#define ADXL345_I2CADDR 0x53 // 0x1D if SDO = HIGH
#define MCP9808_I2CADDR 0x18 // 0x18 - 0x1F (can set address)

#define MCP9808_RESOLUTION 1

#define CHANNEL 1
#define BATT_ADC_PIN 23

#define uS_TO_S_FACTOR 1000000  /* (딥슬립)Conversion factor for micro seconds to seconds */
// #define TIME_TO_SLEEP 5        /* (딥슬립)Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int TIME_TO_SLEEP = 600; // 고정 변수가 아니라 기본 값 설정후 RTC에 저장
RTC_DATA_ATTR int gain = 16; // gain default : 16G, TIME_TO_SLEEP default : 600 (seconds)

RTC_DATA_ATTR int bootCount = 0; // (딥슬립)wakeupCount

RTC_DATA_ATTR int switchAP = 1;

RTC_DATA_ATTR int mac1 = 0;
RTC_DATA_ATTR int mac2 = 0;
RTC_DATA_ATTR int mac3 = 0;
RTC_DATA_ATTR int mac4 = 0;
RTC_DATA_ATTR int mac5 = 0;
RTC_DATA_ATTR int mac6 = 0;



uint8_t broadcastAddress [] = {};

ADXL345_WE myAcc (ADXL345_I2CADDR); // (가속도)센서 주소 지정
// ADXL345_WE myAcc = ADXL345_WE(); // (가속도)Alternative: sets default address 0x53

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808(); // Create the MCP9808 temperature sensor object

esp_now_peer_info_t peerInfo;

int16_t axis [32] = {}; // MESSAGE STRUCTURE FOR SENDING
int16_t payload [32];
int16_t device [32] = {};

int16_t x [2049];
int16_t y [2049];
int16_t z [2049];


typedef struct struct_message {
  int16_t payload[32];                 // Because a maximum length of payload of ESPNOW is 250bytes
} struct_message;
struct_message recvData;


// [타이머 변수 = 디폴트]
// 배터리 전압 변수
int16_t getVoltage()
{
    uint16_t v = analogRead(BATT_ADC_PIN);
    float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (1100 / 1000.0) * 100;
    return (int16_t)battery_voltage;
}

void setup() {
    Wire.begin();
    Serial.begin(115200);
    arrInit();
    initTempSensor();
    Serial.println("Boot number: " + String(bootCount));
    if(switchAP){
      setAPMode();
      initESPNow(); // Init ESPNow with a fallback logic : 초기화
      esp_now_register_recv_cb(OnDataRecv); // [콜백 로직 검토 및 추가 필요] 받으면  callBack 탭 참조
      esp_now_register_send_cb(OnDataSent); // [콜백 로직 검토 필요] 보내면
    } else {
      ++bootCount; // (딥슬립) wakeup카운터 누적 increment boot number
      broadcastAddress[0] = mac1; broadcastAddress[1] = mac2; broadcastAddress[2] = mac3; broadcastAddress[3] = mac4; broadcastAddress[4] = mac5; broadcastAddress[5] = mac6;
      Serial.print("time to sleep : "); Serial.println(TIME_TO_SLEEP);
      espSetStaMode();
      esp_now_register_recv_cb(OnDataRecv); // [콜백 로직 검토 및 추가 필요] 받으면  callBack 탭 참조
      esp_now_register_send_cb(OnDataSent); // [콜백 로직 검토 필요] 보내면
      readAcc();
      switchAP = 1;
      esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);  // set wakeup timing
      esp_deep_sleep_start(); // he's sleep until uS_TO_S_FACTOR comes
    }
    
    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info. : 콜백 세팅
    
    
    
    // ADXL탭 readAcc함수에서 사용: sendAxisInfo(), sendPayload()
    // [Deep sleep 루틴 적용 : timer로 wakeup]
    //esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);  // TIME_TO_SLEEP 12행에 #define되어 있음 set wakeup timing
  /* [Deep sleep 주기 변수화: mqtt로 변경] */
  /* [adxl_we gain값 변수 지정하여 mqtt로 변경하는 기능?] ADXL345탭41행 myAcc.setRange(ADXL345_RANGE_16G); */
    //esp_deep_sleep_start(); // he's sleep until uS_TO_S_FACTOR comes    // [슬립 로직 적용시 AP 모드 전환되지 않음.]
}

void loop() {
  // Chill : 딥슬립 루틴은 셋업에서 끝남
}
