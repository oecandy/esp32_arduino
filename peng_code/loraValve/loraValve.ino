
//
//
//  2020-05-27
//  - valve 검증 중에 멈추는 경우가 발생하여 시리얼 연결했는데 시리얼 연결되지 않음
//  - 정상 = 포트(COM&LPT) > Grasshopper Virture Serial Port(COMxx)로 연결
//    비정상 = 범용 직렬 버스 컨트롤러 > 알 수 없는 USB 장치(장치 설명자 요청 실패)
//  - Grasshopper driver와 windows 사이에 무슨 문제가 발생하는거 같음.
//  - 시리얼 디버깅 가능하도록 RX1, TX1 추가
//
//
//  2020-05-25
//  - software reset이 정상적으로 되지 않는 경우가 발생하여 시리얼 디버깅 코드 추가
//
//
//  2020-05-21
//  - flow meter에 전류 제한 저항 100kΩ 추가하면서 INPUT_PULLUP에서 INPUT으로 변경
//  - INPUT_PULLUP으로 사용하면 전압이 떨어지지 않아서 유량 측정되지 않음.
//  - idle 시리얼 추가하면서 회사별로 시리얼 정리
//
//
//  2020-04-07
//  - 한국 인삼공사 납품 시리얼 추가
//
//
//  2020-04-03
//  - 밸브 동작 관련하여 수정, 코드 정리
//  - 펌웨어 변경시에 동작 확인 체크 리스트 작성 (check list)
//
//
//  2020-03-27
//  - valve test 중에 통신 끊기고 reset도 되지 않는 증상 발생
//  - loop 확인 하는 시리얼 추가해서 다시 검증 시작
//  1. 통신이 끊기고 valve OFF 하는 동작에 여러번 진입
//  - count가 10이어도 반복적으로 진입하여 문제 발생할 수 있음
//  - flag 추가하여 1번만 진입하고, 통신 다시 연결되면 초기화되도록 수정
//
//
//  2020-03-17
//  - gateway test 목적으로 S/N 추가
//
//
//  2020-03-13
//  - valve를 동작하는 중이 아닌 경우에만 step 1로 변경하도록 수정
//
//
//  2020-03-12 OTAA key 변경
//  - LoRa Server에서 appKey 바뀜
//  - 왜?? 바꾼 사람은 없는데...
//
//
//  2020-03-10  define 추가
//  - 유량 측정 핀 숫자로 설정되어있는 부분 define 선언하여 변경
//  - 18 > FLOW_IN_PIN
//
//
//  2020-02-27 시리얼 추가 및 정리
//  - LoRaServer로 태국 tabaco 출고 전 시리얼 정리
//
//
// 2020-02-25 2밸브 만들기 전 확인
//  - DRV8871 ON, OFF 동작 상태에서 IN1,IN2 LOW or HIGH로 변경하여도 동작이랑은 상관 없음
//
//
// 2020-02-24
//  - 펌웨어 버전 확인 기능 추가
//
//
// 2020-02-06
//  - 밸브 OFF 기능 디버깅
//  - 디버기용 메시지 출력 디버깅
//  - 5분 동안 연결 안되면 리셋하도록 추가
//  - 2월 12일에 태국 출고되는 시리얼 추가
//
//
// 2020-02-04
//  1. 안테나 게인과 파워 추가
//  - 기본 MAX 상태로 설정
//
//
// 2020-01-29
//  1. VALVE OFF 기능 추가
//  - ACK + busy로 gateway와 alive 확인
//  - 재확인으로 연결 끊겼다고 확인되면 VALVE OFF
//  2. 펌웨어 정리
//  - super capacitor 추가하면서 분류 해놓은 항목들 삭제 및 통합
//
//
// 2020-01-28
//  1. +3.3VDC에 2.0F super capacitor 추가
//  - 전압 흔들림 거의 없이 동작. 약 +3.0VDC까지만 떨어짐 (최소 2.0F 사용해야 함)
//  - 1.5F 추가시에는 2.0F보다 많이 흔들림
//  - LoRa 전송 정상, reset 기능 사용하지 않음, valve 상태 내부 flash memory에 저장하는 부분 삭제
//  2. LRS_ABP, LRS_OTAA, TTN_OTAA 나눔
//  - define 설정하여 구분
//
//
// 2019-12-09
//  - 상용 gateway로 변경하고 gateway와 연결 문제 발생하지 않음 (연결 시간, 연결 끊김)
//
// 2019-12-04
//  - 전압에 따라서 밸브 동작 하기도 하고 안하기도 함
//
//
// 2019-12-03
// 1. gateway와 연결되는 시간
//  - 원인은 전원 공급 불안정
//  - rasberry pi에서 rak module로 연결되는 전원선 교체 후에 1분이내 접속
// 2. 밸브 동작하면 전압 흔들림
//  - IC 변경하여 전원이 흔들리는건 조금 안정화
//  - DRV8871을 거치면 꺼지면서 전압이 peak
//  - PWM으로 DRV8871 동작 시켜보면 어떻게 될까? 검증 필요
// 3. DRV8871을 사용하면 50ms 지연으로 밸브가 동작하지 않음
//  - 이 부분도 수정 필요
//
//
// 2019-11-22
// 1. gateway와 연결되는 시간
//  - 처음 join 후 데이터 전송하면 busy가 풀리는데 10~15분정도 소요
//  - gateway가 문제? TTN에서 LoRa Server로 변경하면 다시 확인
//
// 2. 밸브 동작하면 전압 흔들림
//  1) MCU가 죽지는 않음. loop 동작 LED로 확인.
//  2) 통신 COM port PC>MCU RX 끊김. PC>MCU TX는 동작. USB 다시 연결하면 PC>MCU RX 다시 동작 함. USB 장치 인식 실패도 발생
//  3) LoRa 전송되지 않음, begin&join, rejoin 동작하지 않음
//  - reset으로 동작 가능
//  - but! gateway와 연결되는데 약 10 ~ 15분정도 소요
//  - 즉, 밸브 동작 이후에 10~15분 연결이 끊김
//  - 부품 변경하고 회로 설계 수정 필요!
//  - reset으로 동작시에는 밸브 정보를 저장하고 읽어와서 데이터 처리해야함!
//


/* includes ---------------------------------------------------------------------------------------------------- */
#include <STM32L0.h>
#include "LoRaWAN.h"
#include <EEPROM.h>


/* defines ---------------------------------------------------------------------------------------------------- */
/* check mode */
#define DEBUG_MODE      0
#define CHECK_RESET     0

/* LoRaWAN Packet Legnth */
#define PACKET_LEGNTH   11

/* pins */
#define TPS61291_MODE   10      // D10, PB12, CS            // high = boost mode, low = bypass mode
#define BATTERY_IN      16      // A0, PA4, DAC0
#define DRV8871_IN1     12      // D12, PB14, MISO
#define DRV8871_IN2     11      // D11, PB15, MOSI
#define TPS22916_ON     15      // D15, PB8, SCL            // valve power
#define TPS63070_EN     14      // D14, PB9, SDA            // valve enable signal
#define FLOW_IN_PIN     18      // A2, PA2
#define TPS22860_ON     19      // A3, PA3, RX2             // flowmeter power

/* SET Times */
#define FLOW_TIME       1000    // 1000ms = 1s          // 유량 측정 하는 시간
#define DELAY_TIME      75      // ms                   // valve 동작하는 시간
#define RECEIVE_TIME    3500    // 3500ms = 3.5s        // 로라 데이터를 수신하기 위해 기다리는 시간
#define STOP_TIME       4000    // 7000ms = 7s          // 대기 모드 시간

#define VALVE_MAX_STEP  9


