#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <ArduinoJson.h>

// Variables abo\\\\\ut LoraDevice Setting, This is not LoraWan solution, 
// only using LoRaWan Library that why the names of variables contain 'LoRaWAN'.
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

#define RF_FREQUENCY                                915000000 // Hz

#define TX_OUTPUT_POWER                             14        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 250 // Define the payload size here

// device profiles
#define CHANNEL         1
#define DEVICE_TYPE     11 // 1: 1ch relay, 2: 2ch relay, 3: 6ch relay

#define SWITCH_01       GPIO5
#define SWITCH_02       GPIO6
#define SWITCH_03       GPIO11
#define SWITCH_04       GPIO12

//#define FLOW_METER_PIN  7
#define INTERVAL_TIME   6000

// define TX/RX packet size
char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

// callback events about RadioEvents defined in the [callback] tab
static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

// reserve buffer for Json structs
StaticJsonDocument<BUFFER_SIZE> doc;

// time var for tx timing
unsigned long txMillis = 0;

// struct for radio states
typedef enum{
    LOWPOWER,
    RX,
    TX
}States_t;

int16_t txNumber;
States_t state;
bool sleepMode = false;
int16_t Rssi,rxSize;

void pinSetup(){
  pinMode(SWITCH_01,INPUT_PULLUP);
  pinMode(SWITCH_02,INPUT_PULLUP);
  pinMode(SWITCH_03,INPUT_PULLUP);
  pinMode(SWITCH_04,INPUT_PULLUP);
}

// switch state
int switch1State = 0;
int switch2State = 0;
int switch3State = 0;
int switch4State = 0;

int switchNo = 0;
int switchStatus = 0;

void setup() {
    
    boardInitMcu( );
    Serial.begin(9600);
    attachInterrupt(SWITCH_01,onWakeUp1,FALLING);
    attachInterrupt(SWITCH_02,onWakeUp2,FALLING);
    attachInterrupt(SWITCH_03,onWakeUp3,FALLING);
    attachInterrupt(SWITCH_04,onWakeUp4,FALLING);
    
    pinSetup();
    loraSetup();   // in [lora]
}

void loop(){
	switch(state){
		case TX:
        processTxJob();   // in [lora]
		    break;
		case RX:
		    break;
		case LOWPOWER:
        lowPowerHandler();
		    break;
    default:
        break;
	}
  Radio.IrqProcess( );
}
