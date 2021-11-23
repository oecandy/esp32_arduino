
//
//
//  2020-10-15
// 시간 저장 하는 부분 구현
// 이벤트 발생 하면 저장 하는 부분 구현
//
//

// //
// //
// //  2020-10-06
// //  1. 송,수신 변경 (변경 전 > 변경 후)
// //  - valve가 정해진 시간마다 송신 > 정해진 시간마다 수신
// //  - 수신 데이터에 따라서 동작 or 응답
// //  - valve 동작 제외하고 송,수신 먼저 코딩
// //
// //
// //  2020-07-03
// //  - A2 | PA2 | TX2 회로도 적용하면 동작하지 않음. 추후 원인 찾기
// //  - 네오 출고 시료는 A4 포트로 변경
// //
// //
// //  2020-06-02
// //  - check list 1 = 통신 전송하여 밸브1, 밸브2 ON / OFF 동작하는지 확인
// //  - check list 2 = 통신 끊겼을 때 밸브 닫는지 확인 (check_com_count 값을 10에서 낮은 값으로 변경하여 빠른 확인)
// //  - check list 3 = 통신 재접속하면 밸브1, 밸브2 ON / OFF 다시 동작하는지 확인
// //  - check list 3 = 통신 끊겼을 때 reset 후 다시 JOIN 하는지 확인 (check_com_count 값을 30에서 낮은 값으로 변경하여 빠른 확인)
// //  - check list 4 = 낮은 전압에서 밸브 ON / OFF 동작시키면 동작하는지 확인
// //
// //
// //  2020-05-18
// //  - 코딩 시작
// //
// //


#include <STM32L0.h>
#include "LoRaWAN.h"
#include <ThreeWire.h>  
#include <RtcDS1302.h>

#define DEBUG_MODE      1


enum message_type
{
    CODE,
    UPDATE_DEVICE_REQUEST,
    UPDATE_DEVICE_RESPONSE, 
    REPORT_DEVICE_REQUEST,
    REPORT_DEVICE_RESPONSE,
    PING_REQUEST,
    PING_RESPONSE
};

enum command_type
{
    TYPE,
    OPEN,
    CLOSE,
    SCHEDULE,
    PAUSE_SCHEDULE,
    RESUME_SCHEDULE,
    CANCEL_SCHEDULE,
    OFF,
    REPORT_SCHEDULE
};


union long_long_type {
    char bytes[8];
    long long long_long_val;
};

union long_type {
    char bytes[4];
    long long_val;
};

uint8_t command_code;
uint8_t message_size;
uint8_t response_id;
uint8_t update_command;

uint8_t temp_mode = 0;


byte packetArr[250] = { 0, };
byte LoRa_step = 0;


/* Applications/Development_Device_OTAA/Devices/test-lora-valve-otaa_03 */
const char *appEui = "0000000000000000";
const char *appKey = "2F52315BB17E89E84DB1A932194F61E5";
const char *devEui = "06f60b16dabb2f1e";


void requestLoRa (void)
{
    if (LoRaWAN.parsePacket())
    {
        uint32_t size;
        uint8_t data[250];
        uint8_t i;

        size = LoRaWAN.read(&data[0], sizeof(data));

        if(size)
        {
        #if DEBUG_MODE
            Serial.println("r1");
        #endif
            command_code = data[0];
            message_size = data[1];
            response_id = data[2];

            if(command_code == 1)        // UpdateDeviceRequest
            {
                if(message_size == 2)    // size 2
                    update_command = data[3];
                else
                {
// 추후에 수신 및 이벤트 동작 구현
/*                    for (int i = 0; i < event_count; i++)
                    {
                        buffer[index++] = events[i].type; //event type
                        buffer[index++] = get_event_chunk_size(&events[i]) - 2; //event size
                        buffer[index++] = events[i].event_status;

                        if((events[i].event_status == 2) ||
                            (events[i].event_status == 3))
                        {
                            int j;
                            for (j = 0 ; j < 4; j++)
                            {
                                buffer[index++] = events[i].value.bytes[j];
                            }
                        }

                        if (events[i].is_time_exists)
                        {
                            int j;
                            for (j = 0; j < 8; j++)
                            {
                                buffer[index++] = events[i].time.bytes[j];
                            }
                        }
                    }*/
                }
            }
            else if(command_code == 4)  // ReportDeviceResponse
            {
            }
            else if(command_code == 5)  // PingRequest
            {
                
            }

            ++ LoRa_step;
        }
    }
}

