#include <esp_now.h>
#include <WiFi.h>
#include <stdio.h>

//(가속도) 센서 라이브러리
#include<Wire.h>
#include<ADXL345_WE.h>

#define ADXL345_I2CADDR 0x53 // 0x1D if SDO = HIGH

#define CHANNEL         2
#define BATT_ADC_PIN    23
#define FREQENCY        50 // 50hz
#define COLLECT_COUNT   50 //50회 수집
#define GAIN_VAL        16
#define TIME_TO_SLEEP   60

#define uS_TO_S_FACTOR 1000000  /* (딥슬립)Conversion factor for micro seconds to seconds */

//uint8_t broadcastAddress[] = {0x98, 0xF4, 0xAB, 0x20, 0xE6, 0xB8};
uint8_t broadcastAddress[] = {0x98, 0xF4, 0xAB, 0x20, 0xCD, 0x60};

ADXL345_WE myAcc (ADXL345_I2CADDR); // (가속도)센서 주소 지정
RTC_DATA_ATTR int bootCount = 0; // (딥슬립)wakeupCount

esp_now_peer_info_t peerInfo;

float x[COLLECT_COUNT];
float y[COLLECT_COUNT];
float z[COLLECT_COUNT];


typedef struct struct_message {
  int16_t device_no;                 // Because a maximum length of payload of ESPNOW is 250bytes
  float x_val;
  float y_val;
  float z_val;
} struct_message;
struct_message sensorData;

void setup() {
    Wire.begin();
    Serial.begin(115200);
    initESPNow();
    arrInit();
    
    Serial.println("Boot number: " + String(bootCount));
     
    ++bootCount; // (딥슬립) wakeup카운터 누적 increment boot number
    
    Serial.print("time to sleep : "); Serial.println(TIME_TO_SLEEP);

    readAcc();
    sendPayload();
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);  // set wakeup timing
    esp_deep_sleep_start(); // he's sleep until uS_TO_S_FACTOR comes
}

void loop() {
  // Chill : 딥슬립 루틴은 셋업에서 끝남
}
