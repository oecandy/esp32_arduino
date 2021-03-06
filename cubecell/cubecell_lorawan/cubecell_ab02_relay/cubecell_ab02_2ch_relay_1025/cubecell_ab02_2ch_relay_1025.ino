#include "LoRaWan_APP.h"
#include "Arduino.h"

#define VALVE_1_PIN         GPIO5
#define VALVE_2_PIN         GPIO6
#define FLOWMETER_PIN       GPIO12
#define CHANNEL             2
#define FIRMWARE_VERSION    10
/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

#if FIRMWARE_VERSION == 1 // test-relay-2ch-cubecell
  uint8_t devEui[] = { 0xA0, 0x0C, 0x18, 0xFE, 0x21, 0x05, 0xB4, 0xEF };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0x89, 0xF4, 0xDA, 0xA8, 0x8F, 0x44, 0x3E, 0xC3, 0x81, 0x30, 0x36, 0x87, 0xF0, 0x91, 0x93, 0x9F };
#elif FIRMWARE_VERSION == 2 // skep-pangyo-gate-2111-01
  uint8_t devEui[] = { 0x76, 0x53, 0xe0, 0x6d, 0xe7, 0xe0, 0xaa, 0x34 };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0x3A, 0x96, 0xBD, 0xEE, 0xE9, 0x02, 0x38, 0xFA, 0xCC, 0xBC, 0xBE, 0x6B, 0xE6, 0x6E, 0x14, 0x9A };
#elif FIRMWARE_VERSION == 3 // skep-pangyo-mvsprk-2111-01
  uint8_t devEui[] = { 0x2b, 0x95, 0xc6, 0xf4, 0x38, 0xfb, 0xe8, 0x39 };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0x87, 0x46, 0x94, 0x15, 0x9B, 0xF5, 0x00, 0x8E, 0x50, 0x3D, 0x9A, 0x05, 0xBF, 0x22, 0x8D, 0xF4 };
#elif FIRMWARE_VERSION == 4 // skep-pangyo-mvsprk-2111-02
  uint8_t devEui[] = { 0xc6, 0x6a, 0x82, 0x6f, 0xfe, 0x84, 0xa3, 0x18 };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0x0C, 0x7B, 0xC1, 0xD3, 0x2B, 0x87, 0x89, 0x76, 0xEC, 0xCD, 0x01, 0xBE, 0xFE, 0x21, 0x17, 0x4D };
#elif FIRMWARE_VERSION == 5 // skep-pangyo-mvsprk-2111-03
  uint8_t devEui[] = { 0xc0, 0x0e, 0xa0, 0x3e, 0xcb, 0xbf, 0x66, 0x95 };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0x2E, 0x38, 0x33, 0x57, 0xE4, 0x5A, 0x5F, 0x97, 0x19, 0xDE, 0x80, 0x48, 0x63, 0x2F, 0xF4, 0x8C };
#elif FIRMWARE_VERSION == 6 // skep-pangyo-mvsprk-2111-04
  uint8_t devEui[] = { 0x27, 0x6b, 0x28, 0x31, 0xe1, 0x10, 0x74, 0xbf };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0xD6, 0xDE, 0x50, 0x8C, 0xEF, 0x2E, 0x40, 0x83, 0x10, 0x5C, 0xAB, 0x19, 0x9C, 0x5F, 0xF6, 0x56 };
#elif FIRMWARE_VERSION == 7 // skep-dalseo-gate-2111-01
  uint8_t devEui[] = { 0xfa, 0x7c, 0xbc, 0x00, 0x09, 0xfc, 0x95, 0x20 };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0x82, 0x46, 0x1E, 0xA7, 0xC7, 0x86, 0xA1, 0xC9, 0xD5, 0x97, 0xFE, 0x4A, 0x96, 0x6B, 0x9B, 0x5E };
#elif FIRMWARE_VERSION == 8 // skep-dalseo-sprk-2111-15
uint8_t devEui[] = { 0x7f, 0x70, 0xbb, 0xa7, 0x0c, 0xa7, 0x34, 0xd0 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0xDC, 0xDD, 0x06, 0x82, 0xA3, 0xA3, 0x4B, 0xB7, 0x08, 0xC2, 0x19, 0x22, 0x0C, 0x05, 0x2D, 0xD4 };
#elif FIRMWARE_VERSION == 9 // skep-dalseo-sprk-2111-16
uint8_t devEui[] = { 0x36, 0xe2, 0xf5, 0xa2, 0x58, 0x89, 0xa0, 0x4e };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0xD5, 0x97, 0x6B, 0x9C, 0x4B, 0x18, 0x19, 0x50, 0x4D, 0x5F, 0xE6, 0x93, 0xB8, 0x42, 0x01, 0xB5 };
#elif FIRMWARE_VERSION == 10 // skep-dalseo-sprk-2111-17
uint8_t devEui[] = { 0x2c, 0x7d, 0x1a, 0x2d, 0x11, 0x40, 0x3c, 0x43 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0xB5, 0x78, 0x19, 0x5D, 0xF6, 0xC8, 0x53, 0x5F, 0x79, 0x94, 0x25, 0x88, 0xDE, 0xC0, 0xB3, 0x22 };
#endif
 
/* ABP para*/
uint8_t nwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda,0x85 };
uint8_t appSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef,0x67 };
uint32_t devAddr =  ( uint32_t )0x007e6ae1;


// LoRaWAN example Sub-Band usage. change the channel to 8-15
uint16_t userChannelsMask[6]={ 0xFF00,0x0000,0x0000,0x0000,0x0000,0x0000 };
/*LoraWan channelsmask, default channels 0-7*/ 
//uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 16000;

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;

/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/* Application port */
uint8_t appPort = 2;

/* Valve control */
uint8_t valveStat;
uint8_t valveCommand[CHANNEL] = {0,};

/* Flowmeter */
char flowChar[5] = "0000";

uint8_t flowStep = 0;

union ulong_type {
    char bytes[4];
    unsigned long ulong_val;
}FL;

/* Battery */
char battChar[4] = "000";

union float_type {
    char bytes[4];
    float float_val;
}BT;


/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 8;

void setup() {
  boardInitMcu();
  Serial.begin(115200);
#if(AT_SUPPORT)
  enableAt();
#endif
  pinSetup();
  deviceState = DEVICE_STATE_INIT;
  LoRaWAN.ifskipjoin();
  
}

void loop()
{
  switch( deviceState )
  {
    case DEVICE_STATE_INIT:
    {
#if(AT_SUPPORT)
      getDevParam();
#endif
      printDevParam();
      LoRaWAN.init(loraWanClass,loraWanRegion);
      deviceState = DEVICE_STATE_JOIN;
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      prepareTxFrame( appPort );
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule next packet transmission
      txDutyCycleTime = appTxDutyCycle + randr( 0, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      LoRaWAN.sleep();
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}