void responseLoRa (void)
{
    switch(command_code)
    {
        case UPDATE_DEVICE_REQUEST:
            #if DEBUG_MODE
                Serial.println("t1");
            #endif
                packetArr[0] = UPDATE_DEVICE_RESPONSE;
                packetArr[1] = 2;
                packetArr[2] = response_id;
                packetArr[3] = 0;

                if(!LoRaWAN.busy() && LoRaWAN.joined())
                {
                    LoRaWAN.beginPacket();
                    LoRaWAN.write(packetArr, 4);
                    LoRaWAN.endPacket();

                #if DEBUG_MODE
                    Serial.print("[ ");
                    Serial.print(packetArr[0]);     Serial.print(" ");
                    Serial.print(packetArr[1]);     Serial.print(" ");
                    Serial.print(packetArr[2]);     Serial.print(" ");
                    Serial.print(packetArr[3]);     Serial.print(" ");
                    Serial.println(" ]");
                #endif
                }

                if(update_command == REPORT_SCHEDULE)
                    command_code = REPORT_DEVICE_REQUEST;
                else
                    command_code = 0;
            break;

        case REPORT_DEVICE_REQUEST:
            #if DEBUG_MODE
                Serial.println("t2");
            #endif
                packetArr[0] = REPORT_DEVICE_REQUEST;
                packetArr[2] = response_id;
                if(temp_mode == 0)      // valve가 open 된 것이 성공한 이벤트 하나를 시간 없이
                {
                    packetArr[1] = 5;   // msg size
                    packetArr[3] = 1;   // event count
                    packetArr[4] = 1;   // 1: valve open
                    packetArr[5] = 1;   // event size
                    packetArr[6] = 0;   // 0 : success

                    if(!LoRaWAN.busy() && LoRaWAN.joined())
                    {
                        LoRaWAN.beginPacket();
                        LoRaWAN.write(packetArr, 7);
                        LoRaWAN.endPacket();

                    #if DEBUG_MODE
                        Serial.print("[ ");
                        Serial.print(packetArr[0]);     Serial.print(" ");
                        Serial.print(packetArr[1]);     Serial.print(" ");
                        Serial.print(packetArr[2]);     Serial.print(" ");
                        Serial.print(packetArr[3]);     Serial.print(" ");
                        Serial.print(packetArr[4]);     Serial.print(" ");
                        Serial.print(packetArr[5]);     Serial.print(" ");
                        Serial.print(packetArr[6]);     Serial.print(" ");
                        Serial.println(" ]");
                    #endif
                    }
                }
                else if(temp_mode == 1) // 특정 시간에 측정된 센서의 값 하나
                {
                    packetArr[1] = 17;  // msg size
                    packetArr[3] = 1;   // event count
                    packetArr[4] = 4;   // 4: sensor value
                    packetArr[5] = 13;  // event size
                    packetArr[6] = 2;   // 2: floating value 4bytes
                    packetArr[7] = 3;   // 1byte
                    packetArr[8] = 4;   // 2byte
                    packetArr[9] = 5;   // 3byte
                    packetArr[10] = 6;  // 4byte
                    packetArr[11] = 0;  // time 1byte
                    packetArr[12] = 0;  // time 2byte
                    packetArr[13] = 0;  // time 3byte
                    packetArr[14] = 23; // time 4byte
                    packetArr[15] = 12; // time 5byte
                    packetArr[16] = 13; // time 6byte
                    packetArr[17] = 45; // time 7byte
                    packetArr[18] = 0;  // time 8byte

                    if(!LoRaWAN.busy() && LoRaWAN.joined())
                    {
                        LoRaWAN.beginPacket();
                        LoRaWAN.write(packetArr, 19);
                        LoRaWAN.endPacket();

                    #if DEBUG_MODE
                        Serial.print("[ ");
                        Serial.print(packetArr[0]);     Serial.print(" ");
                        Serial.print(packetArr[1]);     Serial.print(" ");
                        Serial.print(packetArr[2]);     Serial.print(" ");
                        Serial.print(packetArr[3]);     Serial.print(" ");
                        Serial.print(packetArr[4]);     Serial.print(" ");
                        Serial.print(packetArr[5]);     Serial.print(" ");
                        Serial.print(packetArr[6]);     Serial.print(" ");
                        Serial.print(packetArr[7]);     Serial.print(" ");
                        Serial.print(packetArr[8]);     Serial.print(" ");
                        Serial.print(packetArr[9]);     Serial.print(" ");
                        Serial.print(packetArr[10]);    Serial.print(" ");
                        Serial.print(packetArr[11]);    Serial.print(" ");
                        Serial.print(packetArr[12]);    Serial.print(" ");
                        Serial.print(packetArr[13]);    Serial.print(" ");
                        Serial.print(packetArr[14]);    Serial.print(" ");
                        Serial.print(packetArr[15]);    Serial.print(" ");
                        Serial.print(packetArr[16]);    Serial.print(" ");
                        Serial.print(packetArr[17]);    Serial.print(" ");
                        Serial.print(packetArr[18]);    Serial.print(" ");
                        Serial.println(" ]");
                    #endif
                    }
                }
                else if(temp_mode == 2) // 특정 시각에 밸브가 Opne되고 Close된 내용
                {
                    packetArr[1] = 23;  // msg size
                    packetArr[3] = 2;   // event count
                    packetArr[4] = 1;   // 1: valve open
                    packetArr[5] = 9;   // event size
                    packetArr[6] = 0;   // 0: success
                    packetArr[7] = 0;   // time[0]
                    packetArr[8] = 0;   // time[1]
                    packetArr[9] = 0;   // time[2]
                    packetArr[10] = 23; // time[3]
                    packetArr[11] = 12; // time[4]
                    packetArr[12] = 14; // time[5]
                    packetArr[13] = 45; // time[6]
                    packetArr[14] = 0;  // time[7]
                    packetArr[15] = 0;  // 0: valve close
                    packetArr[16] = 9;  // event size
                    packetArr[17] = 0;  // 0: success
                    packetArr[18] = 0;  // time[0]
                    packetArr[19] = 0;  // time[1]
                    packetArr[20] = 0;  // time[2]
                    packetArr[21] = 23; // time[3]
                    packetArr[22] = 13; // time[4]
                    packetArr[23] = 14; // time[5]
                    packetArr[24] = 45; // time[6]
                    packetArr[25] = 0;  // time[7]

                    if(!LoRaWAN.busy() && LoRaWAN.joined())
                    {
                        LoRaWAN.beginPacket();
                        LoRaWAN.write(packetArr, 26);
                        LoRaWAN.endPacket();

                #if DEBUG_MODE
                    Serial.print("[ ");
                    Serial.print(packetArr[0]);     Serial.print(" ");
                    Serial.print(packetArr[1]);     Serial.print(" ");
                    Serial.print(packetArr[2]);     Serial.print(" ");
                    Serial.print(packetArr[3]);     Serial.print(" ");
                    Serial.print(packetArr[4]);     Serial.print(" ");
                    Serial.print(packetArr[5]);     Serial.print(" ");
                    Serial.print(packetArr[6]);     Serial.print(" ");
                    Serial.print(packetArr[7]);     Serial.print(" ");
                    Serial.print(packetArr[8]);     Serial.print(" ");
                    Serial.print(packetArr[9]);     Serial.print(" ");
                    Serial.print(packetArr[10]);    Serial.print(" ");
                    Serial.print(packetArr[11]);    Serial.print(" ");
                    Serial.print(packetArr[12]);    Serial.print(" ");
                    Serial.print(packetArr[13]);    Serial.print(" ");
                    Serial.print(packetArr[14]);    Serial.print(" ");
                    Serial.print(packetArr[15]);    Serial.print(" ");
                    Serial.print(packetArr[16]);    Serial.print(" ");
                    Serial.print(packetArr[17]);    Serial.print(" ");
                    Serial.print(packetArr[18]);    Serial.print(" ");
                    Serial.print(packetArr[19]);    Serial.print(" ");
                    Serial.print(packetArr[20]);    Serial.print(" ");
                    Serial.print(packetArr[21]);    Serial.print(" ");
                    Serial.print(packetArr[22]);    Serial.print(" ");
                    Serial.print(packetArr[23]);    Serial.print(" ");
                    Serial.print(packetArr[24]);    Serial.print(" ");
                    Serial.print(packetArr[25]);    Serial.print(" ");
                    Serial.println(" ]");
                #endif
                    }
                }

                if(temp_mode > 1)   temp_mode = 0;
                else                ++ temp_mode;

                command_code = 0;
            break;

        case PING_REQUEST:
            #if DEBUG_MODE
                Serial.println("t3");
            #endif
                packetArr[0] = PING_RESPONSE;
                packetArr[1] = message_size;
                packetArr[2] = response_id;
                if(!LoRaWAN.busy() && LoRaWAN.joined())
                {
                
                    LoRaWAN.beginPacket();
                    LoRaWAN.write(packetArr, 3);
                    LoRaWAN.endPacket();

                #if DEBUG_MODE
                    Serial.print("[ ");
                    Serial.print(packetArr[0]);     Serial.print(" ");
                    Serial.print(packetArr[1]);     Serial.print(" ");
                    Serial.print(packetArr[2]);     Serial.print(" ");
                    Serial.println(" ]");
                #endif
                }
                command_code = 0;
            break;

        default: break;
    }
}


