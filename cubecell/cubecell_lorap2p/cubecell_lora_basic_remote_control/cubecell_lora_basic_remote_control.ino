#include "LoRaWan_APP.h"
#include "Arduino.h"

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
  PINMODE_INPUT_PULLUP(SWITCH_01);
  PINMODE_INPUT_PULLUP(SWITCH_02);
  PINMODE_INPUT_PULLUP(SWITCH_03);
  PINMODE_INPUT_PULLUP(SWITCH_04);
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
    Serial.begin(115200);
    attachInterrupt(digitalPinToInterrupt(SWITCH_01),onWakeUp1,CHANGE);
    attachInterrupt(digitalPinToInterrupt(SWITCH_02),onWakeUp2,CHANGE);
    attachInterrupt(digitalPinToInterrupt(SWITCH_03),onWakeUp3,CHANGE);
    attachInterrupt(digitalPinToInterrupt(SWITCH_04),onWakeUp4,CHANGE);
    
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