/* variables ---------------------------------------------------------------------------------------------------- */
//  'B' = battery voltage(1.11), 'V' = VALVE ON(1), OFF(0), 'F' = Flowmeter count number (Max 9999)
//                      B     3  .  7     6      V     0      F     9     9     9     9
byte packetArr[11] = { 0x42, 0x31, 0x33, 0x35,  0x56, 0x30,  0x46, 0x31, 0x32, 0x33, 0x34 };
uint32_t pulse_cnt = 0;
//                      F     V   -   2     0     2     0   -   0     5   -   2     1  -  0
byte fwVersion[11] = { 0x46, 0x56,   0x32, 0x30, 0x32, 0x30,  0x30, 0x35,  0x32, 0x31,  0x30 };
byte firmware_version = 0;

byte sequenceStep = 1;
unsigned long cTime1 = 0x00000000, cTime2 = 0x00000000, cTime3 = 0x00000000;

byte valveStep = 9;
byte valveState = 0;
byte valveCommand = 0;

byte check_com_count = 0;
byte valveOFF = 0;

byte check_state_led = 0;
byte first_check = 1;

byte serial_debugging = 0;

// 개발용 OTAA 장치
// #define DEVICE_NUMBER   6
// #if DEVICE_NUMBER == 0      /* Applications/Development_Device_OTAA/Devices/OTAA-Test */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "7D5CE421BC5FD025B458247E6D778090";
//     const char *devEui = "b2a57934dceef69f";
// #elif DEVICE_NUMBER == 1    /* Applications/Development_Device_OTAA/Devices/test-lora-valve-otaa_01 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "5F297F25DE28A5B05B93719A89DFE679";
//     const char *devEui = "009fe879284dcd45";
// #elif DEVICE_NUMBER == 2    /* Applications/Development_Device_OTAA/Devices/test-lora-valve-otaa_02 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "784CB785F1766E08A8165B5DB0911D02";
//     const char *devEui = "008bbf967e1e94c9";
// #elif DEVICE_NUMBER == 3    /* Applications/Development_Device_OTAA/Devices/test-lora-valve-otaa_03 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "2F52315BB17E89E84DB1A932194F61E5";
//     const char *devEui = "06f60b16dabb2f1e";
// #elif DEVICE_NUMBER == 4    /* Applications/Development_Device_OTAA/Devices/test-lora-valve-otaa_04 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "49A3464145FF4972132FCD71CA4EB2AF";
//     const char *devEui = "e620040391b57254";
// #elif DEVICE_NUMBER == 5    /* Applications/Development_Device_OTAA/Devices/test-lora-valve-otaa_05 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "3F5BAC927A8396C1B5EA128CE30BF045";
//     const char *devEui = "0a203487dcdeee83";
// #elif DEVICE_NUMBER == 6    /* Applications/Development_Device_OTAA/Devices/test-lora-valve-otaa_06 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "C60171E93486B2DCE52F29BD8182EAA9";
//     const char *devEui = "ba7987ee45c19fc8";
// #elif DEVICE_NUMBER == 7    /* Applications/Development_Device_OTAA/Devices/test-lora-valve-otaa_07 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "5BA49CB1ECA0505828AA3950BE9B3066";
//     const char *devEui = "4ca020dc758e1c33";
// #elif DEVICE_NUMBER == 8    /* Applications/Development_Device_OTAA/Devices/test-lora-valve-otaa_08 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "42F416D152C32A998CEB8CE51EB663A8";
//     const char *devEui = "aabd750829929801";
// #endif
// mqtt connection = lora.farota.com:1883, Topic = application/1/device/devEui16serial/tx
// vavle ON : {"fPort":1,"data":"VkFMVkUx","confirmed":true}

// OTAA 밸브
/* SK-LORA */
// #define DEVICE_NUMBER   3
// #if DEVICE_NUMBER == 1    /* Applications/valve-otaa/Devices/SK-lusk2-valve-otaa-01 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "A87A5074BF9256F06B4D358A7850F67C";
//     const char *devEui = "21b4ed24bf1394c3";
// #elif DEVICE_NUMBER == 2    /* Applications/valve-otaa/Devices/SK-lusk2-valve-otaa-02 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "2A18578445488888B6803EA5A265D023";
//     const char *devEui = "4b00ffeab9992068";
// #elif DEVICE_NUMBER == 3    /* Applications/valve-otaa/Devices/SK-lusk2-valve-otaa-03 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "8687C1FA7DB2FEA0919E12F358C2D16D";
//     const char *devEui = "5ec60d5d4e90ef56";
// #elif DEVICE_NUMBER == 4    /* Applications/valve-otaa/Devices/SK-lusk2-valve-otaa-04 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "CE3C9E9BC630885909FD2859BF21C4D0";
//     const char *devEui = "4a0116cd4f9b3426";
// #elif DEVICE_NUMBER == 5    /* Applications/valve-otaa/Devices/SK-lusk2-valve-otaa-05 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "489750B83C046D03BD3605A884DFDAE9";
//     const char *devEui = "9c0f85afecf2dd19";
// #endif

/* ginseng-ginsengcorperation */
// #define DEVICE_NUMBER   1
// #if DEVICE_NUMBER == 1    /* Applications/valve-otaa/Devices/ginseng-ginsengcorperation-pesticide-valve-otaa-01 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "17ED28CD3449661162EC64E031F56FE6";
//     const char *devEui = "2dcbfa8a7d1dfac7";
// #elif DEVICE_NUMBER == 2    /* Applications/valve-otaa/Devices/ginseng-ginsengcorperation-pesticide-valve-otaa-02 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "C4BE1CC85884F486357C38A13350AA6E";
//     const char *devEui = "069383228e4ed7bd";
// #elif DEVICE_NUMBER == 3    /* Applications/valve-otaa/Devices/ginseng-ginsengcorperation-pesticide-valve-otaa-03 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "B5E6196888908A70F89EBFB9B8F28C92";
//     const char *devEui = "74be59f903c55850";
// #elif DEVICE_NUMBER == 11    /* Applications/valve-otaa/Devices/ginseng-ginsengcorperation-water-valve-otaa-01 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "214F87DA2CE458A1EC85F6777515A176";
//     const char *devEui = "910470b2f636f681";
// #elif DEVICE_NUMBER == 12    /* Applications/valve-otaa/Devices/ginseng-ginsengcorperation-water-valve-otaa-02 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "F59127A79F2FB7C8DFDD5D11D795FD5A";
//     const char *devEui = "e18a7fa2a0dd9544";
// #elif DEVICE_NUMBER == 13    /* Applications/valve-otaa/Devices/ginseng-ginsengcorperation-water-valve-otaa-03 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "67030D8DF726A52A19BE0CBE0C2CF126";
//     const char *devEui = "ad7143eaf8fcf44f";
// #elif DEVICE_NUMBER == 14    /* Applications/valve-otaa/Devices/ginseng-ginsengcorperation-water-valve-otaa-04 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "D955FDBA8B129A891F053DD364F1D55B";
//     const char *devEui = "721d393941206868";
// #elif DEVICE_NUMBER == 15    /* Applications/valve-otaa/Devices/ginseng-ginsengcorperation-water-valve-otaa-05 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "0F115A73123A417AC50128290893174B";
//     const char *devEui = "a988de2d0a72229e";
// #elif DEVICE_NUMBER == 16    /* Applications/valve-otaa/Devices/ginseng-ginsengcorperation-water-valve-otaa-06 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "98EC4FE50C859E2C08DE2A13B7E1F7C2";
//     const char *devEui = "450334cb5681aa88";
// #elif DEVICE_NUMBER == 17    /* Applications/valve-otaa/Devices/ginseng-ginsengcorperation-water-valve-otaa-07 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "F9A12DAE530F83D7BB0926445F70E96E";
//     const char *devEui = "fc333262fb78eef8";
// #elif DEVICE_NUMBER == 18    /* Applications/valve-otaa/Devices/ginseng-ginsengcorperation-water-valve-otaa-08 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "863BC17090EAD18635A324BC0BB6E25C";
//     const char *devEui = "be107069afcb850c";
// #endif

