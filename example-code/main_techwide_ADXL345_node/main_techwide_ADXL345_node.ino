#include<esp_now.h>
#include<WiFi.h>
#include<Wire.h>
#include<ADXL345_WE.h>

#define ADXL345_I2CADDR 0x53 // 0x1D if SDO = HIGH
// REPLACE WITH YOUR RECEIVER MAC Address
//uint8_t broadcastAddress [] = { 0x7C, 0x9E, 0xBD, 0xE6, 0x95, 0xD8 };
uint8_t broadcastAddress [] = { 0x24, 0x0A, 0xC4, 0xF5, 0x49, 0x68 };

// ESPNOP PEER INFO SHOULD DECLARE HERE AS GLOBAL
esp_now_peer_info_t peerInfo;

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 5        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;

ADXL345_WE myAcc (ADXL345_I2CADDR);
// ADXL345_WE myAcc = ADXL345_WE(); // Alternative: sets default address 0x53
int16_t axis [32] = {};
// MESSAGE STRUCTURE FOR SENDING
int16_t payload [32];

int16_t x [2049];
int16_t y [2049];
int16_t z [2049];

void setup () {
    Wire.begin();
    Serial.begin(115200);
    arrInit();

    //Increment boot number and print it every reboot
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));
    espNowInit();
    readAcc();

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);  // set wakeup timing
    esp_deep_sleep_start(); // he's sleep until uS_TO_S_FACTOR comes
}

void loop () {}