void setup()
{
#if DEBUG_MODE
    Serial.begin(115200);
    while (!Serial) { }
    Serial.println("POWER(ON)");
#endif

    // LoRa setup
    // LoRa_setup();
// void LoRa_setup (void)
// {
    // US915 settting
    LoRaWAN.begin(US915);
    LoRaWAN.setSubBand(2);

// #if DEBUG_MODE
//     Serial.print("chMask = ");
//     Serial.print(ChannelsMask[0]);
//     Serial.print(ChannelsMask[1]);
//     Serial.print(ChannelsMask[2]);
//     Serial.print(ChannelsMask[3]);
//     Serial.print(ChannelsMask[4]);
// #endif

    LoRaWAN.setAntennaGain(2.0);
    LoRaWAN.setTxPower(20);
    LoRaWAN.setADR(false);
    LoRaWAN.setDataRate(2);

    LoRaWAN.joinOTAA(appEui, appKey, devEui);
// }
}

unsigned long previousMillis = millis();
void loop()
{
    unsigned long currentMillis = millis();

    switch(LoRa_step)
    {
        case 0:
            if (currentMillis - previousMillis >= 2000) {
                previousMillis = currentMillis;

            #if DEBUG_MODE
                Serial.print("s1 ");
            #endif

                packetArr[0] = PING_RESPONSE;
                packetArr[1] = 1;
                packetArr[2] = 99;
                if(!LoRaWAN.busy() && LoRaWAN.joined())
                {
                
                    LoRaWAN.beginPacket();
                    LoRaWAN.write(packetArr, 3);
                    LoRaWAN.endPacket();

                #if DEBUG_MODE
                    Serial.print("[ ");
                    Serial.print(packetArr[0]);     Serial.print(" ");
                    Serial.print(packetArr[1]);     Serial.print(" ");
                    Serial.print(packetArr[2]);     Serial.print(" ");
                    Serial.println(" ]");
                #endif
                }

                ++ LoRa_step;
            }
            break;

        case 1:
            if (currentMillis - previousMillis >= 3000) {
                previousMillis = currentMillis;
                ++ LoRa_step;
            }
            else
                requestLoRa();
            break;

        case 2:
            responseLoRa();
            LoRa_step = 0;
            break;

        default: break;
    }
}



// #include <STM32L0.h>
// #include "LoRaWAN.h"
// #include <ThreeWire.h>  
// #include <RtcDS1302.h>

// #define DEBUG_MODE      0


// /* pins */
// #define ADC_PRESSURE    19      // A3 | PA3 | RX2
// #define FLOW2_IN        20      // A4
// // #define FLOW2_IN        18      // A2 | PA2 | TX2
// #define FLOW1_IN        17      // A1 | PA5 | DAC1
// #define ADC_BAT         16      // A0 | PA4 | DAC0
// #define FLOW2_PWR_SW    15      // SCL | PB8
// #define FLOW1_PWR_SW    14      // SDA | PB9
// #define DS1302_CLK      13      // D13 | PB13 | D13
// #define DS1302_IO       12      // D12 | PB14 | MISO
// #define DS1302_CE       11      // D11 | PB15 | MOSI
// #define PWR_3_3VDC_MODE 10      // D10 | PB12 | CS
// #define VALVE_PWR_ON    9       // D9 | PA14 | TX3
// #define VALVE_PWR_SW    8       // D8 | PA13 | RX3
// #define VALVE2_IN2      5       // D5 | PB2
// #define VALVE2_IN1      4       // D4 | PB7
// #define VALVE1_IN2      3       // D3 | PB6
// #define VALVE1_IN1      2       // D2 | PB5
// #define PWR_5VDC_EN     1       // D1 | PA9 | TX1

// #define DELAY_TIME      75      // ms, valve 사양서에는 pulse width 20~500msec이지만 실제 50ms가 넘어야 동작
// #define FLOW_TIME       1000    // ms, 유량값 읽어오는 시간
// #define RECEIVE_TIME    3000    // ms, LoRa 데이터 수신대기하는 시간

// #define VALVE_MAX_STEP  9

// /* variables ---------------------------------------------------------------------------------------------------- */
// //                Battery Voltage   VALVE1    FLOW1     VALVE2    FLOW2     Pressure Voltage
// //                        3.76VDC   1=ON                1=ON                   4.50VDC
// //                     0x0178=376   0=OFF  0x270F=9999  0=OFF  0x270F=9999  0x01C2=450    'X'
// byte packetArr[11] = { 0x01, 0x78,  0x00,  0x27, 0x0F,  0x00,  0x27, 0x0F,  0x01, 0xC2,  0x58 };

// //                      F     V   -   2     0     2     0   -   0     6   -   0     2  -  0
// byte fwVersion[11] = { 0x46, 0x56,   0x32, 0x30, 0x32, 0x30,  0x30, 0x36,  0x30, 0x32,  0x30 };
// byte firmware_version = 0;

// word pulse1_cnt = 0;
// word pulse2_cnt = 0;

// byte valve1step = 9, valve2step = 9;
// byte valve1command = 0, valve2command = 0;
// byte valve1state = 0, valve2state = 0;
// uint32_t checkTime1 = 0, checkTime2 = 0;
// byte check_com_count = 0;
// byte valveOFF = 0;

// word batteryLevel = 0;
// word pressureLevel = 0;

// byte mainStep = 1;
// unsigned long mainTime = 0;
// byte LoRaStep = 0;
// byte LoRaCount = 0;
// unsigned long LoRaTime = 0;


// ThreeWire myWire(DS1302_IO,DS1302_CLK,DS1302_CE); // IO, SCLK, CE
// RtcDS1302<ThreeWire> Rtc(myWire);


// /* Applications/Development_Device_OTAA/Devices/test-lora-valve-otaa_03 */
// const char *appEui = "0000000000000000";
// const char *appKey = "2F52315BB17E89E84DB1A932194F61E5";
// const char *devEui = "06f60b16dabb2f1e";


// // #define DEVICE_NUMBER   1
// // #if DEVICE_NUMBER == 1 /* Applications/Development_Device_OTAA/Devices/test-lora-valve-otaa_01 */
// // const char *appEui = "0000000000000000";
// // const char *appKey = "5F297F25DE28A5B05B93719A89DFE679";
// // const char *devEui = "009fe879284dcd45";
// // #endif
// // mqtt connection = lora.farota.com:1883, Topic = application/1/device/devEui16serial/tx
// // 'V''A''L''V''E''1''1' valve1 ON : {"fPort":1,"data":"VkFMVkUxMQ==","confirmed":true}
// // 'V''A''L''V''E''1''0' valve1 OFF : {"fPort":1,"data":"VkFMVkUxMA==","confirmed":true}
// // 'V''A''L''V''E''2''1' valve2 ON : {"fPort":1,"data":"VkFMVkUyMQ==","confirmed":true}
// // 'V''A''L''V''E''2''0' valve2 OFF : {"fPort":1,"data":"VkFMVkUyMA==","confirmed":true}