/* hydrofresh */
// #define DEVICE_NUMBER   1
// #if DEVICE_NUMBER == 1    /* Applications/valve-otaa/Devices/hydrofresh-testbed-valve-main-otaa-01 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "D49E6FC40E1493080ADE6BFD201E6D83";
//     const char *devEui = "004075b5fcb6d1fe";
// #elif DEVICE_NUMBER == 2    /* Applications/valve-otaa/Devices/tobacco-testbed-valve-fert-otaa-02 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "5425A512CCB95D3E6A4C06B9EDAFD11C";
//     const char *devEui = "00ab90639e6dee3b";
// #endif

/* tobacco */
// #define DEVICE_NUMBER   1
// #if DEVICE_NUMBER == 1    /* Applications/valve-otaa/Devices/tobacco-testbed-valve-main-otaa-01 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "6C6B604CBE86C7B125453F6CB0FE2D29";
//     const char *devEui = "ec25278148203aea";
// #elif DEVICE_NUMBER == 2    /* Applications/valve-otaa/Devices/tobacco-testbed-valve-fert-otaa-02 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "CA5A4FF62A43AF38E663D49F09A045F1";
//     const char *devEui = "00db6921ff79ac2e";
// #endif

/* idle */
// #define DEVICE_NUMBER   4
// #if DEVICE_NUMBER == 1    /* Applications/valve-otaa/Devices/idle-valve-with-flowmeter-otaa-01 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "DC4CC16D0628BDACCE506DDD1439F45E";
//     const char *devEui = "985604415a517426";
// #elif DEVICE_NUMBER == 2    /* Applications/valve-otaa/Devices/idle-valve-with-flowmeter-otaa-02 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "7C58B94D200B2E0E32AF1536DE29F8D5";
//     const char *devEui = "4360d7b382f42d1b";
// #elif DEVICE_NUMBER == 3    /* Applications/valve-otaa/Devices/idle-valve-with-flowmeter-otaa-03 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "F0B8B8EB98BD78A551749A41829BC4B6";
//     const char *devEui = "56bf605fae6f37f1";
// #elif DEVICE_NUMBER == 4    /* Applications/valve-otaa/Devices/idle-valve-with-flowmeter-otaa-04 */
//     const char *appEui = "0000000000000000";
//     const char *appKey = "BE0B4034461099AF1BAFEBA19495414A";
//     const char *devEui = "fdf77139185bc3f0";
// #endif


///* 한국농어촌공사 */
//#define DEVICE_NUMBER   1
//#if DEVICE_NUMBER == 1    /* Applications/valve-otaa/Devices/korea-rural-corporation-valve-otaa-01 */
//    const char *appEui = "0000000000000000";
//    const char *appKey = "0F0B6D94D5D75F884FD71AB326C68A28";
//    const char *devEui = "266cd6523090af32";
//#elif DEVICE_NUMBER == 2    /* Applications/valve-otaa/Devices/korea-rural-corporation-valve-otaa-02 */
//    const char *appEui = "0000000000000000";
//    const char *appKey = "4526677C0236E59B9CC243DF3DCFA84F";
//    const char *devEui = "48a750238e7e4aa0";
//#endif

/* luonecity 2 */
#define DEVICE_NUMBER   1
#if DEVICE_NUMBER == 1    /* Applications/valve-otaa/Devices/korea-rural-corporation-valve-otaa-01 */
    const char *appEui = "0000000000000000";
    const char *appKey = "9C6A90A47F943EBA7B06383A798A0ED8";
    const char *devEui = "d3d5df0b405cae5f";
#elif DEVICE_NUMBER == 2    /* Applications/valve-otaa/Devices/korea-rural-corporation-valve-otaa-02 */
    const char *appEui = "0000000000000000";
    const char *appKey = "D97F0C481A8D48DDC4E97DB1F8F3C6D6";
    const char *devEui = "7aebe60b9cd5c16d";
#endif


// UpstreamTopic : application/3/device/devEui16serial/tx
// DownstreamTopic : application/3/device/devEui16serial/rx
// vavle ON : {"fPort":1,"data":"VkFMVkUx","confirmed":true}
// valve OFF : {"fPort":1,"data":"VkFMVkUw","confirmed":true}


/* functions ---------------------------------------------------------------------------------------------------- */
/* pins */
void set_pin (uint32_t ulPin)
{
    // low current setup
    pinMode(ulPin, INPUT_PULLUP);
}

void not_used_pins_setup (void)   // 사용하지 않는 핀을 풀업 모드로 설정
{
    set_pin(0);     // D0, PA10, RX1
    set_pin(1);     // D1, PA9, TX1
    set_pin(2);     // D2, PB5
    set_pin(3);     // D3, PB6
    set_pin(4);     // D4, PB7
    set_pin(5);     // D5, PB2
    set_pin(8);     // D8, PA13, RX3
    set_pin(9);     // D9, PA14, TX3

    pinMode(13, OUTPUT);    // D13, PB13, SCK
    digitalWrite(13, LOW);  // LED OFF (MCU - |>| - GND)

    set_pin(17);    // A1, PA5, DAC1
    set_pin(20);    // A4, PA0
}

void pins_setup (void)
{
    // battery enable pin Configuration
    pinMode(TPS61291_MODE, OUTPUT);
    digitalWrite(TPS61291_MODE, HIGH);      // Boost Mode

    // drv8871 control pin Configuration (sleep mode = IN1 and IN2 low)
    pinMode(DRV8871_IN1, OUTPUT);       // IN1 0, IN2 0 = sleep entered after 1 ms
    digitalWrite(DRV8871_IN1, LOW);     // IN1 0, IN2 1 = Reverse (Current OUT2 → OUT1)
    pinMode(DRV8871_IN2, OUTPUT);       // IN1 1, IN2 0 = Forward (Current OUT1 → OUT2)
    digitalWrite(DRV8871_IN2, LOW);     // IN1 1, IN2 1 = Brake; low-side slow decay

    // TPS22916 enable pin Configuration
    pinMode(TPS22916_ON, OUTPUT);   // 3.3VDC 전압 Vin > Vout 연결 선택
    digitalWrite(TPS22916_ON, LOW); // ENABLE(ON) Active High

    // TPS63070 enable pin Configuration
    pinMode(TPS63070_EN, OUTPUT);   // 6VDC 전압 Vout 출력 시작
    digitalWrite(TPS63070_EN, LOW); // Pull high to enable the device, pull low to disable the device.

    // Flowmeter input pin Configuration
    pinMode(FLOW_IN_PIN, INPUT);
    // pinMode(FLOW_IN_PIN, INPUT_PULLUP);

    // Flowmeter power pin Configuration
    pinMode(TPS22860_ON, OUTPUT);   // Flowmeter 전압 Vout 출력 시작
    digitalWrite(TPS22860_ON, LOW); // Active high switch control input.
}

/* LoRa */
void LoRa_setup (void)
{
    // EU868 settting
    // LoRaWAN.begin(EU868);
    // LoRaWAN.setAntennaGain(2.0);
    // LoRaWAN.setTxPower(16);

    // // US915 settting
    LoRaWAN.begin(US915);
    LoRaWAN.setSubBand(2);

    LoRaWAN.setAntennaGain(2.0);
    LoRaWAN.setTxPower(20); // 2020-04-23 20 > 10, 2020-05-25 > 20
    // LoRaWAN.setADR(false);
    // LoRaWAN.setDataRate(0);

    LoRaWAN.joinOTAA(appEui, appKey, devEui);
}

