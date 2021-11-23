#include "LoRaWan_APP.h"
#include "Arduino.h"

#include <ModbusMaster.h>
#include "softSerial.h"

// MODBUS settings
#define MAX485_DE                                   GPIO5
#define MAX485_RE_NEG                               GPIO6
#define MAX485_RXD                                  GPIO2
#define MAX485_TXD                                  GPIO1
#define REGISTER_SIZE                               10 // MODBUS register size
#define SLAVE_ID                                    1  // gas sensor 485 slave id

#define SENSOR_NO                                   5

/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

#if   SENSOR_NO == 1
  /* Pangyo fire sensor 1 */ 
  uint8_t devEui[] = { 0xb8, 0xe5, 0xf7, 0xfe, 0x7d, 0x47, 0x89, 0xb6 };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0xFE, 0xCB, 0xE1, 0xA9, 0x93, 0xD7, 0xA4, 0x37, 0xDA, 0xB9, 0x1D, 0x79, 0xD3, 0x1B, 0x4C, 0xEA };
#elif SENSOR_NO == 2
  /* Pangyo fire sensor 2 */ 
  uint8_t devEui[] = { 0x54, 0x13, 0x46, 0x6f, 0x4e, 0x00, 0xa9, 0x0e };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0x65, 0x15, 0x99, 0x0F, 0xBA, 0xC7, 0x59, 0xAF, 0xF5, 0x74, 0x45, 0x5A, 0x0E, 0xEA, 0x21, 0x83 };
#elif SENSOR_NO == 3
  /* Pangyo fire sensor 3 */ 
  uint8_t devEui[] = { 0x5f, 0x8b, 0x55, 0xc7, 0xba, 0x04, 0xaa, 0x84 };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0x4A, 0x11, 0x3C, 0x00, 0xA7, 0xA4, 0x60, 0x0B, 0xD7, 0xE2, 0x75, 0xE8, 0x63, 0x58, 0xC2, 0x8B };
#elif SENSOR_NO == 4
  /* Pangyo fire sensor 4 */ 
  uint8_t devEui[] = { 0x95, 0x00, 0x36, 0xaf, 0x23, 0x49, 0x25, 0xa6 };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0x61, 0x3A, 0x6B, 0xCA, 0xA1, 0x36, 0xF8, 0x64, 0x1A, 0x25, 0xDB, 0x1B, 0xF6, 0x19, 0x1F, 0x6A };
#elif SENSOR_NO == 5
  /* Pangyo fire sensor 5 */ 
  uint8_t devEui[] = { 0x14, 0x37, 0xba, 0x3d, 0x30, 0xed, 0xeb, 0x0b };
  uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t appKey[] = { 0xB4, 0x98, 0xDD, 0x25, 0x5F, 0xAA, 0x5F, 0x84, 0x20, 0xD5, 0x76, 0xC9, 0x17, 0x5F, 0xD9, 0xA0 };
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
uint32_t appTxDutyCycle = 10000;

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

/* Fire alarm status */
int fireState;

/* Temperature */
char tempChar[5] = "0000";

union ulong_type {
    char bytes[4];
    int int_val;
}TP;

/* Humidity */
char humiChar[5] = "0000";

union float_type {
    char bytes[4];
    int int_val;
}HM;

// modbus setting
softSerial softwareSerial(MAX485_TXD , MAX485_RXD);
ModbusMaster node;

int registerDatas[REGISTER_SIZE];
unsigned int readingCnt = 0;


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
  pinSetup();
  softwareSerial.begin(9600);
  setupModbus();
#if(AT_SUPPORT)
  enableAt();
#endif
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