// /* 한국농어촌공사 */
// /* Applications/valve-otaa/Devices/korea-rural-corporation-fert-main-valve-otaa-01 */
// // const char *appEui = "0000000000000000";
// // const char *appKey = "D595F056B7DF0CFE13480709E91AD139";
// // const char *devEui = "4ec18dffbb89f341";


// /* 아이들 */
// /* Applications/valve-otaa/Devices/idle-main-fert-valve-with-flowmeter-otaa-01 */
// // const char *appEui = "0000000000000000";
// // const char *appKey = "F009BBCEBD3BE4A5CB5B8E159D7F7EF9";
// // const char *devEui = "9af727132bc47c7b";


// union long_long_type {
//     uint8_t bytes[8];
//     long long long_long_val;
// };

// union int_type {
//     uint8_t bytes[4];
//     long long_val;
// };




// void pins_setup (void)
// {
//     // Water Pressure Sensor power enable pin Configuration
//     pinMode(PWR_5VDC_EN, OUTPUT);
//     digitalWrite(PWR_5VDC_EN, LOW);      // HIGH: enabled, LOW: disabled


//     // drv8871 control pin Configuration (sleep mode = IN1 and IN2 low)
//     pinMode(VALVE1_IN1, OUTPUT);       // IN1 0, IN2 0 = sleep entered after 1 ms
//     digitalWrite(VALVE1_IN1, LOW);     // IN1 0, IN2 1 = Reverse (Current OUT2 → OUT1)
//     pinMode(VALVE1_IN2, OUTPUT);       // IN1 1, IN2 0 = Forward (Current OUT1 → OUT2)
//     digitalWrite(VALVE1_IN2, LOW);     // IN1 1, IN2 1 = Brake; low-side slow decay

//     pinMode(VALVE2_IN1, OUTPUT);       // IN1 0, IN2 0 = sleep entered after 1 ms
//     digitalWrite(VALVE2_IN1, LOW);     // IN1 0, IN2 1 = Reverse (Current OUT2 → OUT1)
//     pinMode(VALVE2_IN2, OUTPUT);       // IN1 1, IN2 0 = Forward (Current OUT1 → OUT2)
//     digitalWrite(VALVE2_IN2, LOW);     // IN1 1, IN2 1 = Brake; low-side slow decay


//     // valve power enable pin Configuration
//     pinMode(VALVE_PWR_SW, OUTPUT);
//     digitalWrite(VALVE_PWR_SW, LOW);      // HIGH: enabled, LOW: disabled

//     pinMode(VALVE_PWR_ON, OUTPUT);
//     digitalWrite(VALVE_PWR_ON, LOW);      // HIGH: enabled, LOW: disabled


//     // battery enable pin Configuration
//     pinMode(PWR_3_3VDC_MODE, OUTPUT);
//     digitalWrite(PWR_3_3VDC_MODE, HIGH);      // HIGH : boost mode, LOW : bypass mode


//     // Real-Time Clock pin Configuration
//     pinMode(DS1302_CE, OUTPUT);
//     pinMode(DS1302_IO, OUTPUT);
//     pinMode(DS1302_CLK, OUTPUT);


//     // flow meter power enable pin Configuration
//     pinMode(FLOW1_PWR_SW, OUTPUT);
//     digitalWrite(FLOW1_PWR_SW, LOW);      // Active high
//     pinMode(FLOW2_PWR_SW, OUTPUT);
//     digitalWrite(FLOW2_PWR_SW, LOW);      // Active high

//     // flow meter signal input pin Configuration
//     pinMode(FLOW1_IN, INPUT);
//     pinMode(FLOW2_IN, INPUT);
// }


// /* DRV8871 Control ----------------------------------------------------------------------------------------------------------- */
// void Start_Valve1_OFF (void)
// {
//     valve1command = 0;

//     if(valve1state)                  // 밸브가 켜져있으면
//         valve1step = 1;              // 밸브 동작
//     else                             // 밸브가 꺼져있으면
//         valve1step = VALVE_MAX_STEP; // 밸브는 동작하지 않음
// }

// void Start_Valve2_OFF (void)
// {
//     valve2command = 0;

//     if(valve2state)                  // 밸브가 켜져있으면
//         valve2step = 1;              // 밸브 동작
//     else                             // 밸브가 꺼져있으면
//         valve2step = VALVE_MAX_STEP; // 밸브는 동작하지 않음
// }

// void Init_Valve1 (void)
// {
//     digitalWrite(VALVE1_IN1, LOW);     // Coast;
//     digitalWrite(VALVE1_IN2, LOW);
// }

// void Valve1_Operation (byte sel)
// {
//     if(sel)
//     {
//         digitalWrite(VALVE1_IN1, LOW);     // Reverse (Current OUT2 → OUT1)
//         digitalWrite(VALVE1_IN2, HIGH);
//     }
//     else
//     {
//         digitalWrite(VALVE1_IN1, HIGH);    // Forward (Current OUT1 → OUT2)
//         digitalWrite(VALVE1_IN2, LOW);
//     }
// }

// void Init_Valve2 (void)
// {
//     digitalWrite(VALVE2_IN1, LOW);     // Coast;
//     digitalWrite(VALVE2_IN2, LOW);
// }

// void Valve2_Operation (byte sel)
// {
//     if(sel)
//     {
//         digitalWrite(VALVE2_IN1, LOW);     // Reverse (Current OUT2 → OUT1)
//         digitalWrite(VALVE2_IN2, HIGH);
//     }
//     else
//     {
//         digitalWrite(VALVE2_IN1, HIGH);    // Forward (Current OUT1 → OUT2)
//         digitalWrite(VALVE2_IN2, LOW);
//     }
// }


// void Valve_Control (unsigned long waitTime, byte *valveStep, byte *valveCommand, uint32_t *checkTime, byte selValve, byte *valvestate)
// {
//     switch(*valveStep)
//     {
//         case 1:
//             digitalWrite(VALVE_PWR_SW, HIGH);    // ENABLE(ON) Active High
//             digitalWrite(VALVE_PWR_ON, HIGH);    // Pull high to enable the device, pull low to disable the device.
//             *checkTime = millis();
//             ++ *valveStep;
//         #if DEBUG_MODE
//             Serial.print("v1s=");
//             Serial.print(valve1step);
//             Serial.print(", v2s=");
//             Serial.println(valve2step);
//         #endif
//             break;