/* Flowmeter */
void getFlow (void)
{
    pulse_cnt++;    //  low가 인식, high가 미인식
}

byte check_battery_voltage (byte num)
{
    byte ascBT = 0;
    byte stateBT = 0;
    word adcBT = analogRead(BATTERY_IN);    // 배터리 전압 ADC로 읽어오기

    // 전압 분배로 인하여 ADC로 읽어온 값이 0.75188배 낮아짐
    // 실제 전압은 읽어온 ADC 값에 낮아진 비율만큼 나눠서 계산해야 함
    // 읽어온 ADC 값을 전압으로 변환
    double BTvoltage = (double)adcBT * 3.3 / 0.75188 / 1023;
    word BTdata = (word)(BTvoltage*100); // 전압으로 변경된 ADC 값에 소수점 두자리까지 정수로 사용하기 위해 * 100

    switch(num)     // 아스키 코드로 변경
    {
        case 1:     // 100의 자리 숫자
            ascBT = ( ((BTdata % 1000) / 100) + 0x30 );
            break;
        case 2:     // 10의 자리 숫자
            ascBT = ( (((BTdata % 1000) % 100) / 10) + 0x30 ); 
            break;
        case 3:     // 1의 자리 숫자
            ascBT = ( (((BTdata % 1000) % 100) % 10) + 0x30 );
            break;

        default: break;
    }

    if(BTdata < 150)    stateBT = 0;    // 배터리 전압이 1.5V보다 적으면 0
    else                stateBT = 1;    // 높으면 1

    if(num) return ascBT;
    else    return stateBT;
}


/* DRV8871 Control ----------------------------------------------------------------------------------------------------------- */
void Start_Valve_OFF (void)
{
    valveCommand = 0;

    if(valveState)                  // 밸브가 켜져있으면
        valveStep = 1;              // 밸브 동작
    else                            // 밸브가 꺼져있으면
        valveStep = VALVE_MAX_STEP; // 밸브는 동작하지 않음
}

void Valve_Control_TPS61291 (unsigned long waitTime)
{
    switch(valveStep)
    {
        case 1:
            digitalWrite(TPS22916_ON, HIGH);    // ENABLE(ON) Active High
            digitalWrite(TPS63070_EN, HIGH);    // Pull high to enable the device, pull low to disable the device.
            cTime3 = millis();
            ++ valveStep;

            if(serial_debugging)    { Serial.println("v1"); Serial1.println("v1"); }
            break;

        case 2:
            if(waitTime - cTime3 >= 10)      // Wait Turn On Time
                ++ valveStep;
            break;

        case 3:
            if(valveCommand)                        // Valve ON
            {
                digitalWrite(DRV8871_IN1, LOW);     // Reverse (Current OUT2 → OUT1)
                digitalWrite(DRV8871_IN2, HIGH);
            }
            else                                    // Valve OFF
            {
                digitalWrite(DRV8871_IN1, HIGH);    // Forward (Current OUT1 → OUT2)
                digitalWrite(DRV8871_IN2, LOW);
            }
            cTime3 = millis();
            ++ valveStep;

            if(serial_debugging)    { Serial.println("v3"); Serial1.println("v3"); }
            break;

        case 4:
            if(waitTime - cTime3 >= DELAY_TIME)     // Operating pulse width
                ++ valveStep;
            break;

        case 5:
            digitalWrite(TPS22916_ON, LOW);
            digitalWrite(TPS63070_EN, LOW);
            ++ valveStep;

            if(serial_debugging)    { Serial.println("v5"); Serial1.println("v5"); }
            break;

        case 6:
            digitalWrite(DRV8871_IN1, LOW);     // Coast;
            digitalWrite(DRV8871_IN2, LOW);
            ++ valveStep;

            if(serial_debugging)    { Serial.println("v6"); Serial1.println("v6"); }
            break;

        case 7:
            valveState = valveCommand;
            ++ valveStep;

            if(serial_debugging)    { Serial.println("v7"); Serial1.println("v7"); }
            break;

        case 8:
            ++ valveStep;

            if(serial_debugging)    { Serial.println("v8"); Serial1.println("v8"); }
            break;

        // case 8:     // superCap Test
        //     if(valveState == 1)
        //         valveCommand = 0;
        //     else
        //         valveCommand = 1;

        //     valveStep = 1;
        //     delay(10000);
        //     break;

        default: break;
    }
}
/* --------------------------------------------------------------------------------------------------------------------------- */

/* LoRaWAN Transmit ---------------------------------------------------------------------------------------------------------- */
uint8_t Check_Alive (uint32_t busyTime)
{
    while (LoRaWAN.busy())
    {
        uint32_t now_busy_time = millis();

        if(now_busy_time - busyTime >= 10000)    // 10sec
        {
        #if DEBUG_MODE
            Serial.println("10sec, ");
        #endif
            return 1;                            // 일정 시간 동안 busy 안풀리면 +1
        }
            
    }
    if (LoRaWAN.confirmed())
    {
    #if DEBUG_MODE
        Serial.println("ACK");
    #endif
        return 0;                               // confirmed 되면  +0
    }
    else
    {
    #if DEBUG_MODE
        Serial.println("NACK");
    #endif
        return 1;                               // confirmed 안되면 +1
    }   
}

void transmitData (void)
{
    uint8_t alive_data = 0;
    uint32_t check_busy_time = 0;

#if DEBUG_MODE
    Serial.print("busy: ");
    Serial.print(!LoRaWAN.busy());
    Serial.print(", ");
#endif
    if(!LoRaWAN.busy())
    {
    #if DEBUG_MODE
        Serial.print("joined: ");
        Serial.print(LoRaWAN.joined());
        Serial.println(", ");
    #endif
        if(LoRaWAN.joined())
        {
        // #if DEBUG_MODE
        //     Serial.print("TRANSMIT( ");
        //     Serial.print("TimeOnAir: ");
        //     Serial.print(LoRaWAN.getTimeOnAir());
        //     Serial.print(", NextTxTime: ");
        //     Serial.print(LoRaWAN.getNextTxTime());
        //     Serial.print(", MaxPayloadSize: ");
        //     Serial.print(LoRaWAN.getMaxPayloadSize());
        //     Serial.print(", DR: ");
        //     Serial.print(LoRaWAN.getDataRate());
        //     Serial.print(", TxPower: ");
        //     Serial.print(LoRaWAN.getTxPower(), 1);
        //     Serial.print("dbm, UpLinkCounter: ");
        //     Serial.print(LoRaWAN.getUpLinkCounter());
        //     Serial.print(", DownLinkCounter: ");
        //     Serial.print(LoRaWAN.getDownLinkCounter());
        //     Serial.println(" )");
        // #endif
            LoRaWAN.beginPacket();
            LoRaWAN.write(packetArr, 11);
            LoRaWAN.endPacket(1);

        #if DEBUG_MODE
            Serial.write(packetArr, 11);
            Serial.println(", ");
        #endif
// -----------------------------------------------------------------------------------------------------------------------
            check_busy_time = millis();
            alive_data = Check_Alive(check_busy_time);
            if(alive_data)
                ++check_com_count;              // 연결이 끊기면 count 증가
            else
            {
                check_com_count = 0;            // 연결이 끊기지 않으면 count 초기화
                valveOFF = 0;                   // valve off 초기화
            }
// -----------------------------------------------------------------------------------------------------------------------
        }
        else
            ++check_com_count;                              // gateway와 연결이 안되면 count 증가
    }
    else
        ++check_com_count;                                 // gateway와 연결이 안되면 count 증가

#if DEBUG_MODE
    Serial.print("ad=");
    Serial.print(alive_data);
    Serial.print(", ");
    Serial.print("ccc=");
    Serial.print(check_com_count);
    Serial.println(". ");
#endif
}

