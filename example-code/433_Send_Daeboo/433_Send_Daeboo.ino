/*Libraries about OTA feature
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "SMARF";
const char* password = "43100900";
*/

//RF433 library
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

//Deepsleep variables
#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex
RTC_DATA_ATTR int bootCount = 0;

void setup() {
  Serial.begin(115200);
  pinMode(33, INPUT);
  delay(1000);

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));    

  //Print the wakeup reason for ESP32 and touchpad too
  print_wakeup_reason();

//  attachInterrupt(digitalPinToInterrupt(33), OTA, RISING);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,1); //1 = High, 0 = Low

  //print_wakeup_touchpad();
  /*
  First we configure the wake up source
  We set our ESP32 to wake up for an external trigger.
  There are two types for ESP32, ext0 and ext1 .
  ext0 uses RTC_IO to wakeup thus requires RTC peripherals
  to be on while ext1 uses RTC Controller so doesnt need
  peripherals to be powered on.
  Note that using internal pullups/pulldowns also requires
  RTC peripherals to be turned on.
  */
  
  //Setup interrupt on Touch Pad 3 (GPIO15)
  /*touchAttachInterrupt(T3, callback, Threshold);*/
  
  //Configure Touchpad as wakeup source
  /*esp_sleep_enable_touchpad_wakeup();*/

  //Go to sleep now
  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop() {
  //This will never be reached
}