//         case 2:
//             if(waitTime - *checkTime >= 10)      // Wait Turn On Time
//                 ++ *valveStep;
//             break;

//         case 3:
//             if(selValve == 1)       // 1번 밸브가 선택되었으면
//             {
//                 if(*valveCommand)
//                     Valve1_Operation(1);  // ON
//                 else
//                     Valve1_Operation(0);  // OFF
//             }
//             else
//             {
//                 if(*valveCommand)
//                     Valve2_Operation(1);  // ON
//                 else
//                     Valve2_Operation(0);  // OFF
//             }
            
//             *checkTime = millis();
//             ++ *valveStep;
//         #if DEBUG_MODE
//             Serial.print("v1s=");
//             Serial.print(valve1step);
//             Serial.print(", v2s=");
//             Serial.println(valve2step);
//         #endif
//             break;

//         case 4:
//             if(waitTime - *checkTime >= DELAY_TIME)     // Operating pulse width
//                 ++ *valveStep;
//             break;

//         case 5:
//             digitalWrite(VALVE_PWR_SW, LOW);
//             digitalWrite(VALVE_PWR_ON, LOW);
//             ++ *valveStep;
//         #if DEBUG_MODE
//             Serial.print("v1s=");
//             Serial.print(valve1step);
//             Serial.print(", v2s=");
//             Serial.println(valve2step);
//         #endif
//             break;

//         case 6:
//             if(selValve == 1)
//                 Init_Valve1();
//             else
//                 Init_Valve2();
//             ++ *valveStep;
//         #if DEBUG_MODE
//             Serial.print("v1s=");
//             Serial.print(valve1step);
//             Serial.print(", v2s=");
//             Serial.println(valve2step);
//         #endif
//             break;

//         case 7:
//             *valvestate = *valveCommand;
//             ++ *valveStep;
//         #if DEBUG_MODE
//             Serial.print("v1s=");
//             Serial.print(valve1step);
//             Serial.print(", v2s=");
//             Serial.println(valve2step);
//         #endif
//             break;

//         case 8:
//             ++ *valveStep;
//         #if DEBUG_MODE
//             Serial.print("v1s=");
//             Serial.print(valve1step);
//             Serial.print(", v2s=");
//             Serial.println(valve2step);
//         #endif
//             break;
//         default: break;
//     }
// }

// void Check_Valve (void)             // 통신이 끊기고 일정 시간이 지나면 밸브가 꺼지는 함수
// {
//     //  check list 2, check list 3
//     if( (check_com_count > 10) && (!valveOFF) && ((valve1state)||(valve2state))     // 10번 전송하는 동안 통신이 끊기고, 밸브 OFF 동작을 한적이 없고, 밸브가 켜져 있고
//         && ((valve1step == VALVE_MAX_STEP)&&(valve2step == VALVE_MAX_STEP)) )       // 현재 밸브 동작이 진행중이 아니면
//     {
//         valveOFF = 1;

//         if(valve1state) // 밸브1이 켜져 있으면
//         {
//             Start_Valve1_OFF();

//             while (valve1step < VALVE_MAX_STEP)  // confirm을 10초씩 대기하는 경우가 생기므로
//             {                                   // 다른 함수 진입하지 않고 while에서 밸브 꺼지도록 처리
//                 unsigned long countMillis = millis();

//                 Valve_Control(countMillis, &valve1step, &valve1command, &checkTime1, 1, &valve1state);
//             }
//         }

//         if(valve2state) // 밸브2가 켜져 있으면
//         {
//             Start_Valve2_OFF();

//             while (valve2step < VALVE_MAX_STEP)  // confirm을 10초씩 대기하는 경우가 생기므로
//             {                                   // 다른 함수 진입하지 않고 while에서 밸브 꺼지도록 처리
//                 unsigned long countMillis = millis();

//                 Valve_Control(countMillis, &valve2step, &valve2command, &checkTime2, 2, &valve2state);
//             }
//         }
//     }
// }

// /* --------------------------------------------------------------------------------------------------------------------------- */


// /* Flowmeter ----------------------------------------------------------------------------------------------------------------- */
// void getFlow1 (void)
// {
//     ++ pulse1_cnt;    //  low가 인식, high가 미인식
// }

// void getFlow2 (void)
// {
//     ++ pulse2_cnt;    //  low가 인식, high가 미인식
// }
// /* --------------------------------------------------------------------------------------------------------------------------- */

// /* Read ADC Voltage ---------------------------------------------------------------------------------------------------------- */
// void check_battery_voltage (void)
// {
//     word adcBT = analogRead(ADC_BAT);    // 배터리 전압 ADC로 읽어오기

// #if DEBUG_MODE
//     Serial.print("adcBT = ");
//     Serial.println(adcBT);
// #endif

//     // 전압 분배로 인하여 ADC로 읽어온 값이 0.75188배 낮아짐
//     // 실제 전압은 읽어온 ADC 값에 낮아진 비율만큼 나눠서 계산해야 함
//     // 읽어온 ADC 값을 전압으로 변환
//     double BTvoltage = (double)adcBT * 3.3 / 0.75188 / 1023;
//     word BTdata = (word)(BTvoltage*100); // 전압으로 변경된 ADC 값에 소수점 두자리까지 정수로 사용하기 위해 * 100

//     batteryLevel = BTdata;  // valve 동작 제어하기 위해 batteryLevel 저장

//     // HM4100B 기준 0~4bar
// #if DEBUG_MODE
//     Serial.print("btLV = ");
//     Serial.println(batteryLevel);
// #endif
// }


// void check_pressure_voltage (void)
// {
//     word adcWP = analogRead(ADC_PRESSURE);    // 배터리 전압 ADC로 읽어오기

//     // 전압 분배로 인하여 ADC로 읽어온 값이 0.662252배 낮아짐
//     // 실제 전압은 읽어온 ADC 값에 낮아진 비율만큼 나눠서 계산해야 함
//     // 읽어온 ADC 값을 전압으로 변환
//     double WPvoltage = (double)adcWP * 3.3 / 0.662252 / 1023;
//     word WPdata = (word)(WPvoltage*100); // 전압으로 변경된 ADC 값에 소수점 두자리까지 정수로 사용하기 위해 * 100

//     // 압력계 정해지면 압력계 값으로 변경하기
//     pressureLevel = WPdata;
// #if DEBUG_MODE
//     Serial.print("WPLV = ");
//     Serial.println(pressureLevel);
// #endif
// }
// /* --------------------------------------------------------------------------------------------------------------------------- */

// void LoRa_setup (void)
// {
//     // US915 settting
//     LoRaWAN.begin(US915);
//     LoRaWAN.setSubBand(2);

//     LoRaWAN.setAntennaGain(2.0);
//     LoRaWAN.setTxPower(20);
//     // LoRaWAN.setADR(false);
//     // LoRaWAN.setDataRate(3);