void transmitVersion (void)
{
    if(firmware_version)
    {
        if (LoRaWAN.joined() && !LoRaWAN.busy())
        {
            LoRaWAN.beginPacket();
            LoRaWAN.write(fwVersion, 11);
            LoRaWAN.endPacket();
            firmware_version = 0;
        }
    }
}
/* --------------------------------------------------------------------------------------------------------------------------- */

/* LoRaWAN Receive ----------------------------------------------------------------------------------------------------------- */
void receiveData (void)
{
    if (LoRaWAN.parsePacket())
    {
        uint32_t size;
        uint8_t data[20];

        size = LoRaWAN.read(&data[0], sizeof(data));

        if(size)
        {
        #if DEBUG_MODE
            Serial.print("receiveData = [ ");
            Serial.write(data[0]);
            Serial.write(data[1]);
            Serial.write(data[2]);
            Serial.write(data[3]);
            Serial.write(data[4]);
            Serial.write(data[5]);
            Serial.println(" ]");
        #endif

            if(data[0] == 'V')
            {
                if( (data[1] == 'A') && (data[2] == 'L') && (data[3] == 'V') && (data[4] == 'E') )
                {
// check list - ON, OFF 통신으로 동작하는지 확인
                    // // 2020-02-25 2밸브 만들기 전 확인
                    // valveCommand = data[5] - 0x30;
                    if((data[5] - 0x30) == 1)           // 켜라는 명령이 오면   // 56 41 4C 56 45 31, VkFMVkUx
                        valveCommand = 1;
                    else                                // 그 외 명령이 오면    // 56 41 4C 56 45 30, VkFMVkUw
                        valveCommand = 0;

                    if(valveState != valveCommand)      // 밸브가 이전 상태와 다르게 동작하라는 명령이 오면
                    {
                        if(valveStep == VALVE_MAX_STEP)     // 현재 밸브가 동작 중이 아니면
                            valveStep = 1;                  // 밸브 동작 시작
                    }
                    else                                // 밸브가 이전 상태와 같은 동작하라는 명령이 오면
                        valveStep = VALVE_MAX_STEP-1;   // 수신 완료 상태로 변경
                    
                }
            }
            else if(data[0] == 'R')
            {
                if( (data[1] == 'E') && (data[2] == 'S') && (data[3] == 'E') && (data[4] == 'T') )
                    STM32L0.reset();                    // 52 45 53 45 54, UkVTRVQ=
            }
            else if(data[0] == 'F')
            {
                if( (data[1] == 'W') && (data[2] == 'V') && (data[3] == 'E') && (data[4] == 'R') )
                    firmware_version = 1;               // 46 57 56 45 52, RldWRVI=
            }
        }
    }
}
/* --------------------------------------------------------------------------------------------------------------------------- */

void Check_Valve (void)             // 밸브가 켜져 있으면 문제가 되므로 일정 시간 이후에 꺼지도록하는 함수
{
// check list - 10 변경하여 통신 끊김 빨리 재현시켜 밸브가 꺼지는지 확인
// 1. 밸브가 켜져 있고, 통신이 끊기면 꺼지는지 확인
// 2. 통신이 끊기고 다시 접속하면 정상 동작 (ON, OFF) 확인
    if( (check_com_count > 10) && (!valveOFF) && (valveState) && (valveStep == VALVE_MAX_STEP) )    // 30번 전송하는 동안 통신이 끊기면 (10s * 10 = 100s)
    {
        valveOFF = 1;
        Start_Valve_OFF();

        if(serial_debugging)    { Serial.println("c1"); Serial1.println("c1"); }

        while (valveStep < VALVE_MAX_STEP)  // confirm을 10초씩 대기하는 경우가 생기므로
        {                                   // 다른 함수 진입하지 않고 while에서 밸브 꺼지도록 처리
            unsigned long countMillis = millis();
            Valve_Control_TPS61291(countMillis);
        }

        if(serial_debugging)    { Serial.println("c2"); Serial1.println("c2"); }
    }
}

void reset_system (void)            // 통신이 끊기는거 확인 후에 reset 시켜주는 함수
{
#if CHECK_RESET
    if(first_check)     // 처음 한번 켜지면 확인
    {
        first_check = 0;

        check_state_led = EEPROM.read(100);     // 값을 읽어와서
        digitalWrite(13, check_state_led);      // led 동작

        if(serial_debugging)    { Serial.println("r1"); Serial1.println("r1"); }
    }
#endif

// check list - 30 변경하여 빨리 재현시켜서 통신이 끊기면 reset 되는지 확인
    if(check_com_count > 30)    // 1000ms + 10000ms + 3500ms + 5000ms 가 30번 반복될때까지 gateway 응답이 없으면
    {
        if(serial_debugging)    { Serial.println("r2"); Serial1.println("r2"); }

    #if CHECK_RESET
        if(check_state_led) EEPROM.write(100, 0);   // 1이면 0저장
        else                EEPROM.write(100, 1);   // 0이면 1저장
    #endif
        STM32L0.reset();        // 초기화해서 다시 연결 시도

        if(serial_debugging)    { Serial.println("r3"); Serial1.println("r3"); }
    }
}

