/*
  LoRa Duplex communication wth callback

  Sends a message every half second, and uses callback
  for new incoming messages. Implements a one-byte addressing scheme,
  with 0xFF as the broadcast address.

  Note: while sending, LoRa radio is not listening for incoming messages.
  Note2: when using the callback method, you can't use any of the Stream
  functions that rely on the timeout, such as readString, parseInt(), etc.

  created 28 April 2017
  by Tom Igoe
*/
#include <SPI.h>              // include libraries
#include <LoRa.h>
#include <ArduinoJson.h>

#ifdef ARDUINO_SAMD_MKRWAN1300
#error "This example is not compatible with the Arduino MKR WAN 1300 board!"
#endif


/*
 *  LoRa Radio Node pinout
 *  
 *  DIO0      ||      D2
 *  DIO1      ||      D5
 *  DIO2      ||      D6
 *  DIO3      ||      D7
 *  DIO5      ||      D8
 */

// lora profiles
//#define CS_PIN          7       // LoRa radio chip select
//#define RESET_PIN       6       // LoRa radio reset
//#define IRQ_PIN         1       // change for your board; must be a hardware interrupt pin
#define BUFFER_SIZE     250 // Define the payload size here

// device profiles
#define CHANNEL         1
#define VALVE_NO        1
#define DEVICE_TYPE     1 // 1: 1ch relay, 2: 2ch relay, 3: 6ch relay

#define RELAY_PIN       7 // DIO3
#define INTERVAL_TIME   6000

String txpacket;              // outgoing message
long lastSendTime = 0;        // last send time

// reserve buffer for Json structs
StaticJsonDocument<BUFFER_SIZE> doc;
int relayStatus[CHANNEL] = {};
int relayStatusChange = false;
int switchNo = 1;

void setup() {
  Serial.begin(9600);                   // initialize serial
  while (!Serial);
  Serial.setTimeout(100);
  loraSetup();   // in [lora]

}

void loop() {
}