//     LoRaWAN.joinOTAA(appEui, appKey, devEui);
// }

// /* LoRaWAN Transmit ---------------------------------------------------------------------------------------------------------- */
// uint8_t Check_Alive (uint32_t busyTime)
// {
//     while (LoRaWAN.busy())
//     {
//         uint32_t now_busy_time = millis();

//         if(now_busy_time - busyTime >= 10000)    // 10sec
//         {
//         #if DEBUG_MODE
//             Serial.println("10sec, ");
//         #endif
//             return 1;                            // 일정 시간 동안 busy 안풀리면 +1
//         }
            
//     }
//     if (LoRaWAN.confirmed())
//     {
//     #if DEBUG_MODE
//         Serial.println("ACK");
//     #endif
//         return 0;                               // confirmed 되면  +0
//     }
//     else
//     {
//     #if DEBUG_MODE
//         Serial.println("NACK");
//     #endif
//         return 1;                               // confirmed 안되면 +1
//     }   
// }

// void transmitData (void)
// {
//     uint8_t alive_data = 0;
//     uint32_t check_busy_time = 0;

// #if DEBUG_MODE
//     Serial.print("busy: ");
//     Serial.print(!LoRaWAN.busy());
//     Serial.print(", ");
// #endif
//     if(!LoRaWAN.busy())
//     {
//     #if DEBUG_MODE
//         Serial.print("joined: ");
//         Serial.print(LoRaWAN.joined());
//         Serial.println(", ");
//     #endif
//         if(LoRaWAN.joined())
//         {
//         // #if DEBUG_MODE
//         //     Serial.print("TRANSMIT( ");
//         //     Serial.print("TimeOnAir: ");
//         //     Serial.print(LoRaWAN.getTimeOnAir());
//         //     Serial.print(", NextTxTime: ");
//         //     Serial.print(LoRaWAN.getNextTxTime());
//         //     Serial.print(", MaxPayloadSize: ");
//         //     Serial.print(LoRaWAN.getMaxPayloadSize());
//         //     Serial.print(", DR: ");
//         //     Serial.print(LoRaWAN.getDataRate());
//         //     Serial.print(", TxPower: ");
//         //     Serial.print(LoRaWAN.getTxPower(), 1);
//         //     Serial.print("dbm, UpLinkCounter: ");
//         //     Serial.print(LoRaWAN.getUpLinkCounter());
//         //     Serial.print(", DownLinkCounter: ");
//         //     Serial.print(LoRaWAN.getDownLinkCounter());
//         //     Serial.println(" )");
//         // #endif
//             LoRaWAN.beginPacket();
//             LoRaWAN.write(packetArr, 11);
//             LoRaWAN.endPacket(1);

//         #if DEBUG_MODE
//             Serial.write(packetArr, 11);
//             Serial.println(", ");
//         #endif
// // -----------------------------------------------------------------------------------------------------------------------
//             check_busy_time = millis();
//             alive_data = Check_Alive(check_busy_time);
//             if(alive_data)
//                 ++check_com_count;              // 연결이 끊기면 count 증가
//             else
//             {
//                 check_com_count = 0;            // 연결이 끊기지 않으면 count 초기화
//                 valveOFF = 0;                   // valve off 초기화
//             }
// // -----------------------------------------------------------------------------------------------------------------------
//         }
//         else
//             ++check_com_count;                              // gateway와 연결이 안되면 count 증가
//     }
//     else
//         ++check_com_count;                                 // gateway와 연결이 안되면 count 증가

// #if DEBUG_MODE
//     Serial.print("ad=");
//     Serial.print(alive_data);
//     Serial.print(", ");
//     Serial.print("ccc=");
//     Serial.print(check_com_count);
//     Serial.println(". ");
// #endif
// }

// void transmitVersion (void)
// {
//     if(firmware_version)
//     {
//         if (LoRaWAN.joined() && !LoRaWAN.busy())
//         {
//             LoRaWAN.beginPacket();
//             LoRaWAN.write(fwVersion, 11);
//             LoRaWAN.endPacket();
//             firmware_version = 0;
//         }
//     }
// }
// /* --------------------------------------------------------------------------------------------------------------------------- */

// /* LoRaWAN Receive ----------------------------------------------------------------------------------------------------------- */
// void valve_data (uint8_t RXdata, byte *valveCMD, byte *valveST, byte *valveSTP)
// {
//     if((RXdata - 0x30) == 1)           // 켜라는 명령이 오면
//         *valveCMD = 1;
//     else                               // 그 외 명령이 오면
//         *valveCMD = 0;

//     if(*valveST != *valveCMD)      // 밸브가 이전 상태와 다르게 동작하라는 명령이 오면
//     {
//         if(*valveSTP == VALVE_MAX_STEP)     // 현재 밸브가 동작 중이 아니면
//             *valveSTP = 1;                  // 밸브 동작 시작
//     }
//     else                                // 밸브가 이전 상태와 같은 동작하라는 명령이 오면
//         *valveSTP = VALVE_MAX_STEP-1;   // 수신 완료 상태로 변경
// }

// void receiveData (void)
// {
//     if (LoRaWAN.parsePacket())
//     {
//         uint32_t size;
//         uint8_t data[250];

//         size = LoRaWAN.read(&data[0], sizeof(data));

//         if(size)
//         {
//         #if DEBUG_MODE
//             Serial.print("receiveData = [ ");
//             Serial.write(data[0]);
//             Serial.write(data[1]);
//             Serial.write(data[2]);
//             Serial.write(data[3]);
//             Serial.write(data[4]);
//             Serial.write(data[5]);
//             Serial.write(data[6]);
//             Serial.println(" ]");
//         #endif

//             // data[0] = MSGtype;
//             // data[1] = MSGsize;
//             // data[2] = ID;
//             // data[3] = CMD;

//             if(data[0] == 1)            // UpdateDeviceRequest
//             {

//             }
//             else if(data[0] == 4)       // ReportDeviceResponse
//             {

//             }
//             else if(data[0] == 5)       // PingResponse
//             {

//             }

//             if(data[0] == 'V')
//             {
//                 //  check list 1
//                 if( (data[1] == 'A') && (data[2] == 'L') && (data[3] == 'V') && (data[4] == 'E') )
//                 {
//                 #if DEBUG_MODE
//                     Serial.print("valve1command = ");
//                     Serial.print(valve1command);
//                     Serial.print(" , valve1step = ");
//                     Serial.print(valve1step);
//                     Serial.print(" , valve2command = ");
//                     Serial.print(valve2command);
//                     Serial.print(" , valve2step = ");
//                     Serial.println(valve2step);
//                 #endif

//                     if(data[5] == '1')
//                         valve_data(data[6], &valve1command, &valve1state, &valve1step);
//                     else if(data[5] == '2')
//                         valve_data(data[6], &valve2command, &valve2state, &valve2step);