char stepSerial;
void check_serial (void)
{
    if(Serial.available())
        stepSerial = Serial.read();

    if(Serial1.available())
        stepSerial = Serial1.read();

    switch(stepSerial)
    {
        case '0':
            Serial.println(stepSerial);
            Serial1.println(stepSerial);
            serial_debugging = 0;
            stepSerial = 0;
            break;

        case '1':
            Serial.println(stepSerial);
            Serial1.println(stepSerial);
            serial_debugging = 1;
            stepSerial = 0;
            break;

        // case 't':
        //     transmitData();
        //     stepSerial = 0;
        //     break;

        default: break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define REAL_CODE 1
#if REAL_CODE
byte ModeOperation = 0;
void setup()
{
    not_used_pins_setup();  // 사용하지 않는 포트 설정

    Serial.begin(115200);
    Serial1.begin(115200);
#if DEBUG_MODE
    while (!Serial) { }
    Serial.println("POWER(ON)");
#endif
    pins_setup();           // 사용하는 포트 설정
    LoRa_setup();           // 로라 통신 설정

    // ModeOperation = check_battery_voltage(0);   // 배터리 전압 확인하여 Boost Mode Operation or Bypass Mode Operation
    delay(2000);           // 전원 ON되고 2초 뒤부터 동작
}

// MULTIMETER UT71D로 전류 측정 (밸브 동작 x) 대기 0.6mA ~ 0.7mA↓ > 유량 측정 8.5mA↓
void loop()
{
    unsigned long nowTime = millis();
    switch(sequenceStep)
    {
        case 1:             // 유량계 동작하고 현재 시간 저장
            digitalWrite(TPS22860_ON, HIGH); // Active high switch control input.
            attachInterrupt(digitalPinToInterrupt(FLOW_IN_PIN), getFlow, FALLING);
            cTime1 = millis();
            ++ sequenceStep;

            if(serial_debugging)    { Serial.println("s1"); Serial1.println("s1"); }
            break;

        case 2:             // 정해진 시간이 지나면 다음 단계로 이동
            if(nowTime - cTime1 >= FLOW_TIME)
            {
                ++ sequenceStep;

                if(serial_debugging)    { Serial.println("s2"); Serial1.println("s2"); }
            }
            break;

        case 3:             // 유량계 동작 차단 후 다음 단계로 이동
            detachInterrupt(FLOW_IN_PIN);        // 인터럽트 정지
            digitalWrite(TPS22860_ON, LOW); // 전원 차단
            ++ sequenceStep;

            if(serial_debugging)    { Serial.println("s3"); Serial1.println("s3"); }
            break;

        case 4:             // 데이터 저장
            packetArr[1] = check_battery_voltage(1);
            packetArr[2] = check_battery_voltage(2);
            packetArr[3] = check_battery_voltage(3);

            packetArr[5] = valveState + 0x30;

            packetArr[7] = ( ((pulse_cnt % 10000) / 1000) + 0x30 );
            packetArr[8] = ( (((pulse_cnt % 10000) % 1000) / 100) + 0x30 );
            packetArr[9] = ( ((((pulse_cnt % 10000) % 1000) % 100) / 10) + 0x30 );
            packetArr[10] = ( ((((pulse_cnt % 10000) % 1000) % 100) % 10) + 0x30 );
            ++ sequenceStep;

            if(serial_debugging)    { Serial.println("s4"); Serial1.println("s4"); }
            break;

        case 5:             // 데이터 전송 후 시간 저장
            transmitData();
            pulse_cnt = 0;          // 유량 데이터 초기화
            cTime2 = millis();
            ++ sequenceStep;

            if(serial_debugging)    { Serial.println("s5"); Serial1.println("s5"); }
            break;

        case 6:             // 정해진 시간동안 수신 모드
            receiveData();

            if( (nowTime - cTime2 >= RECEIVE_TIME) || (valveStep == 1) || (valveStep == VALVE_MAX_STEP-1) ) // 수신 시간이 지나거나 수신이 완료 되면
            {
// check list - 전압이 낮은 환경에서 밸브가 동작안하는지 확인
                if(!check_battery_voltage(0))    // 배터리 전압이 일정 기준(1.50V) 밑이면 무조건 밸브 닫고 열리지 않도록 수정
                    Start_Valve_OFF();

                ++ sequenceStep;

                if(serial_debugging)    { Serial.println("s6"); Serial1.println("s6"); }
            }
            break;

        case 7:
            if(valveStep == VALVE_MAX_STEP)     // 밸브가 동작중이 아니면
            {
                STM32L0.stop(5000);
                sequenceStep = 1;

                if(serial_debugging)    { Serial.println("s7"); Serial1.println("s7"); }
            }
            break;

        default: break;
    }

    Valve_Control_TPS61291(nowTime);
    Check_Valve();
    reset_system();
    transmitVersion();

    check_serial();

// // // 검증이 정확히 되지 않음
// 2020-02-25
// DRV8871 IN1, IN2에 따른 4가지 상태에서 전류 확인
// DRV8871 Table 1. H-Bridge Control
// 1. IN1 = 0, IN2 = 0  \  '1' > '2' > '6'  \  16mA > 25, 33, 9, 0mA > 
// 2. IN1 = 0, IN2 = 1  \  '1' > '2' > '3' > '2' > '6'  \  
// 3. IN1 = 1, IN2 = 0  \  '1' > '2' > '4' > '2' > '6'  \  
// 4. IN1 = 1, IN2 = 1  \  '1' > '2' > '5' > '2' > '6'  \  

// '0' = VkFMVkUw, '1' = VkFMVkUx, '2' = VkFMVkUy, '3' = VkFMVkUz, '4' = VkFMVkU0, '5' = VkFMVkU1, '6' = VkFMVkU2

//     unsigned long nowTime = millis();

//     switch(sequenceStep)
//     {
//         case 1:     // data 송신
//             if(!LoRaWAN.busy())
//             {
//                 if(LoRaWAN.joined())
//                 {
//                     LoRaWAN.beginPacket();
//                     LoRaWAN.write(packetArr, 11);
//                     LoRaWAN.endPacket(0);

//                     cTime1 = millis();
//                     ++ sequenceStep;
//                 }
//             }
//             break;

//         case 2:     // data 수신
//             if (LoRaWAN.parsePacket())
//             {
//                 uint32_t size;
//                 uint8_t data[20];

//                 size = LoRaWAN.read(&data[0], sizeof(data));

//                 if(size)
//                 {
//                     if( (data[0] == 'V') && (data[1] == 'A') && (data[2] == 'L') && (data[3] == 'V') && (data[4] == 'E') )
//                         stepSerial = data[5];
//                 }
//             }

//             if( (nowTime - cTime1 >= 5000) || (stepSerial > 0) ) // 수신 시간이 지나거나 수신이 완료 되면
//             {
//                 cTime2 = millis();
//                 ++ sequenceStep;
//             }
//             break;

//         case 3:     // 10초 대기
//             if(nowTime - cTime2 >= 5000)
//                 sequenceStep = 1;
//             break;

//         default: break;
//     }

//     // if(Serial.available())
//     //     stepSerial = Serial.read();

//     switch(stepSerial)
//     {
//         case '1':
//             digitalWrite(TPS22916_ON, HIGH);    // ENABLE(ON) Active High
//             digitalWrite(TPS63070_EN, HIGH);    // Pull high to enable the device, pull low to disable the device.
//             // Serial.println(stepSerial);
//             stepSerial = 0;
//             break;

//         case '2':
//             digitalWrite(DRV8871_IN1, LOW);
//             digitalWrite(DRV8871_IN2, LOW);
//             // Serial.println(stepSerial);
//             stepSerial = 0;
//             break;

//         case '3':
//             digitalWrite(DRV8871_IN1, LOW);     // Reverse (Current OUT2 → OUT1)
//             digitalWrite(DRV8871_IN2, HIGH);
//             // Serial.println(stepSerial);
//             stepSerial = 0;
//             break;

//         case '4':
//             digitalWrite(DRV8871_IN1, HIGH);    // Forward (Current OUT1 → OUT2)
//             digitalWrite(DRV8871_IN2, LOW);
//             // Serial.println(stepSerial);
//             stepSerial = 0;
//             break;

//         case '5':
//             digitalWrite(DRV8871_IN1, HIGH);
//             digitalWrite(DRV8871_IN2, HIGH);
//             // Serial.println(stepSerial);
//             stepSerial = 0;
//             break;

//         case '6':
//             digitalWrite(TPS22916_ON, LOW);    // ENABLE(ON) Active High
//             digitalWrite(TPS63070_EN, LOW);    // Pull high to enable the device, pull low to disable the device.
//             // Serial.println(stepSerial);
//             stepSerial = 0;
//             break;

//         default: break;
//     }
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#if DRV8871_CURRENT_TEST
// DRV8871 사용하지 않고 동작하면 정상적으로 동작하는데
// DRV8871을 사용하면 전압이 항상 튀고, 50ms에서 동작하지 않아서
// 동작 순서 변경하여 검증
    if(Serial.available())
        stepSerial = Serial.read();

    switch (stepSerial)
    {
        case '1':       // VM 전원 ON
            VM_ON = 1;
            Serial.println(stepSerial);
            stepSerial = 0;
            break;

        case '2':       // VM 전원 OFF
            digitalWrite(TPS22916_ON, LOW);    // ENABLE(ON) Active High
            digitalWrite(TPS63070_EN, LOW);    // Pull high to enable the device, pull low to disable the device.
            Serial.println(stepSerial);
            stepSerial = 0;
            break;

        case '3':       // 8871 동작 ON
            DRV8871_ON = 1;
            Serial.println(stepSerial);
            stepSerial = 0;
            break;

        case '4':       // 8871 동작 OFF
            DRV8871_OFF = 1;
            Serial.println(stepSerial);
            stepSerial = 0;
            break;

        case '5':       // 8871 동작
            digitalWrite(DRV8871_IN1, LOW);
            digitalWrite(DRV8871_IN2, LOW);
            Serial.println(stepSerial);
            stepSerial = 0;
            break;            

        default: break;
    }

    switch(VM_ON)
    {
        case 1:
            digitalWrite(TPS22916_ON, HIGH);    // ENABLE(ON) Active High
            digitalWrite(TPS63070_EN, HIGH);    // Pull high to enable the device, pull low to disable the device.
            ++ VM_ON;
            break;

        case 2:
            digitalWrite(DRV8871_IN1, LOW);
            digitalWrite(DRV8871_IN2, LOW);
            ++ VM_ON;
            break;

        default: break;
    }

    switch (DRV8871_ON)
    {
        case 1:
            digitalWrite(TPS22916_ON, HIGH);    // ENABLE(ON) Active High
            digitalWrite(TPS63070_EN, HIGH);    // Pull high to enable the device, pull low to disable the device.
            ++ DRV8871_ON;
            break;

        case 2:
            digitalWrite(DRV8871_IN1, LOW);
            digitalWrite(DRV8871_IN2, LOW);
            ++ DRV8871_ON;
            break;

        case 3:
            digitalWrite(DRV8871_IN1, HIGH);    // Forward (Current OUT1 → OUT2)
            digitalWrite(DRV8871_IN2, LOW);
            ++ DRV8871_ON;
            break;

        case 4:
            delay(100);
            digitalWrite(DRV8871_IN1, LOW);
            digitalWrite(DRV8871_IN2, LOW);
            ++ DRV8871_ON;
            break;

        case 5:
            digitalWrite(TPS22916_ON, LOW);    // ENABLE(ON) Active High
            digitalWrite(TPS63070_EN, LOW);    // Pull high to enable the device, pull low to disable the device.
            ++ DRV8871_ON;
        break;

        default: break;
    }

    switch (DRV8871_OFF)
    {
        case 1:
            digitalWrite(TPS22916_ON, HIGH);    // ENABLE(ON) Active High
            digitalWrite(TPS63070_EN, HIGH);    // Pull high to enable the device, pull low to disable the device.
            ++ DRV8871_OFF;
            break;

        case 2:
            digitalWrite(DRV8871_IN1, LOW);
            digitalWrite(DRV8871_IN2, LOW);
            ++ DRV8871_OFF;
            break;

        case 3:
            digitalWrite(DRV8871_IN1, LOW);     // Reverse (Current OUT2 → OUT1)
            digitalWrite(DRV8871_IN2, HIGH);
            ++ DRV8871_OFF;
            break;

        case 4:
            delay(100);
            digitalWrite(DRV8871_IN1, LOW);
            digitalWrite(DRV8871_IN2, LOW);
            ++ DRV8871_OFF;
            break;

        case 5:
            digitalWrite(TPS22916_ON, LOW);    // ENABLE(ON) Active High
            digitalWrite(TPS63070_EN, LOW);    // Pull high to enable the device, pull low to disable the device.
            ++ DRV8871_OFF;
        break;

        default: break;
    }

    // switch(stepSerial)
    // {
    //     case 'o':
    //         // 밸브를 켜라는 통신을 받으면
    //         // 밸브 동작 전원 구동 시작
    //         DRV8871_Forward_Drive();

    //         cTime3 = nowTime;
    //         stepSerial = 1;
    //         break;

    //     case 'f':
    //         // 밸브를 켜라는 통신을 받으면
    //         // 밸브 동작 전원 구동 시작
    //         DRV8871_Reverse_Drive();

    //         cTime3 = nowTime;
    //         stepSerial = 1;
    //         break;

    //     case 1:
    //         if(nowTime - cTime3 >= DELAY_TIME)
    //             stepSerial = 2;
    //         break;

    //     case 2:
    //         DRV8871_Brake();
    //         stepSerial = 0;
    //         break;

    //     case 'l':
    // // pins_setup();           // 사용하는 포트 설정
    // // LoRa_setup();           // 로라 통신 설정
    //         transmitData();
    //         stepSerial = 0;
    //         break;

    //     default: break;
    // }
#endif


#if MODULE_TEST
char stepSerial = 0;
byte flag_flow = 0;

    if(Serial.available())
        stepSerial = Serial.read();

    switch(stepSerial)
    {
        case 'o':
            // 밸브를 켜라는 통신을 받으면
            // 밸브 동작 전원 구동 시작
            DRV8871_Forward_Drive();
            delay(dTime);    // valve가 켜지는데 필요한 시간. 사양에 따라서 Impulse Width: 20-500mSec or Min. 50 MS
            // delay(2);    // TPS22916 Ton 1700us, TPS63070 920us, DRV8871 51us

            // 밸브가 다 켜졌으면 동작 OFF
            DRV8871_Brake();

            // 밸브 켜지고 OFF하면 밸브는 상태 유지하는지 확인
            stepSerial = 0;
            break;

        case 's':
            // 밸브를 끄라는 통신을 받으면
            // 밸브 동작 전원 구동 시작
            DRV8871_Reverse_Drive();
            delay(dTime);    // valve가 꺼지는데 필요한 시간. 사양에 따라서 Impulse Width: 20-500mSec or Min. 50 MS
            // delay(4);    // TPS22916 tOFF+tFALL 3155us, DRV8871 2us

            // 밸브가 다 꺼졌으면 동작 OFF
            DRV8871_Brake();


            // 밸브 꺼지고 OFF하면 밸브는 상태 유지하는지 확인

            // 유량 데이터 감지 안함
            pinMode(18, INPUT_PULLUP);
            stepSerial = 0;
            break;

        case 'f':
            flag_flow = 1;
            digitalWrite(TPS22860_ON, HIGH); // Active high switch control input.
            // 1초 동안만 유량 감지하여 데이터 전송
            attachInterrupt(digitalPinToInterrupt(18), getFlow, FALLING);
            lastTime = millis();
            stepSerial = 0;
            break;

        default: break;
    }

    switch(flag_flow)
    {
        case 1:
            nowTime = millis();
            // 동작이 ON 되면 이전 시간에서 1초가 넘었는지 확인
            if(nowTime - lastTime >= 1000)
                ++ flag_flow;
            break;

        case 2:
            detachInterrupt(18);        // 인터럽트 정지
            digitalWrite(TPS22860_ON, LOW); // 전원 차단
            ++ flag_flow;
            break;

        case 3:
            // 로라로 측정한 유량 데이터 전송
            if (LoRaWAN.joined() && !LoRaWAN.busy())
            {
                LoRaWAN.beginPacket();
                LoRaWAN.write((pulse_cnt >> 24) & 0xff);   //유량 카운트 전송
                LoRaWAN.write((pulse_cnt >> 16) & 0xff);   //유량 카운트 전송
                LoRaWAN.write((pulse_cnt >> 8) & 0xff);   //유량 카운트 전송
                LoRaWAN.write(pulse_cnt & 0xff);
                LoRaWAN.endPacket();

                // digitalWrite(13, HIGH);
                ++ flag_flow;
                pulse_cnt = 0;  // 유량 카운트 초기화
            }
            break;

        default: break;
    }
#endif


#define TEST_OTAA_SEND_RECEIVE_MODE 0
#if TEST_OTAA_SEND_RECEIVE_MODE
#include "LoRaWAN.h"

const char *appEui = "70B3D57ED002477B";
const char *appKey = "5F297F25DE28A5B05B93719A89DFE679";
const char *devEui = "009FE879284DCD45";

void setup( void )
{
    Serial.begin(9600);
    while (!Serial) { }

    LoRaWAN.begin(EU868);
    LoRaWAN.joinOTAA(appEui, appKey, devEui);

    Serial.println("JOIN( )");
}

char stepSerial = 0;
void loop( void )
{

    if(Serial.available())
        stepSerial = Serial.read();

    switch(stepSerial)
    {
        case 's':
            // LoRaWAN TRANSMIT
            if (LoRaWAN.joined() && !LoRaWAN.busy())
            {
                LoRaWAN.beginPacket();
                    LoRaWAN.write(0x42);    // 'B'
                    LoRaWAN.write(0x31);
                    LoRaWAN.write(0x33);
                    LoRaWAN.write(0x35);
                    LoRaWAN.write(0x56);    // 'V'
                    LoRaWAN.write(0x31);
                    LoRaWAN.write(0x46);    // 'F'
                    LoRaWAN.write(0x31);
                    LoRaWAN.write(0x32);
                    LoRaWAN.write(0x33);
                    LoRaWAN.write(0x34);
                LoRaWAN.endPacket();

                Serial.println("e");
            }
            stepSerial = 0;
            break;
    }

    // LoRaWAN RECEIVE
    if (LoRaWAN.parsePacket())
    {
        uint32_t size;
        uint8_t data[256];

        size = LoRaWAN.read(&data[0], sizeof(data));

        if (size)
        {
            data[size] = '\0';

            Serial.print(", PORT: ");
            Serial.print(LoRaWAN.remotePort());
            Serial.print(", DATA: \"");
            Serial.print((const char*)&data[0]);
            Serial.println("\"");
        }
    }
}
#endif


#define TEST_OTAA_SEND_MODE_WITH_LED 0
#if TEST_OTAA_SEND_MODE_WITH_LED
#include "LoRaWAN.h"

const char *appEui = "0000000000000000";
const char *appKey = "81d062c5c97b071e835c75f8c35a79d7";
const char *devEui = "8188108ba4a09daa";

// const char *appEui = "70B3D57ED002477B";
// const char *appKey = "5F297F25DE28A5B05B93719A89DFE679";
// const char *devEui = "009FE879284DCD45";

byte led = 0;

void setup( void )
{
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);

    Serial.begin(9600);
    // while (!Serial) { }

    // LoRaWAN.begin(US915);
    LoRaWAN.begin(EU868);
    LoRaWAN.joinOTAA(appEui, appKey, devEui);

    Serial.println("JOIN( )");
}

void loop( void )
{
    LoRaWAN.rejoinOTAA();

        Serial.print("joined: ");
        Serial.print(LoRaWAN.joined());
        Serial.print(", busy: ");
        Serial.println(!LoRaWAN.busy());

    if (LoRaWAN.joined() && !LoRaWAN.busy())
    {
        Serial.print("TRANSMIT( ");
        Serial.print("TimeOnAir: ");
        Serial.print(LoRaWAN.getTimeOnAir());
        Serial.print(", NextTxTime: ");
        Serial.print(LoRaWAN.getNextTxTime());
        Serial.print(", MaxPayloadSize: ");
        Serial.print(LoRaWAN.getMaxPayloadSize());
        Serial.print(", DR: ");
        Serial.print(LoRaWAN.getDataRate());
        Serial.print(", TxPower: ");
        Serial.print(LoRaWAN.getTxPower(), 1);
        Serial.print("dbm, UpLinkCounter: ");
        Serial.print(LoRaWAN.getUpLinkCounter());
        Serial.print(", DownLinkCounter: ");
        Serial.print(LoRaWAN.getDownLinkCounter());
        Serial.println(" )");

        LoRaWAN.beginPacket();
        LoRaWAN.write(0xef);
        LoRaWAN.write(0xbe);
        LoRaWAN.write(0xad);
        LoRaWAN.write(0xde);
        LoRaWAN.endPacket();

        if(led) led = 0;
        else    led = 1;

        digitalWrite(13, led);
    }

    delay(10000);
}
#endif


#define TEST_ABP_SEND_MODE 0
#if TEST_ABP_SEND_MODE
#include "STM32L0.h"
#include "LoRaWAN.h"
#include "TimerMillis.h"

#define REGION_US915

byte ledState = 0;

// const char *devAddr = "01eb3c60";
// const char *nwkSKey = "00f51db86d00440794d1606e092eed7f";
// const char *appSKey = "1d65d021292e4a3488ff1f924201a447";
const char *devAddr = "01c99e3e";
const char *nwkSKey = "86cc2eb854057c2e26f6c3392e33ec98";
const char *appSKey = "0b2c08a5c208b086e73a52c83284b7f1";

TimerMillis transmitTimer;

void transmitCallback(void)
{
    Serial.print("a");
    Serial.print(LoRaWAN.joined()); // joined = 1
    Serial.print(LoRaWAN.busy()); // busy = 0

    if (LoRaWAN.joined() && !LoRaWAN.busy())
    {
        Serial.print("b");
        Serial.print(LoRaWAN.joined());
        Serial.print(LoRaWAN.busy());

        LoRaWAN.beginPacket();
        LoRaWAN.write(0x31);
        LoRaWAN.write(0x32);
        LoRaWAN.write(0x33);
        LoRaWAN.write(0x34);
        LoRaWAN.write(0x35);
        LoRaWAN.endPacket();

        // 검증용 led 깜빡임
        if(ledState)
            ledState = 0;
        else
            ledState = 1;

        digitalWrite(13, ledState);  // red led on, off toggle
    }
}

void setup( void )
{
    Serial.begin(9600);
    while (!Serial);  // serial이 연결전까지 대기

    LoRaWAN.begin(US915);
    LoRaWAN.setSubBand(2);
    LoRaWAN.joinABP(devAddr, nwkSKey, appSKey);

    transmitTimer.start(transmitCallback, 0, 10000);

    // led output port
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);  // start with leds off, since active HIGH
}

void loop( void )
{
}
#endif

#define TEST_POWER_SWITCH 0
#if TEST_POWER_SWITCH
void setup( void )
{
    // TPS22860 enalbe pin Configuration, flowmeter power
    pinMode(TPS22860_ON, OUTPUT);
    digitalWrite(TPS22860_ON, LOW);

    // TPS22916 input voltage switch pin Configuration, valve power input
    pinMode(TPS22916_ON, OUTPUT);   // 3.3VDC 전압 Vin > Vout 연결 선택
    digitalWrite(TPS22916_ON, LOW); // ENABLE(ON) Active High

    // TPS22916 enable pin Configuration, valve power enable
    pinMode(TPS63070_EN, OUTPUT);   // 3.3VDC 전압 Vin > Vout 연결 선택
    digitalWrite(TPS63070_EN, LOW); // ENABLE(ON) Active High

    Serial.begin(9600);
    while (!Serial) { }
    Serial.println("POWER(ON)");
}

uint8_t stepSerial = 0;

void loop( void )
{
    if(Serial.available())
        stepSerial = Serial.read();

    switch (stepSerial)
    {
        case '1':
            Serial.write(stepSerial);
            digitalWrite(TPS63070_EN, HIGH);    // VM 입력 전원 스위치 ON
            digitalWrite(TPS22916_ON, HIGH);    // VM 전원 ON
            digitalWrite(TPS22860_ON, HIGH);    // V_SWITCH 전원 ON
            stepSerial = 0;
            break;

        case '0':
            Serial.write(stepSerial);
            digitalWrite(TPS63070_EN, LOW);     // VM 입력 전원 스위치 ON
            digitalWrite(TPS22916_ON, LOW);     // VM 전원 OFF
            digitalWrite(TPS22860_ON, LOW);     // V_SWITCH 전원 ON
            stepSerial = 0;
            break;

        default: break;
    }
}
#endif
