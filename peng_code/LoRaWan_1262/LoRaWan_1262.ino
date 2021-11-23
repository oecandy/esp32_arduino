#include "LoRaWan_APP.h"
#include "Arduino.h"

/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

/* OTAA para*/
//uint8_t devEui[] = { 0x22, 0x32, 0x33, 0x00, 0x00, 0x88, 0x88, 0x02 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x66, 0x01 };

/* Test Device */
//uint8_t devEui[] = { 0xe4, 0xd5, 0xfd, 0x6a, 0x5c, 0x61, 0x9a, 0x18 };
//uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t appKey[] = { 0xf0, 0x57, 0xb9, 0x26, 0xc8, 0x0d, 0xe2, 0xb1, 0x97, 0xaa, 0x84, 0x07, 0x76, 0x40, 0x69, 0x3d };

/* OP Device */
uint8_t devEui[] = { 0xd3, 0xd5, 0xdf, 0x0b, 0x40, 0x5c, 0xae, 0x5f };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0xD9, 0x7F, 0x0C, 0x48, 0x1A, 0x8D, 0x48, 0xDD, 0xC4, 0xE9, 0x7D, 0xB1, 0xF8, 0xF3, 0xC6, 0xD6 };

/* ABP para*/
uint8_t nwkSKey[] = { 0xdc, 0x1a, 0xfe, 0x96, 0x43, 0x57, 0x56, 0x0d, 0x8d, 0xdf, 0xc1, 0x23, 0x36, 0x5a, 0xd7, 0x05 };
uint8_t appSKey[] = { 0x51, 0xfc, 0x57, 0xe8, 0xb1, 0x11, 0xc6, 0 xb3, 0x8a, 0x7c, 0x2a, 0xaa, 0x2e, 0x25, 0x2c, 0xb2 };
uint32_t devAddr =  ( uint32_t )0x00459766;

/*LoraWan channelsmask, default channels 0-7 { 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 }, sub-band-2 channels 8-15 { 0xFF00,0x0000,0x0000,0x0000,0x0000,0x0000 }*/ 
uint16_t userChannelsMask[6]={ 0x0000,0x00FF,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty 
.  value in [ms].*/
uint32_t appTxDutyCycle = 2000;

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

uint8_t valveStat;
uint8_t valveCommand = 0;
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

/* Prepares the payload of the frame */
static void prepareTxFrame( uint8_t port )
{
	/*appData size is LORAWAN_APP_DATA_MAX_SIZE which is defined in "commissioning.h".
	*appDataSize max value is LORAWAN_APP_DATA_MAX_SIZE.
	*if enabled AT, don't modify LORAWAN_APP_DATA_MAX_SIZE, it may cause system hanging or failure.
	*if disabled AT, LORAWAN_APP_DATA_MAX_SIZE can be modified, the max value is reference to lorawan region and SF.
	*for example, if use REGION_CN470, 
	*the max value for different DR can be found in MaxPayloadOfDatarateCN470 refer to DataratesCN470 and BandwidthsCN470 in "RegionCN470.h".
	*/
    appDataSize = 11;
    appData[0] = 0x42;
    appData[1] = 0x33;
    appData[2] = 0x35;
    appData[3] = 0x30;
    appData[4] = 0x56;
    if(valveCommand == 1){
      appData[5] = 0x31;
    } else {
      appData[5] = 0x30;
    }
    appData[6] = 0x46;
    appData[7] = 0x30;
    appData[8] = 0x30;
    appData[9] = 0x30;
    appData[10] = 0x30;
}

void relayOn(){
  delay(100);
  digitalWrite(GPIO14,LOW);
  delay(100);
  digitalWrite(GPIO14,HIGH);
}

void relayOff(){
  delay(100);
  digitalWrite(GPIO14,LOW);
}

void downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
  Serial.printf("+REV DATA:%s,RXSIZE %d,PORT %d\r\n",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  Serial.print("+REV DATA:");
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++)
  {
    Serial.printf("%02X",mcpsIndication->Buffer[i]);
  }
  Serial.println();
  valveStat = mcpsIndication->Buffer[5];
  Serial.printf("%02X",mcpsIndication->Buffer[5]);
  Serial.println();
  Serial.println(valveStat);
  if(valveStat == 49){
    valveCommand = 1;
    relayOn();
    delay(1000);
    relayOff();
    Serial.println("valve opened");
  } else if(valveStat == 48){
    valveCommand = 0;
    relayOff();
    Serial.println("valve closed");
  }
}

void setup() {
	boardInitMcu();
	Serial.begin(115200);
  pinMode(GPIO14,OUTPUT);
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