//                 #if DEBUG_MODE
//                     Serial.print("valve1command = ");
//                     Serial.print(valve1command);
//                     Serial.print(" , valve1step = ");
//                     Serial.print(valve1step);
//                     Serial.print(" , valve2command = ");
//                     Serial.print(valve2command);
//                     Serial.print(" , valve2step = ");
//                     Serial.println(valve2step);
//                 #endif
//                 }
//             }
//             else if(data[0] == 'R')
//             {
//                 if( (data[1] == 'E') && (data[2] == 'S') && (data[3] == 'E') && (data[4] == 'T') )
//                     STM32L0.reset();                    // 52 45 53 45 54, UkVTRVQ=
//             }
//             else if(data[0] == 'F')
//             {
//                 if( (data[1] == 'W') && (data[2] == 'V') && (data[3] == 'E') && (data[4] == 'R') )
//                     firmware_version = 1;               // 46 57 56 45 52, RldWRVI=
//             }
//         }
//     }
// }
// /* --------------------------------------------------------------------------------------------------------------------------- */

// void LoRa_sequence (unsigned long readTime)
// {
//     word nowBT = 0;
//     switch(LoRaStep)
//     {
//         case 1:     // LoRa 전송 데이터 저장
//             packetArr[0] = (byte)((batteryLevel & 0xff00) >> 8);
//             packetArr[1] = (byte)(batteryLevel & 0xff);


//             packetArr[2] = valve1state;

//             packetArr[3] = (byte)((pulse1_cnt & 0xff00) >> 8);
//             packetArr[4] = (byte)(pulse1_cnt & 0xff);

//         #if DEBUG_MODE
//             Serial.print("p1CNT = ");
//             Serial.println(pulse1_cnt);
//         #endif

//             packetArr[5] = valve2state;

//             packetArr[6] = (byte)((pulse2_cnt & 0xff00) >> 8);
//             packetArr[7] = (byte)(pulse2_cnt & 0xff);

//         #if DEBUG_MODE
//             Serial.print("p2CNT = ");
//             Serial.println(pulse2_cnt);
//         #endif

//             packetArr[8] = (byte)((pressureLevel & 0xff00) >> 8);
//             packetArr[9] = (byte)(pressureLevel & 0xff);

//             // packetArr[10] += LoRaCount;
//             ++ LoRaStep;
//         #if DEBUG_MODE
//             Serial.println("L1");
//         #endif
//             break;

//         case 2:     // 데이터 전송 후 시간 저장
//             transmitData();
//             pulse1_cnt = 0;          // 유량1 데이터 초기화
//             pulse2_cnt = 0;          // 유량2 데이터 초기화
//             LoRaTime = millis();
//             ++ LoRaStep;
//         #if DEBUG_MODE
//             Serial.println("L2");
//         #endif
//             break;

//         case 3:             // 정해진 시간동안 수신 모드
//             receiveData();

//             if( (readTime - LoRaTime >= RECEIVE_TIME) ||    // 수신 대기 시간이 지나거나
//                 (valve1step == 1) || (valve2step == 1) ||   // 현재 밸브 상태와 다른 동작 명령을 수신하거나
//                 (valve1step == VALVE_MAX_STEP-1) || (valve2step == VALVE_MAX_STEP-1) )  // 현재 밸브 상태와 같은 동작 명령을 수신하면
//             {
//                 nowBT = (word)(packetArr[0] << 8);
//                 nowBT |= (word)packetArr[1];

//                 //  check list 5
//                 if(nowBT < 150)         // 배터리 전압이 1.50VDC 미만이면 밸브 닫고 열리지 않음
//                 {
//                     Start_Valve1_OFF();
//                     Start_Valve2_OFF();
//                 }

//                 ++ LoRaStep;
//         #if DEBUG_MODE
//             Serial.println("L3");
//         #endif
//             }
//             break;

//         case 4:
//             if( (valve1step == VALVE_MAX_STEP) && (valve2step == VALVE_MAX_STEP) )      // 밸브가 동작중이 아니면
//             {
//                 STM32L0.stop(5000);
//                 LoRaStep = 0;
//                 mainStep = 1;
//         #if DEBUG_MODE
//             Serial.println("L4");
//         #endif;
//             }
//             break;
        
//         default:
//             break;
//     }

// }

// void main_sequence (unsigned long readTime)
// {
//     switch(mainStep)
//     {
//         // case 1:
//         //     digitalWrite(FLOW1_PWR_SW, HIGH); // Flow Sensor1 power ON
//         //     attachInterrupt(digitalPinToInterrupt(FLOW1_IN), getFlow1, FALLING);    // Flow Sensor1 interrupt ON
//         //     mainTime = millis();    // 현재 시간 저장
//         //     ++ mainStep;            // 다음 단계로 이동

//         // #if DEBUG_MODE
//         //     Serial.println("m1");
//         // #endif
//         //     break;

//         // case 2:
//         //     if(readTime - mainTime >= FLOW_TIME)    // 설정 시간만큼 유량값 측정
//         //         ++ mainStep;
//         //     break;

//         // case 3:
//         //     detachInterrupt(FLOW1_IN);        // 유량 1 인터럽트 정지
//         //     digitalWrite(FLOW1_PWR_SW, LOW); // Flow1 power OFF
//         //     ++ mainStep;
//         // #if DEBUG_MODE
//         //     Serial.println("m3");
//         // #endif
//         //     break;

//         // case 4:
//         //     digitalWrite(FLOW2_PWR_SW, HIGH); // Flow Sensor2 power ON
//         //     attachInterrupt(digitalPinToInterrupt(FLOW2_IN), getFlow2, FALLING);    // Flow Sensor2 interrupt ON
//         //     mainTime = millis();    // 현재 시간 저장
//         //     ++ mainStep;            // 다음 단계로 이동

//         // #if DEBUG_MODE
//         //     Serial.println("m4");
//         // #endif
//         //     break;

//         // case 5:
//         //     if(readTime - mainTime >= FLOW_TIME)    // 설정 시간만큼 유량값 측정
//         //         ++ mainStep;
//         //     break;

//         // case 6:
//         //     detachInterrupt(FLOW2_IN);        // 유량 2 인터럽트 정지
//         //     digitalWrite(FLOW2_PWR_SW, LOW); // Flow2 power OFF
//         //     ++ mainStep;
//         // #if DEBUG_MODE
//         //     Serial.println("m6");
//         // #endif
//         //     break;

//         // case 7:
//         //     check_battery_voltage();        // 배터리 전압 읽어서 저장
//         //     check_pressure_voltage();       // 압력계 전압 읽어서 저장
//         //     LoRaStep = 1;                   // 로라 송,수신 시작
//         //     ++ mainStep;
//         // #if DEBUG_MODE
//         //     Serial.println("m7");
//         // #endif
//         //     break;

