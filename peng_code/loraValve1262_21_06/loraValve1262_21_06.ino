
// LoRa max payload = 242bytes
// max schedule = 18
// protocol : 6byte (= msg_type, size, id, commnad, schedule number, schedule length)
//        + 13byte (= schedule commnad, schedule start time, schdule processing time)
// 242 - 6 - (13 * 18.15385)
//  

#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Wire.h>

// pins define
#define FLOW_PWR_ON     GPIO11
#define FLOW_IN         GPIO12
#define SEQUENCE_SWITCH GPIO7
#define VALVE_PWR_ON    GPIO5

char flowChar[5] = "0000";
char battChar[4] = "000";

union ulong_type {
    char bytes[4];
    unsigned long ulong_val;
}FL;

union float_type {
    char bytes[4];
    float float_val;
}BT;

int secondCommand = 0;

/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */
//uint8_t devEui[] = { 0x64, 0x85, 0xef, 0xff, 0x0a, 0xc6, 0x2a, 0x37 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0x4A, 0x0D, 0x35, 0xF9, 0x14, 0x58, 0xFD, 0xAA, 0xDF, 0xFE, 0xD6, 0x18, 0x61, 0xA1, 0xB9, 0x4B };


uint8_t devEui[] = { 0xc8, 0x85, 0x3b, 0x9d, 0x4e, 0xd9, 0x1f, 0x0c };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x17, 0xD5, 0xCF, 0xB0, 0x73, 0x4C, 0x4E, 0xF7, 0x5C, 0x8E, 0xA9, 0x2A, 0x94, 0x4B, 0x8D, 0x9C };

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
//uint32_t appTxDutyCycle = 10000;  // 10s
uint32_t appTxDutyCycle = 15000;

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
uint8_t confirmedNbTrials = 4;

/* DRV8871 Control ----------------------------------------------------------------------------------------------------------- */
#define TURN_ON_TIME    10      // ms                   // IC가 동작하도록 기다리는 시간
#define DELAY_TIME      75      // ms                   // valve 동작하는 시간
#define FULL_TIME       4320000 // ms 72min

unsigned long startGtime = 0x00000000;
unsigned long endGtime = 0x00000000;

uint8_t valveCommand = 0; 
uint8_t valveState = 0;
uint8_t valveStep = 0;
uint8_t valveStat;
unsigned long checkVtime = 0x00000000;
/* --------------------------------------------------------------------------------------------------------------------------- */

/* Flowmeter ----------------------------------------------------------------------------------------------------------------- */
#define FLOW_TIME       1000    // 1000ms = 1s          // 유량 측정 하는 시간

uint8_t flowStep = 0;
unsigned long checkFtime = 0x00000000;

/* --------------------------------------------------------------------------------------------------------------------------- */
/* Battery ----------------------------------------------------------------------------------------------------------------- */
uint8_t adcStep = 0;
/* --------------------------------------------------------------------------------------------------------------------------- */



// user function --------------------------------------------------------------
void pins_setup ()
{
  pinMode(VALVE_PWR_ON, OUTPUT);
//  digitalWrite(VALVE_PWR_ON, LOW);      // HIGH: enabled, LOW: disabled

  /* FLOW */
  pinMode(FLOW_PWR_ON, OUTPUT);
  digitalWrite(FLOW_PWR_ON, LOW) ;      // Active high
  pinMode(FLOW_IN, INPUT);

  pinMode(SEQUENCE_SWITCH, OUTPUT);
}


uint8_t nodeWoke = 0;
uint8_t tx_buf = 0;
/* Prepares the payload of the frame */

void setup() {
  boardInitMcu();
  Serial.begin(115200);
#if(AT_SUPPORT)
  enableAt();
#endif
  deviceState = DEVICE_STATE_INIT;
  LoRaWAN.ifskipjoin();

  pins_setup();
}

void loop()
{
  uint32_t msecTime = millis();
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
      start_flow_step();
      flow_sequence(msecTime);
      if(millis() - startGtime >= FULL_TIME && secondCommand){      // Wait Turn On Time
         relayGeneratorOff();
         secondCommand = 0;
      }
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
