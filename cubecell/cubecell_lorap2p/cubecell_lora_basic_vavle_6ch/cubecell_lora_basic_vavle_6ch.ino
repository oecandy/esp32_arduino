#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <ArduinoJson.h>
#include <RCSwitch.h>

// Variables about LoraDevice Setting, This is not LoraWan solution, 
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
#define CHANNEL         6
#define VALVE_NO        1
#define DEVICE_TYPE     3 // 1: 1ch relay, 2: 2ch relay, 3: 6ch relay, 11: buzzer, 10: fire sensor

#define INTERVAL_TIME   6000
#define SWING_INTERVAL  8000

#define RESET_TIME      300000
#define SWING_TURN      2000
#define RF_RX_PIN       22

// define TX/RX packet size
char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

// callback events about RadioEvents defined in the [callback] tab
static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

unsigned long code_switch_pan_on_off = 4460136; //decimal RF433 code
unsigned long code_switch_pump_on_off = 4460132; //decimal RF433 code
unsigned long code_switch_lifting_on_off = 4460140; //decimal RF433 code
unsigned long code_switch_right_on_off = 4460130; //decimal RF433 code
unsigned long code_switch_left_on_off = 4460138; //decimal RF433 code
unsigned long code_switch_6 = 4460134;

// reserve buffer for Json structs
StaticJsonDocument<BUFFER_SIZE> doc;
int relayStatus[CHANNEL] = {};
int relayStatusChange = false;
int switchNo = 1;

unsigned long intervalMillis = 0;
unsigned long swingMillis = 0;
int swingFlag = 1;
int turnFlag = 0;
int loop_finish = 0;

int timeFlag = 0;
int dir = 0; // 0 : 정지, 1 : left, 2 : right
RCSwitch mySwitch = RCSwitch();

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

void setup() {
    boardInitMcu( );
    Serial.begin(9600);
    Serial.setTimeout(100);
    loraSetup();   // in [lora]
    RcSensorSetup();
    //OnTxDone();
}

void loop(){
	switch(state){
		case TX:
        processTxJob();   // in [lora]
		    break;
		case RX:
			  processRxJob();   // in [lora]
		    break;
		case LOWPOWER:
		    break;
    default:
        Serial.println("default state");
        break;
	}
  //processReady();   // in [lora]
  swing_loop();
  Radio.IrqProcess( );
}