//         case 1:
//             digitalWrite(FLOW1_PWR_SW, HIGH); // Flow Sensor1 power ON
//             digitalWrite(FLOW2_PWR_SW, HIGH); // Flow Sensor2 power ON
//             attachInterrupt(digitalPinToInterrupt(FLOW1_IN), getFlow1, FALLING);    // Flow Sensor1 interrupt ON
//             attachInterrupt(digitalPinToInterrupt(FLOW2_IN), getFlow2, FALLING);    // Flow Sensor2 interrupt ON
//             mainTime = millis();    // 현재 시간 저장
//             ++ mainStep;            // 다음 단계로 이동

//         #if DEBUG_MODE
//             Serial.println("m1");
//         #endif
//             break;

//         case 2:
//             if(readTime - mainTime >= FLOW_TIME)    // 설정 시간만큼 유량값 측정
//                 ++ mainStep;
//             break;

//         case 3:
//             detachInterrupt(FLOW1_IN);        // 유량 1 인터럽트 정지
//             detachInterrupt(FLOW2_IN);        // 유량 2 인터럽트 정지
//             digitalWrite(FLOW1_PWR_SW, LOW); // Flow1 power OFF
//             digitalWrite(FLOW2_PWR_SW, LOW); // Flow1 power OFF
//             ++ mainStep;
//         #if DEBUG_MODE
//             Serial.println("m3");
//         #endif
//             break;

//         case 4:
//             check_battery_voltage();        // 배터리 전압 읽어서 저장
//             check_pressure_voltage();       // 압력계 전압 읽어서 저장
//             LoRaStep = 1;                   // 로라 송,수신 시작
//             ++ mainStep;
//         #if DEBUG_MODE
//             Serial.println("m4");
//         #endif
//             break;

//         default:
//             break;
//     }
// }

// void reset_system (void)            // 통신이 끊기는거 확인 후에 reset 시켜주는 함수
// {
//     //  check list 4
//     if(check_com_count > 30)    // 1000ms + 10000ms + 3500ms + 5000ms 가 30번 반복될때까지 gateway 응답이 없으면
//         STM32L0.reset();        // 초기화해서 다시 연결 시도
// }



// #define countof(a) (sizeof(a) / sizeof(a[0]))
// void printDateTime(const RtcDateTime& dt)
// {
//     char datestring[20];

//     snprintf_P(datestring, 
//             countof(datestring),
//             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
//             dt.Month(),
//             dt.Day(),
//             dt.Year(),
//             dt.Hour(),
//             dt.Minute(),
//             dt.Second() );
//     Serial.print(datestring);
// }

// void test_from_serial (void)
// {
//     char serialData;
//     RtcDateTime now, manual;
//     if(Serial.available())
//         serialData = Serial.read();

//     switch(serialData)
//     {
//         case '0':
//             Serial.println(serialData);

//             serialData = 0;
//             break;

//         case '1':
//             Serial.println(serialData);
//             valve1step = 1;
//             valve1command = 1;
//             serialData = 0;
//             break;

//         case '2':
//             Serial.println(serialData);
//             valve2step = 1;
//             valve2command = 1;
//             serialData = 0;
//             break;

//         case '3':
//             Serial.println(serialData);
//             valve1step = 1;
//             valve1command = 0;
//             serialData = 0;
//             break;

//         case '4':
//             Serial.println(serialData);
//             valve2step = 1;
//             valve2command = 0;
//             serialData = 0;
//             break;

//         case 'a':
//             Serial.println(serialData);

//             now = Rtc.GetDateTime();
//             printDateTime(now);
//             Serial.println();

//             serialData = 0;
//             break;

//         case 'b':
//             Serial.println(serialData);

//             // sample input: date = "Dec 06 2009", time = "12:34:56"
//             // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
//             manual = RtcDateTime("Jun 01 2020", "10:00:00");
//             Rtc.SetDateTime(manual);

//             serialData = 0;
//             break;
        
//         default:
//             break;
//     }
// }



// void setup()
// {
// #if DEBUG_MODE
//     Serial.begin(115200);
//     while (!Serial) { }
//     Serial.println("POWER(ON)");
// #endif
//     // pins setup
//     pins_setup();

//     // LoRa setup
//     LoRa_setup();

//     // RTC setup
//     RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
// #if DEBUG_MODE
//     printDateTime(compiled);
//     Serial.println();
// #endif

//     if (!Rtc.IsDateTimeValid()) 
//     {
//         // Common Causes:
//         //    1) first time you ran and the device wasn't running yet
//         //    2) the battery on the device is low or even missing

//     #if DEBUG_MODE
//         Serial.println("RTC lost confidence in the DateTime!");
//     #endif
//         Rtc.SetDateTime(compiled);
//     }

//     if (Rtc.GetIsWriteProtected())
//     {
//     #if DEBUG_MODE
//         Serial.println("RTC was write protected, enabling writing now");
//     #endif
//         Rtc.SetIsWriteProtected(false);
//     }

//     if (!Rtc.GetIsRunning())
//     {
//     #if DEBUG_MODE
//         Serial.println("RTC was not actively running, starting now");
//     #endif
//         Rtc.SetIsRunning(true);
//     }

//     RtcDateTime now = Rtc.GetDateTime();
//     if (now < compiled) 
//     {
//     #if DEBUG_MODE
//         Serial.println("RTC is older than compile time!  (Updating DateTime)");
//     #endif
//         Rtc.SetDateTime(compiled);
//     }
//     else if (now > compiled) 
//     {
//     #if DEBUG_MODE
//         Serial.println("RTC is newer than compile time. (this is expected)");
//     #endif
//     }
//     else if (now == compiled) 
//     {
//     #if DEBUG_MODE
//         Serial.println("RTC is the same as compile time! (not expected but all is fine)");
//     #endif
//     }

//     pinMode(A2, OUTPUT);


// // #define RADIO_MOSI                           STM32L0_GPIO_PIN_PA7_SPI1_MOSI
// // #define RADIO_MISO                           STM32L0_GPIO_PIN_PA6_SPI1_MISO
// // #define RADIO_SCLK                           STM32L0_GPIO_PIN_PB3_SPI1_SCK
// // #define RADIO_NSS                            STM32L0_GPIO_PIN_PA15_SPI1_NSS


// }

// void loop()
// {
//     uint32_t now_time = millis();
//     // main loop
//     main_sequence(now_time);

//     // LoRa loop
//     LoRa_sequence(now_time);

//     Valve_Control(now_time, &valve1step, &valve1command, &checkTime1, 1, &valve1state);
//     Valve_Control(now_time, &valve2step, &valve2command, &checkTime2, 2, &valve2state);

//     transmitVersion();
//     reset_system();
//     test_from_serial();
// }
