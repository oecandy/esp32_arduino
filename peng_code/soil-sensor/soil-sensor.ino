
#define RealCODE 1
#if RealCODE

/* includes ---------------------------------------------------------------------------------------------------- */
//#include <STM32L0.h>
#include "LoRaWAN.h"
#include "STM32L0.h"

#include <SPI.h>
#include <EEPROM.h>


/* defines ---------------------------------------------------------------------------------------------------- */
/* check mode */
#define DEBUG_MODE      1

/* LoRa */
#define PACKET_LENGTH       11

/* pins */
#define BATTERY_EN          15      // SCL, PB8

#define MCP3008_CS          9       // D9, PA14, TX3

#define TDC7200_CS          10      // D10, PB12, CS
#define TDC7200_EN          20      // A4, PA0
#define TDC7200_CLK         2       // D2, PB5
#define TDC7200_START       8       // D8, PA13, RX3

#define RF_CTRL3            5       // D5, PB2
#define RF_CTRL2            4       // D4, PB7
#define RF_CTRL1            3       // D3, PB6

#define EC_ground           16      // A0, PA4, DAC0
#define EC_power            14      // SDA, PB9

#define TEMP_ground         1       // D1, PA9, TX1
#define TEMP_power          0       // D0, PA10, RX1

#define ADC_BT              0
#define ADC_TEMP1           1
#define ADC_TEMP2           2
#define ADC_EC1             3
#define ADC_EC2             4
#define ADC_EC3             5


// RIGOL DS1054 기준으로 32MHz / 128 = 250KHz부터 DECODE에서 8bit 한번에 인식 못함
SPISettings settings7200(16000000, MSBFIRST, SPI_MODE0);
SPISettings settings3008(1000000, MSBFIRST, SPI_MODE0);

/* variables ---------------------------------------------------------------------------------------------------- */
// #define DEVICE_NUMBER   99
// #if DEVICE_NUMBER == 1
//     const char *devAddr = "01c99e3e";
//     const char *nwkSKey = "86cc2eb854057c2e26f6c3392e33ec98";
//     const char *appSKey = "0b2c08a5c208b086e73a52c83284b7f1";
// #elif DEVICE_NUMBER == 2
//     const char *devAddr = "015b0fc2";
//     const char *nwkSKey = "e0ec796df503d51d959334fe6d5fb9b1";
//     const char *appSKey = "bf5725b850196e77f154716a0c6aa982";
// #endif

// Thailand SoilSensor
#define DEVICE_NUMBER   6
#if DEVICE_NUMBER == 3      // +3.6VDC 입력에서 대기 전류 4.8mA↓
    const char *devAddr = "017250c0";
    const char *nwkSKey = "aeaadadc325867abd6d0bfe751551eaf";
    const char *appSKey = "b63d63e7cb579d15ccda03e7c4ac41d4";
#elif DEVICE_NUMBER == 4    // +3.6VDC 입력에서 대기 전류 4.3mA↓
    const char *devAddr = "01e4f09d";
    const char *nwkSKey = "a78957053ee9cee1baa37232f9c11ec0";
    const char *appSKey = "aaa3fedb06cd1eb0e671f30e9e0ef5f0";
#elif DEVICE_NUMBER == 5    // +3.6VDC 입력에서 대기 전류 4.3mA↓
    const char *devAddr = "0170159f";
    const char *nwkSKey = "5099668870616add29c01e867a2f79a2";
    const char *appSKey = "272c3aa740c104b76b74c92efc69dbd5";
#elif DEVICE_NUMBER == 6    // +3.6VDC 입력에서 대기 전류 5.6mA↓
    const char *devAddr = "00abf2b7";
    const char *nwkSKey = "0d31d11c28c1faf55c94203597adda17";
    const char *appSKey = "cec375d8d3c2a578b8af21e1ee0e97eb";
#endif


// Thailand SoilSensor
// 대기 전류 3.7mA
// #define DEVICE_NUMBER   2
// #if DEVICE_NUMBER == 1
//     const char *devAddr = "00297fc6";
//     const char *nwkSKey = "2ab932cd1a6a6229e2ded29f6569f827";
//     const char *appSKey = "642f8d777bc92f565f1194eaef270eb6";
// #elif DEVICE_NUMBER == 2
//     const char *devAddr = "015b0fc2";
//     const char *nwkSKey = "e0ec796df503d51d959334fe6d5fb9b1";
//     const char *appSKey = "bf5725b850196e77f154716a0c6aa982";
// #endif

// Kazakstan exhibition
// #if DEVICE_NUMBER == 1
//     const char *devAddr = "01257e6a";
//     const char *nwkSKey = "eb4bd504563410755912d1dc8a627e2a";
//     const char *appSKey = "9bab69a12fc6bedee32681715aabbbe4";
// #endif

// 2Byte 1개, 1Byte 10개 : 배터리 전압, 지습1,지습2,지습3, 지온1,지온2,지온3, EC1,EC2,EC3
byte packetArr[11] = {0x01, 0x12, 0x23, 0x34, 0x45,  0x56, 0x67, 0x78, 0x89, 0x9a,  0xab};
char buf[300];

word tempArr[4] = {0,  0, 0, 0};
byte readStep = 0;
byte saveToF0 = 0, saveToF100 = 0;

word adcToC[42] = {  1016, 1013, 1008, 1001,  993,   981,  965,  945,  920,  888,
                      850,  805,  754,  698,  638,   576,  514,  454,  396,  344,
                      296,  254,  217,  186,  158,   135,  116,  100,   86,   75,
                       65,   57,   50,   44,   39,    35,   32,   29,   27,   24,
                       23,   21};

byte Top[42] =  { 45,  50,  55,  60,  65,   70, 75, 80, 85, 90,
                  95, 100, 105, 110, 115,  120, 125, 130, 135, 140,
                 145, 150, 155, 160, 165,  170, 175, 180, 185, 190,
                 195, 200, 205, 210, 215,  220, 225, 230, 235, 240,
                 245, 250};

float nowTemperature = 0;

/***************************************************************************************************************
*
*  Functions
*
***************************************************************************************************************/

/* General-purpose inputs/outputs (GPIOs) setup ------------------------------------------------------------- */
void LOW_setup (uint32_t ulPin)
{
    pinMode(ulPin, OUTPUT);
    digitalWrite(ulPin, LOW);
}

void HIGH_setup (uint32_t ulPin)
{
    pinMode(ulPin, OUTPUT);
    digitalWrite(ulPin, HIGH);
}

void pins_setup (void)
{
    // battery_pin_configuration
    HIGH_setup(BATTERY_EN);         // battery ENABLE Active High

    // ADCoverter_pin_configuration
    HIGH_setup(MCP3008_CS);          // MCP3008 CS Active Low

    // moisture_pin_configuration
    HIGH_setup(TDC7200_CS);          // TDC7200 CSB Active Low
    LOW_setup(TDC7200_EN);          // TDC7200 ENABLE Active High
    LOW_setup(TDC7200_START);       // TDC7200 START Active High

    LOW_setup(RF_CTRL1);            // PE4259 RF1 on = Active High
    LOW_setup(RF_CTRL2);            // PE4259 RF2 on = Active High
    LOW_setup(RF_CTRL3);            // PE4259 RF3 on = Active High

    // EC_pins_configuration
    LOW_setup(EC_power);            // ADC voltage Active High
    HIGH_setup(EC_ground);          // ADC ground Active Low

    // temperature_pins_configuration
    LOW_setup(TEMP_power);            // ADC voltage Active High
    HIGH_setup(TEMP_ground);          // ADC ground Active Low
}
/* ---------------------------------------------------------------------------------------------------------- */

void LoRaWAN_setup (void)
{
    LoRaWAN.begin(US915);
    LoRaWAN.setSubBand(2);
    LoRaWAN.joinABP(devAddr, nwkSKey, appSKey);
}

/* checke EC  ----------------------------------------------------------------------------------------------- */

    // EC meter
    // ADC 전압 읽어오기
    // 회로 2개가 차이 있는지 확인 (1,2) : 차이 있음
    // 물 vs 소금물 or 공기 중 전압 차이가 있는지 확인 (1-1, 1-2, 2-1, 2-2) : 물 vs 공기 차이 있음
    // 4가지 실험으로 회로와 홀 선택 : 회로 구멍 여부에 따라서 값이 달라지는지 추가로 확인 하기

float TemperatureCoef = 0.019; //this changes depending on what chemical we are measuring

uint32_t raw;

float Vdrop = 0;
float Vin = 3.3;
float Rc = 0;

float Ra=25;          //Resistance of powering Pins
uint32_t R1= 1025;  // R1=(R1+Ra);

float K = 0;

float ConstantK[4] = {0.00,  0.00, 0.00, 0.00};
float ECvalue[4] = {0,  0, 0, 0};

void EC_calibration (byte Num)
{
#if DEBUG_MODE
    Serial.print("wait 10s... \r\n");
#endif
    // EC caliabration
    byte i = 0;
    uint32_t buffer = 0;
    float calEC = 0;
    float CalibrationEC = 0.68; //EC value of Calibration solution is s/cm
    // float CalibrationEC = 0.762; //EC value of Calibration solution is s/cm
    // float CalibrationEC = 1.38; //EC value of Calibration solution is s/cm

    //************Estimates Resistance of Liquid ****************//
    while(i <= 10) {
        digitalWrite(EC_power, HIGH);
        digitalWrite(EC_ground, LOW);

        if(Num == 1)        raw = Read_ADC(ADC_EC1);
        else if(Num == 2)   raw = Read_ADC(ADC_EC2);
        else if(Num == 3)   raw = Read_ADC(ADC_EC3);
        // raw = Read_ADC(ADC_EC1); // This is not a mistake, First reading will be low

        digitalWrite(EC_power, LOW);
        digitalWrite(EC_ground, HIGH);

        buffer = buffer + raw;
        i++;
        delay(1000);

#if DEBUG_MODE
    sprintf(buf, "ADC %d = %d, buffer = %d \r\n", i, raw, buffer);
    Serial.print(buf);
#endif
    };

    raw = (buffer / 10);
#if DEBUG_MODE
    sprintf(buf, "buffer = %d, raw = %d \r\n", buffer, raw);
    Serial.print(buf);
#endif

// sensors.requestTemperatures();// Send the command to get temperatures
// TemperatureFinish=sensors.getTempCByIndex(0); //Stores Value in Variable

    //*************Compensating For Temperaure********************//
#if DEBUG_MODE
    Serial.print(" nowTemperature = ");
    Serial.print(nowTemperature);
#endif
    calEC = CalibrationEC * (1 + (TemperatureCoef * (nowTemperature - 25.0))) ;
    // calEC = CalibrationEC * (1 + (TemperatureCoef * (27.2 - 25.0))) ;
// EC = CalibrationEC*(1+(TemperatureCoef*(TemperatureFinish-25.0))) ;

    //***************** Calculates R relating to Calibration fluid **************************//
    Vdrop = (((Vin) * ((float)raw)) / 1024.0);
    Rc = (Vdrop * (float)R1) / (Vin - Vdrop);
    Rc = Rc - Ra;

    ConstantK[Num] = 1000 / (Rc * calEC);
    // K = 1000 / (Rc * calEC);

#if DEBUG_MODE
    Serial.print(" calEC = ");
    Serial.print(calEC);

    Serial.print(", Cell Constant K = ");
    Serial.print(ConstantK[Num]);
    // Serial.print(K);
#endif
    if(Num == 1)        EEPROM.put(50, ConstantK[Num]);
    else if(Num == 2)   EEPROM.put(60, ConstantK[Num]);
    else if(Num == 3)   EEPROM.put(70, ConstantK[Num]);

#if DEBUG_MODE
    float f = 0.00f;

    if(Num == 1)        EEPROM.get( 50, f );
    else if(Num == 2)   EEPROM.get( 60, f );
    else if(Num == 3)   EEPROM.get( 70, f );

    Serial.print(", save Constant K = ");
    Serial.println(f);
#endif
}


void read_EC_value (byte Num)
{
    float PPMconversion=0.7;

    float EC = 0;
    //float EC25 = 0;

    int ppm = 0;

    //************Estimates Resistance of Liquid ****************//
    // ADC 전압, GND 공급
    digitalWrite(EC_power, HIGH);
    digitalWrite(EC_ground, LOW);
    // ADC 읽어오기
    if(Num == 1)        raw = Read_ADC(ADC_EC1);
    else if(Num == 2)   raw = Read_ADC(ADC_EC2);
    else if(Num == 3)   raw = Read_ADC(ADC_EC3);
    // raw = Read_ADC(ADC_EC1);
    // ADC 전압, GND 차단
    digitalWrite(EC_power, LOW);
    digitalWrite(EC_ground, HIGH);

    //***************** Converts to EC **************************//
    Vdrop= (Vin*(float)raw)/1024.0;
    Rc=(Vdrop*(float)R1)/(Vin-Vdrop);
    Rc=Rc-Ra; //acounting for Digital Pin Resitance
    EC = 1000/(Rc*ConstantK[Num]);
    // EC = 1000/(Rc*K);

#if DEBUG_MODE
    Serial.print(" ConstantK[Num] = ");
    Serial.print(ConstantK[Num]);
    Serial.print(",  ");
#endif

    //*************Compensating For Temperaure********************//
    ECvalue[Num] = EC/ (1+ TemperatureCoef*(nowTemperature-25.0));
    // ECvalue[Num] = EC/ (1+ TemperatureCoef*(27.2-25.0));
    // EC25 = EC/ (1+ TemperatureCoef*(27.2-25.0));
    ppm = (int)((ECvalue[Num])*(PPMconversion*1000));
    // ppm = (int)((EC25)*(PPMconversion*1000));
// EC25  =  EC/ (1+ TemperatureCoef*(Temperature-25.0));
// ppm=(EC25)*(PPMconversion*1000);

#if DEBUG_MODE
    Serial.print(" Rc = ");
    Serial.print(Rc);
    Serial.print(", EC25 = ");
    Serial.print(ECvalue[Num]);
    // Serial.print(EC25);
    sprintf(buf, ", ppm = %d \r\n", ppm);
    Serial.print(buf);
#endif
}

void Init_EC_value (void)
{
    EEPROM.get( 50, ConstantK[1] );
    EEPROM.get( 60, ConstantK[2] );
    EEPROM.get( 70, ConstantK[3] );
}
/* ---------------------------------------------------------------------------------------------------------- */

void send_LoRaWan_Data (void)
{
    if (LoRaWAN.joined() && !LoRaWAN.busy())
    {
        LoRaWAN.beginPacket();
        LoRaWAN.write(packetArr, PACKET_LENGTH);
        LoRaWAN.endPacket();

    #if DEBUG_MODE
        Serial.write(packetArr, PACKET_LENGTH);
    #endif
    }
}


/* Timer PWM ------------------------------------------------------------------------------------------------ */
void TIM22_CH2_PWM_setup (void)
{
    analogWrite(2, 1);  // D2, PB5, TIM22_CH2

    RCC->APB2ENR |= (0x1 << 5);

    TIM22->CR1 = 0;
    /* Set the Autoreload value */
    TIM22->ARR = 1;
    /* Set the Prescaler value */
    // TIM22->PSC = 15; // 2Mhz
    // TIM22->PSC = 7; // 4Mhz
    // TIM22->PSC = 3; // 8Mhz
    TIM22->PSC = 1; // 16Mhz
    /* Generate an update event to reload the Prescaler
    and the repetition counter (only for advanced timer) value immediately */
    TIM22->EGR = 0;

    //sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    TIM22->SMCR = 0;

    /* Write to TIMx CR2 */
    TIM22->CR2 = 0;

    /* Write to TIMx CCMR1 */
    TIM22->CCMR1 = 0x0006C00;

    /* Set the Capture Compare Register value */
    TIM22->CCR2 = 1;

    /* Write to TIMx CCER */
    TIM22->CCER = 0;
}

void TIM22_CH2_PWM_start (void)
{
    TIM22->CR1 = 1;
    TIM22->SR = 7;
    TIM22->CCER = 0x10;
}


void TIM22_CH2_PWM_stop (void)
{
    TIM22->CR1 = 0;
    TIM22->CCER = 0x00;
}
/* ---------------------------------------------------------------------------------------------------------- */


/* Time-to-Digital Converter for Time-of-Flight(TOF) -------------------------------------------------------- */
byte tofStep = 1;
byte tofNumber = 5;
byte tofCount = 1;
// 1번부터 사용하면서 숫자 맞추기 위한 사용하지 않는 배열 0번 추가
uint32_t MeasuredTime[4][11] = { {0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0}, {0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0},
                                 {0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0}, {0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0} };
uint32_t AverageTime[4] = {0,  0, 0, 0};
uint32_t minToF[4] = {0,  0, 0, 0};
uint32_t maxToF[4] = {0,  0, 0, 0};

void Select_RF_Channel (byte selNum, byte motion)
{
    if(motion)  // 동작이면
    {
        switch (selNum) // 선택된 채널 HIHG 설정
        {
            case 1: digitalWrite(RF_CTRL1, HIGH); break;
            case 2: digitalWrite(RF_CTRL2, HIGH); break;
            case 3: digitalWrite(RF_CTRL3, HIGH); break;
            default: break;
        }
    }
    else    // 동작이 아니면
    {
        switch (selNum) // 선택된 채널 LOW 설정
        {
            case 1: digitalWrite(RF_CTRL1, LOW); break;
            case 2: digitalWrite(RF_CTRL2, LOW); break;
            case 3: digitalWrite(RF_CTRL3, LOW); break;
            default: break;
        }
    }
}

void Measurement_TOF (byte selNum)
{
    switch(tofStep)
    {
        case 1:
            SPI.begin();
            // TDC7200 SCLK Frequency MAX 20MHz
            SPI.beginTransaction(settings7200);
            ++ tofStep;
            break;

        case 2:     // TDC7200 신호 Clock START , Enable START, RF CTRL ON
            Select_RF_Channel(selNum, 1);
            digitalWrite(TDC7200_EN, HIGH);
            TIM22_CH2_PWM_start();
            delay(2);   // TIMING REQUIREMENTS: ENABLE INPUT = 100 ns
            ++ tofStep;
            break;

        case 3:     // TDC7200 설정 CONFIG1: Configuration Register 1 Write (address = 00h)
            digitalWrite(TDC7200_CS, LOW);
            SPI.transfer16(0x4019);             
            digitalWrite(TDC7200_CS, HIGH);
            ++ tofStep;
            break;

        case 4:     // TDC7200 설정 CONFIG2: Configuration Register 2 Write (address = 01h)
            digitalWrite(TDC7200_CS, LOW);
            SPI.transfer16(0x41C0);
            digitalWrite(TDC7200_CS, HIGH);
            ++ tofStep;
            break;

        case 5:     // RF 신호 출력
            digitalWrite(TDC7200_START, HIGH);
            digitalWrite(TDC7200_START, LOW);
            ++ tofStep;
            break;

        case 6:     // 출력한 RF 신호가 되돌아 오는 시간 확인
            digitalWrite(TDC7200_CS, LOW);
            SPI.transfer(0x10);
            MeasuredTime[tofNumber][tofCount] = SPI.transfer(0x00);   // data
            MeasuredTime[tofNumber][tofCount] <<= 8;
            MeasuredTime[tofNumber][tofCount] |= SPI.transfer(0x00);   // data
            MeasuredTime[tofNumber][tofCount] <<= 8;
            MeasuredTime[tofNumber][tofCount] |= SPI.transfer(0x00);   // data
            digitalWrite(TDC7200_CS, HIGH);
        #if DEBUG_MODE
            sprintf(buf, "t = %ld \r\n", MeasuredTime[tofNumber][tofCount]);
            Serial.print(buf);
        #endif
            ++ tofStep;
            break;

        case 7:     // SPI
            Select_RF_Channel(selNum, 0);
            digitalWrite(TDC7200_EN, LOW);
            TIM22_CH2_PWM_stop();
            SPI.endTransaction();
            SPI.end();
            ++ tofStep;
            break;

        default: break;
    }
}

void Average_tof_Time (void)
{
    if(tofNumber < 4)       // 1번, 2번, 3번 선택 값이면
    {
        if(tofCount < 11)   // 10번 측정하여 평균 값 측정하기
        {
            Measurement_TOF(tofNumber);

            if(tofStep == 8)    // 1번 측정했으면
            {
                tofStep = 1;    // step 초기화
                ++ tofCount;    // count 증가
            }
        }
        else                // 10번 측정했으면
        {
            if(tofCount == 11)
            {
                tofCount = 1;
                ++ tofNumber;   // 다음 위치 측정
            }
        }
    }
    else
    {
        if(tofNumber == 4)      // 10번씩, 3곳. 30번 측정이 다 되었으면
        {
            uint32_t sumTime[4] = {0,  0, 0, 0};

            for(char Num=1; Num<4; ++Num)
            {
                for(char Cnt=1; Cnt<11; ++Cnt)
                    sumTime[Num] += MeasuredTime[Num][Cnt];

                AverageTime[Num] = sumTime[Num] / 10;

            #if DEBUG_MODE
                sprintf(buf, "sum1 = %ld, ave1 = %ld \r\n", sumTime[Num], AverageTime[Num]);
                Serial.print(buf);
            #endif
            }

            ++ tofNumber;
        }
    }
}

void SaveTimeOfFlight0 (void)
{
#if DEBUG_MODE
    uint32_t read0[4] = {0,  0, 0, 0};
#endif

    EEPROM.write( 1, (AverageTime[1]>>24)&0xff );
    EEPROM.write( 2, (AverageTime[1]>>16)&0xff );
    EEPROM.write( 3, (AverageTime[1]>>8)&0xff );
    EEPROM.write( 4, (AverageTime[1]&0xff) );

    EEPROM.write( 11, (AverageTime[2]>>24)&0xff );
    EEPROM.write( 12, (AverageTime[2]>>16)&0xff );
    EEPROM.write( 13, (AverageTime[2]>>8)&0xff );
    EEPROM.write( 14, (AverageTime[2]&0xff) );

    EEPROM.write( 21, (AverageTime[3]>>24)&0xff );
    EEPROM.write( 22, (AverageTime[3]>>16)&0xff );
    EEPROM.write( 23, (AverageTime[3]>>8)&0xff );
    EEPROM.write( 24, (AverageTime[3]&0xff) );

#if DEBUG_MODE
    // 제대로 저장이 되었는지 번지 값을 읽어서 확인
    read0[1] = EEPROM.read(4) | (EEPROM.read(3)<<8) | (EEPROM.read(2)<<16) | (EEPROM.read(1)<<24);
    read0[2] = EEPROM.read(14) | (EEPROM.read(13)<<8) | (EEPROM.read(12)<<16) | (EEPROM.read(11)<<24);
    read0[3] = EEPROM.read(24) | (EEPROM.read(23)<<8) | (EEPROM.read(22)<<16) | (EEPROM.read(21)<<24);

    sprintf(buf, "read01 = %ld, read02 = %ld, read03 = %ld \r\n", read0[1], read0[2], read0[3]);
    Serial.print(buf);
#endif
}

void SaveTimeOfFlight100 (void)
{
#if DEBUG_MODE
    uint32_t read100[4] = {0,  0, 0, 0};
#endif

    EEPROM.write( 6, (AverageTime[1]>>24)&0xff );
    EEPROM.write( 7, (AverageTime[1]>>16)&0xff );
    EEPROM.write( 8, (AverageTime[1]>>8)&0xff );
    EEPROM.write( 9, (AverageTime[1]&0xff) );
 
    EEPROM.write( 16, (AverageTime[2]>>24)&0xff );
    EEPROM.write( 17, (AverageTime[2]>>16)&0xff );
    EEPROM.write( 18, (AverageTime[2]>>8)&0xff );
    EEPROM.write( 19, (AverageTime[2]&0xff) );
 
    EEPROM.write( 26, (AverageTime[3]>>24)&0xff );
    EEPROM.write( 27, (AverageTime[3]>>16)&0xff );
    EEPROM.write( 28, (AverageTime[3]>>8)&0xff );
    EEPROM.write( 29, (AverageTime[3]&0xff) );

#if DEBUG_MODE
    // 제대로 저장이 되었는지 번지 값을 읽어서 확인
    read100[1] = EEPROM.read(9) | (EEPROM.read(8)<<8) | (EEPROM.read(7)<<16) | (EEPROM.read(6)<<24);
    read100[2] = EEPROM.read(19) | (EEPROM.read(18)<<8) | (EEPROM.read(17)<<16) | (EEPROM.read(16)<<24);
    read100[3] = EEPROM.read(29) | (EEPROM.read(28)<<8) | (EEPROM.read(27)<<16) | (EEPROM.read(26)<<24);

    sprintf(buf, "read1001 = %ld, read1002 = %ld, read1003 = %ld \r\n", read100[1], read100[2], read100[3]);
    Serial.print(buf);
#endif
}

void Init_ToF_value (void)
{
    minToF[1] = EEPROM.read(4) | (EEPROM.read(3)<<8) | (EEPROM.read(2)<<16) | (EEPROM.read(1)<<24);
    minToF[2] = EEPROM.read(14) | (EEPROM.read(13)<<8) | (EEPROM.read(12)<<16) | (EEPROM.read(11)<<24);
    minToF[3] = EEPROM.read(24) | (EEPROM.read(23)<<8) | (EEPROM.read(22)<<16) | (EEPROM.read(21)<<24);

    maxToF[1] = EEPROM.read(9) | (EEPROM.read(8)<<8) | (EEPROM.read(7)<<16) | (EEPROM.read(6)<<24);
    maxToF[2] = EEPROM.read(19) | (EEPROM.read(18)<<8) | (EEPROM.read(17)<<16) | (EEPROM.read(16)<<24);
    maxToF[3] = EEPROM.read(29) | (EEPROM.read(28)<<8) | (EEPROM.read(27)<<16) | (EEPROM.read(26)<<24);
}


byte ToF_Percent (byte Num)
{
    // (읽어온 값 - 0% 값) / (100% 값 / 100)
    // ex1) 0% - 200, 100% = 3400, 읽은 값이 1000이면 
    //      (1000 - 200) / (3400/100) = 800 / 34 = 23%
    // ex2) 0% - 200, 100% = 3400, 읽은 값이 2200이면 
    //      (2200 - 200) / (3400/100) = 800 / 34 = 58%
#if DEBUG_MODE
    sprintf(buf, ",  (AT%d = %ld, min%d = %ld, max%d = %ld \r\n", Num, AverageTime[Num], Num, minToF[Num], Num, maxToF[Num]);
    Serial.print(buf);
#endif

    if(AverageTime[Num] <= minToF[Num])         // 읽어 온 값이 최소 값보다 작거나 같으면 최소값으로 적용
        return 0;
    else if(AverageTime[Num] >= maxToF[Num])    // 읽어 온 값이 최대 값보다 크거나 같으면 최대값으로 적용
        return 100;
    else
        return (byte)( (AverageTime[Num]-minToF[Num])/(maxToF[Num]/100) );
}

/* ---------------------------------------------------------------------------------------------------------- */

/* 8-Channel 10-Bit A/D Converters with SPI Serial Interface ------------------------------------------------ */
word Read_ADC (byte channel)
{
    byte tFirstBit = 0x01;   // 0 0 0 0  0 0 0 1 (Start Bit)
    byte tSecondBit = 0x80;  // 1 0 0 0  x x x x (Single/Diff D2 D1 D0)
    byte tLastBit = 0x00;    // x x x x  x x x x (x = “Don’t Care” Bits)
    byte rFirstBit = 0x00;
    byte rSecondBit = 0x04;
    byte rLastBit = 0x00;
    word Data = 0x0000;
#if DEBUG_MODE
    Serial.print("ch:");
    Serial.print(channel);
#endif

    tSecondBit |= (channel << 4);

    digitalWrite(MCP3008_CS, LOW);              // SPI enable 동작
    SPI.begin();                                // SPI 시작
    SPI.beginTransaction(settings3008);         // SPI 설정 시작

    rFirstBit = SPI.transfer(tFirstBit);        // Start SPI Communication with the MCP3008
    rSecondBit = SPI.transfer(tSecondBit);      // MCU Transmitted Data (selected Channel) & MCU Received Data (B9, B8)
    rLastBit = SPI.transfer(tLastBit);          // MCU Received Data (B7, B6, B5, B4, B3, B2, B1, B0)

    digitalWrite(MCP3008_CS, HIGH);              // SPI enable 동작
    SPI.endTransaction();                       // SPI 설정 끝
    SPI.end();                                  // SPI 종료

#if DEBUG_MODE
    Serial.print(",  t");
    Serial.print(tFirstBit);
    Serial.print(",");
    Serial.print(tSecondBit);
    Serial.print(",");
    Serial.print(tLastBit);
    Serial.print(",  r");
    Serial.print(rFirstBit);
    Serial.print(",");
    Serial.print(rSecondBit);
    Serial.print(",");
    Serial.print(rLastBit);
    Serial.print(",");
#endif

    if(rSecondBit & 0x04)   // 3번째 bit가 Null이 아니면 
        return 0;             // 값을 사용하지 않음
    else
    {
        Data = ( ((rSecondBit & 0x03) << 8) | rLastBit );   // BIT1, BIT0만 사용하여 ADC 값 계산
#if DEBUG_MODE
    Serial.print("  Data = ");
    Serial.println(Data);
#endif
        return Data;
    }
}
/* ---------------------------------------------------------------------------------------------------------- */


/* Read ADC battery voltage  -------------------------------------------------------------------------------- */
byte check_battery_voltage (byte num)
{
    word adcBT = Read_ADC(ADC_BT);    // 배터리 전압 ADC로 읽어오기

    // 전압 분배로 인하여 ADC로 읽어온 값이 0.75188배 낮아짐
    // 실제 전압은 읽어온 ADC 값에 낮아진 비율만큼 나눠서 계산해야 함
    // 읽어온 ADC 값을 전압으로 변환
    double BTvoltage = (double)adcBT / 0.75188 * 3.3 / 1023;
    word BTdata = (word)(BTvoltage*100); // 전압으로 변경된 ADC 값에 소수점 두자리까지 정수로 사용하기 위해 * 100
    byte BTdata1 = (byte)((BTdata & 0xff00) >> 8);  // 상위 1byte
    byte BTdata2 = (byte)(BTdata & 0x00ff);         // 하위 1byte

    if(num)     // 0x1234에서 1byte만(0x12) 사용
        return BTdata1;
    else        // 0x1234에서 1byte만(0x34) 사용
        return BTdata2;

    // if(BTdata < 270)     // 배터리 전압이 2.70V보다 밑으로 내려가면
    //     digitalWrite(BATTERY_EN, LOW);   // 신호 LOW 출력하여 전원 차단
}
/* ---------------------------------------------------------------------------------------------------------- */


/* change temperature from ADC voltage ---------------------------------------------------------------------- */
byte check_temp (byte num)
{
    word adcTemp[4] = {0,  0, 0, 0};
    byte tempToC = 100;
    word plusTemp[4] = {0,  0, 0, 0};

    digitalWrite(TEMP_power, HIGH);
    digitalWrite(TEMP_ground, LOW);
    delay(10);   // 전압이 흐르는 시간 대기

    if(num == 1)        adcTemp[num] = Read_ADC(ADC_TEMP1);
    else if(num == 2)   adcTemp[num] = Read_ADC(ADC_TEMP2);
    // else if(num == 3)   adcTemp[num] = Read_ADC(ADC_TEMP3);

    digitalWrite(TEMP_power, LOW);
    digitalWrite(TEMP_ground, HIGH);

    for(char i=0; i<42; ++i)
    {
    /*
        // ADC 454이면
        // 576,  514,  454,  396,  344

        if(454 >= 514)

        if(454 >= 454)
     */
        if(adcTemp[num] >= adcToC[i])       // 읽어온 전압이 기준 값보다 높으면
        {
            if(adcTemp[num] == adcToC[i])   // 읽어온 전압이 기준 값과 동일하면
                tempToC = Top[i];           // 기준 값을 온도라고 판단
            else
            {

    /*
        // ADC 500이면
        // 576,  514,  454,  396,  344

        if(500 >= 514)

        if(500 >= 454)

                plusTemp[0] = 454 + (((514-454)/5)*4) = 502; 26
                plusTemp[1] = 454 + (((514-454)/5)*3) = 490; 27
                plusTemp[2] = 454 + (((514-454)/5)*2) = 478; 28
                plusTemp[3] = 454 + (((514-454)/5)*1) = 466; 29
     */

/*
        // ADC 505이면
        // 514,  454

        if(505 >= 514)
        if(505 >= 454)

                plusTemp[0] = 454 + (((514-454)/5)*4) = 502; 26
                plusTemp[1] = 454 + (((514-454)/5)*3) = 490; 27
                plusTemp[2] = 454 + (((514-454)/5)*2) = 478; 28
                plusTemp[3] = 454 + (((514-454)/5)*1) = 466; 29
 */
                plusTemp[0] = adcToC[i] + (((adcToC[i-1]-adcToC[i])/5)*4);
                plusTemp[1] = adcToC[i] + (((adcToC[i-1]-adcToC[i])/5)*3);
                plusTemp[2] = adcToC[i] + (((adcToC[i-1]-adcToC[i])/5)*2);
                plusTemp[3] = adcToC[i] + (((adcToC[i-1]-adcToC[i])/5)*1);

        // #if DEBUG_MODE
        //     sprintf(buf, ",  (adcToC[i] = %d, adcToC[i-1] = %d \r\n", adcToC[i], adcToC[i-1]);
        //     Serial.println(buf);

        //     sprintf(buf, ",  pulseTemp0 = %d, pulseTemp1 = %d, pulseTemp2 = %d, pulseTemp3 = %d \r\n", plusTemp[0], plusTemp[1], plusTemp[2], plusTemp[3]);
        //     Serial.println(buf);
        // #endif

                for(char j=0; j<4; ++j)
                {
                    if(adcTemp[num] >= plusTemp[j])
                    {
                        tempToC = Top[i]+j+1;
                        break;
                    }
                }
            }

        // #if DEBUG_MODE
        //     sprintf(buf, ",  adc10bit%d = %d, adcToC = %d, Top = %d, tempToC = %d \r\n", num, adcTemp[num], adcToC[i], Top[i], tempToC);
        //     Serial.println(buf);
        // #endif

            break;
        }
    }

    return tempToC;
}
/* ---------------------------------------------------------------------------------------------------------- */


void setup()
{
    pins_setup();
    LoRaWAN_setup();
    TIM22_CH2_PWM_setup();

    Init_EC_value();
    Init_ToF_value();

#if DEBUG_MODE
    readStep = 9;
    Serial.begin(9600);
    while (!Serial) { }
    Serial.println("POWER(ON)");
#endif
}

char stepSerial = 0;
byte a = 0x2a;
byte addr = 2;
word adcValue = 0x0000;
byte temp1 = 0;
byte temp2 = 0;
word t_1temp = 0;
word t_2temp = 0;

void loop()
{
    if(Serial.available())
        stepSerial = Serial.read();

    switch(stepSerial)
    {
// save ToF value
        case 'o':       // 1번, 2번, 3번 습도 0% 조건에서 값 저장
            tofNumber = 1;
            saveToF0 = 1;
            stepSerial = 0;
            break;

        case 'i':       // 1번, 2번, 3번 습도 100% 조건에서 값 저장
            tofNumber = 1;
            saveToF100 = 1;
            stepSerial = 0;
            break;

// A/D Converter test
        // case '1': case '2': case '3': case '4': case '5':
        // case '6': case '7': case '8':
        //     adcValue = Read_ADC(stepSerial-0x30);   // ADC 읽고
        //     Serial.print("adc=");
        //     Serial.print(adcValue);
        //     stepSerial = 0;
        //     break;

// internal EEPROM TEST
        // case '1':   // 변수 출력
        //     Serial.write(a);
        //     stepSerial = 0;
        //     break;

        // case '2':   // 변수 읽기
        //     a = EEPROM.read(addr);
        //     stepSerial = 0;
        //     break;

        // case '3':   // 변수 저장
        //     EEPROM.write(addr, 0x1e);
        //     stepSerial = 0;
        //     break;
////////////////////

// TDC RF TEST
        // case '1':
        //     tofNumber = 1;
        //     Serial.print(tofNumber);
        //     Serial.print(",");
        //     Serial.print(tofCount);
        //     Serial.print(",");
        //     Serial.print(tofStep);
        //     Serial.print(" \r\n");
        //     stepSerial = 0;
        //     break;
////////////////////

// ADC EC value TEST
        case '1': case '2': case '3':
            temp1 = check_temp(1);
            nowTemperature = ((float)temp1-100);
            EC_calibration(stepSerial-0x30);
            stepSerial = 0;
            break;

        // case '6': case '7': case '8':
        //     read_EC_value(stepSerial-0x35);
        //     stepSerial = 0;
        //     break;
////////////////////

// check TEMP power time
        // case 'w':
        //     digitalWrite(TEMP_power, HIGH);
        //     digitalWrite(TEMP_ground, LOW);
        //     stepSerial = 0;
        //     break;
        // case 'e':
        //     t_1temp = Read_ADC(ADC_TEMP1);
        //     t_2temp = Read_ADC(ADC_TEMP2);
        //     stepSerial = 0;
        //     break;
        // case 'r':
        //     digitalWrite(TEMP_power, LOW);
        //     digitalWrite(TEMP_ground, HIGH);
        //     stepSerial = 0;    
        //     break;
////////////////////

        case 't':
            temp1 = check_temp(1);
            Serial.print("temp1 = ");
            Serial.print(temp1);
            Serial.print(", ");
            temp2 = check_temp(2);
            Serial.print("temp2 = ");
            Serial.println(temp2);
            stepSerial = 0;
            break;

        case 's':
            readStep = 0;
            stepSerial = 0;
            break;

        case 'p':
            readStep = 9;
            stepSerial = 0;
            break;

        default: break;
    }


    if(saveToF0)    // 습도 값 0% 조건에서 측정하여 저장
    {
        Average_tof_Time();
        
        if(tofNumber == 5)
        {
            SaveTimeOfFlight0();
            saveToF0 = 0;
        }
    }

    if(saveToF100)  // 습도 값 100% 조건에서 측정하여 저장
    {
        Average_tof_Time();
        
        if(tofNumber == 5)
        {
            SaveTimeOfFlight100();
            saveToF100 = 0;
        }
    }

    switch(readStep)
    {
        case 0:     // 습도 읽기 시작
            tofNumber = 1;
            ++ readStep;
            break;

        case 1:
            Average_tof_Time();
            if(tofNumber == 5) ++ readStep; // 습도를 다 읽으면 다음으로 이동
            break;

        case 2:     // 온도 읽기
            temp1 = check_temp(1);
            nowTemperature = ((float)temp1-100);
            ++ readStep;
            break;

        case 3:     // EC 읽기
            read_EC_value(1);
            read_EC_value(2);
            read_EC_value(3);
            ++ readStep;
            break;

        case 4:     // 읽어온 정보를 LoRaWAN으로 전송
            packetArr[0] = check_battery_voltage(1);
            packetArr[1] = check_battery_voltage(0);
        #if DEBUG_MODE
            Serial.println(" ");
            Serial.print(" packetArr 0 = ");
            Serial.print(packetArr[0]);
            Serial.print(", 1 = ");
            Serial.println(packetArr[1]);
        #endif

            packetArr[2] = ToF_Percent(1);
            packetArr[3] = ToF_Percent(2);
            packetArr[4] = ToF_Percent(3);
        #if DEBUG_MODE
            Serial.print(" , 2 = ");
            Serial.print(packetArr[2]);
            Serial.print(" , 3 = ");
            Serial.print(packetArr[3]);
            Serial.print(" , 4 = ");
            Serial.println(packetArr[4]);
        #endif

            packetArr[5] = check_temp(1);
            packetArr[6] = check_temp(2);
            packetArr[7] = 100;
            // packetArr[7] = check_temp(3);
        #if DEBUG_MODE
            Serial.print(" , 5 = ");
            Serial.print(packetArr[5]);
            Serial.print(" , 6 = ");
            Serial.print(packetArr[6]);
            Serial.print(" , 7 = ");
            Serial.println(packetArr[7]);
        #endif

            packetArr[8] = (byte)(ECvalue[1]*100);
            packetArr[9] = (byte)(ECvalue[2]*100);
            packetArr[10] = (byte)(ECvalue[3]*100);
        #if DEBUG_MODE
            Serial.print(" , 8 = ");
            Serial.print(packetArr[8]);
            Serial.print(" , 9 = ");
            Serial.print(packetArr[9]);
            Serial.print(" , 10 = ");
            Serial.println(packetArr[10]);
        #endif

            send_LoRaWan_Data();
            ++ readStep;
            break;

        case 5:     // 30초동안 저전력으로 대기
            // LoRaWAN 전송 후 정해진 시간만큼 sleep mode로 대기
            // STM32L0.stop(10000);    // 10000ms = 10s
            STM32L0.stop(30000);    // 30000ms = 30s
            // ++ readStep;    // 30초 지나면 동작 시작
            readStep = 0;
            break;

        default: break;
    }
}

#endif /* RealCODE END */



#define TOFtest 0
#if TOFtest
char stepSerial = 0;
byte runRF = 0;

byte readSPI1 = 0xFF;
byte readSPI2 = 0xFF;
byte readSPI3 = 0xFF;
uint32_t val = 0;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

void loop()
{
    byte readSPI = 0xFF;

    if(Serial.available())
        stepSerial = Serial.read();

    switch(stepSerial)
    {
// TDC RF TEST
        case '1':
            runRF = 1;
            // Serial.print(runRF);
            stepSerial = 0;
            break;
////////////////////

// TDC SPI TEST
        // case '1':
        //     digitalWrite(RF_CTRL1, HIGH);
        //     digitalWrite(TDC7200_CS, LOW);
        //     digitalWrite(TDC7200_EN, HIGH);
        //     stepSerial = 0;
        //     break;

        // // case '2':   // config1 read
        // //     SPI.transfer(0x00);   // command
        // //     readSPI = SPI.transfer(0x00);   // data
        // //     Serial.write(readSPI);
        // //     digitalWrite(TDC7200_CS, HIGH);
        // //     digitalWrite(RF_CTRL1, LOW);
        // //     stepSerial = 0;
        // //     break;

        // // case '3':   // config1 write
        // //     SPI.transfer(0x40);
        // //     SPI.transfer(0x19);
        // //     // SPI.transfer16(0x4019);
        // //     digitalWrite(TDC7200_CS, HIGH);
        // //     digitalWrite(RF_CTRL1, LOW);
        // //     stepSerial = 0;
        // //     break;

        // case '4':   // config2 read
        //     // readSPI = SPI.transfer16(0x0101);
        //     SPI.transfer(0x01);   // command
        //     readSPI = SPI.transfer(0x01);   // data
        //     Serial.write(readSPI);
        //     digitalWrite(TDC7200_CS, HIGH);
        //     digitalWrite(RF_CTRL1, LOW);
        //     stepSerial = 0;
        //     break;

        // case '5':   // config2 write
        //     SPI.transfer16(0x41C0);
        //     // SPI.transfer(0x41);
        //     // SPI.transfer(0xC0);
        //     digitalWrite(TDC7200_CS, HIGH);
        //     stepSerial = 0;
        //     break;
////////////////////
        default: break;
    }

    switch(runRF)
    {
        case 1:     // Clock START , Enable START, RF CTRL ON
            digitalWrite(RF_CTRL1, HIGH);
            digitalWrite(TDC7200_EN, HIGH);
            TIM22_CH2_PWM_start();
            delay(2);   // TIMING REQUIREMENTS: ENABLE INPUT = 100 ns
            ++ runRF;
            break;

        case 2:
            digitalWrite(TDC7200_CS, LOW);
            SPI.transfer16(0x4019);
            digitalWrite(TDC7200_CS, HIGH);
            ++ runRF;
            break;

        case 3:
            digitalWrite(TDC7200_CS, LOW);
            SPI.transfer16(0x41C0);
            digitalWrite(TDC7200_CS, HIGH);
            ++ runRF;
            break;

        case 4:
            digitalWrite(TDC7200_START, HIGH);
            digitalWrite(TDC7200_START, LOW);
            ++ runRF;
            break;

        case 5:
            digitalWrite(TDC7200_CS, LOW);
            SPI.transfer(0x10);
            val = SPI.transfer(0x00);   // data
            val <<= 8;
            val |= SPI.transfer(0x00);   // data
            val <<= 8;
            val |= SPI.transfer(0x00);   // data
            digitalWrite(TDC7200_CS, HIGH);
            // Serial.write(readSPI1);
            // Serial.write(readSPI2);
            // Serial.write(readSPI3);
            sprintf(buf, "t = %ld \r\n", val);
            Serial.print(buf);
            // Serial.write(val);
            ++ runRF;
            break;

        case 6:
            digitalWrite(RF_CTRL1, LOW);
            digitalWrite(TDC7200_EN, LOW);
            TIM22_CH2_PWM_stop();

            ++ runRF;
            break;

        // case 2:
        //     digitalWrite(TDC7200_CS, LOW);
        //     SPI.transfer(0x00);   // command
        //     readSPI = SPI.transfer(0x00);   // data
        //     digitalWrite(TDC7200_CS, HIGH);
        //     Serial.write(readSPI);
        //     ++ runRF;
        //     break;

        // case 3:
        //     digitalWrite(TDC7200_CS, LOW);
        //     SPI.transfer16(0x4019);
        //     digitalWrite(TDC7200_CS, HIGH);
        //     ++ runRF;
        //     break;

        // case 4:
        //     digitalWrite(TDC7200_CS, LOW);
        //     SPI.transfer(0x00);   // command
        //     readSPI = SPI.transfer(0x00);   // data
        //     digitalWrite(TDC7200_CS, HIGH);
        //     Serial.write(readSPI);
        //     ++ runRF;
        //     break;

        // case 5:
        //     digitalWrite(TDC7200_CS, LOW);
        //     SPI.transfer(0x01);   // command
        //     readSPI = SPI.transfer(0x00);   // data
        //     digitalWrite(TDC7200_CS, HIGH);
        //     Serial.write(readSPI);
        //     ++ runRF;
        //     break;

        // case 6:
        //     digitalWrite(TDC7200_CS, LOW);
        //     SPI.transfer16(0x41C0);
        //     digitalWrite(TDC7200_CS, HIGH);
        //     ++ runRF;
        //     break;

        // case 7:
        //     digitalWrite(TDC7200_CS, LOW);
        //     SPI.transfer(0x01);   // command
        //     readSPI = SPI.transfer(0x00);   // data
        //     digitalWrite(TDC7200_CS, HIGH);
        //     Serial.write(readSPI);
        //     ++ runRF;
        //     break;

        // case 8:
        //     // currentMillis = micros();
        //     // if (currentMillis - previousMillis >= 20) {
        //         digitalWrite(TDC7200_START, HIGH);
        //         digitalWrite(TDC7200_START, LOW);
        //         ++ runRF;
        //     // }
        //     break;

        // case 9:
        //     digitalWrite(TDC7200_CS, LOW);
        //     SPI.transfer(0x10);
        //     val = SPI.transfer(0x00);   // data
        //     val <<= 8;
        //     val |= SPI.transfer(0x00);   // data
        //     val <<= 8;
        //     val |= SPI.transfer(0x00);   // data
        //     digitalWrite(TDC7200_CS, HIGH);
        //     // Serial.write(readSPI1);
        //     // Serial.write(readSPI2);
        //     // Serial.write(readSPI3);
        //     sprintf(buf, "t = %ld \r\n", val);
        //     Serial.print(buf);
        //     // Serial.write(val);
        //     ++ runRF;
        //     break;

        // case 10:
        //     digitalWrite(RF_CTRL1, LOW);
        //     digitalWrite(TDC7200_EN, LOW);
        //     TIM22_CH2_PWM_stop();

        //     ++ runRF;
        //     break;

        // case 1: case 2: case 3:
        //     if(runRF == 1)  digitalWrite(RF_CTRL1, HIGH);
        //     else if(runRF == 2) digitalWrite(RF_CTRL2, HIGH);
        //     else if(runRF == 3) digitalWrite(RF_CTRL3, HIGH);

        //     previousMillis = micros();
        //     Serial.println(runRF);
        //     runRF = runRF+10;
        //     break;

        // case 11: case 12: case 13:
        //     currentMillis = micros();
        //     if (currentMillis - previousMillis >= 1293) {
        //         digitalWrite(TDC7200_START, HIGH);
        //         digitalWrite(TDC7200_START, LOW);
        //         runRF = runRF+10;
        //     }
        //     break;

        // case 21: case 22: case 23:
        //     currentMillis = micros();
        //     if (currentMillis - previousMillis >= 1300) {
        //         if(runRF == 21)  digitalWrite(RF_CTRL1, LOW);
        //         else if(runRF == 22) digitalWrite(RF_CTRL2, LOW);
        //         else if(runRF == 23) digitalWrite(RF_CTRL3, LOW);
        //         Serial.println(runRF);
        //         runRF = 0;
        //     }
        //     break;

        default: break;
    }
}
#endif /* TOFtest END */


#define timerPWMtest 0
#if timerPWMtest

//#include "wiring_private.h"

void TIM22_CH2_PWM_setup (void)
{
    analogWrite(2, 1);  // D2, PB5, TIM22_CH2

    RCC->APB2ENR |= (0x1 << 5);

    TIM22->CR1 = 0;
    /* Set the Autoreload value */
    TIM22->ARR = 1;
    /* Set the Prescaler value */
    TIM22->PSC = 1;
    /* Generate an update event to reload the Prescaler
    and the repetition counter (only for advanced timer) value immediately */
    TIM22->EGR = 0;

    //sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    TIM22->SMCR = 0;

    /* Write to TIMx CR2 */
    TIM22->CR2 = 0;

    /* Write to TIMx CCMR1 */
    TIM22->CCMR1 = 0x0006C00;

    /* Set the Capture Compare Register value */
    TIM22->CCR2 = 1;

    /* Write to TIMx CCER */
    TIM22->CCER = 0;
}

void TIM22_CH2_PWM_start (void)
{
    TIM22->CR1 = 1;
    TIM22->SR = 7;
    TIM22->CCER = 0x10;
}


void TIM22_CH2_PWM_stop (void)
{
    TIM22->CR1 = 0;
    TIM22->CCER = 0x00;
}


void setup ()
{
    Serial.begin(9600);
    TIM22_CH2_PWM_setup();
}


void loop ()
{
    if(Serial.available())
    {
        char rData = Serial.read();
        switch (rData)
        {
            case '1':
                Serial.println("1");
                TIM22_CH2_PWM_start();
                break;

            case '2':
                Serial.println("2");
                TIM22_CH2_PWM_stop();
                break;

            default: break;
        }
    }
}

#endif




#define CodeForTheECmeter 0
#if CodeForTheECmeter
/*
  ElCheapo Arduino EC-PPM measurments

  This scrip uses a common USA two prong plug and a 47Kohm Resistor to measure the EC/PPM of a Aquaponics/Hydroponics Sytem.
  You could modift this code to Measure other liquids if you change the resitor and values at the top of the code.

  This Program will give you a temperature based feed controller. See Read me in download file for more info.

  28/8/2015  Michael Ratcliffe  Mike@MichaelRatcliffe.com


          This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.


    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.


    You should have received a copy of the GNU General Public License
    along with this program.  If not, see .

    Parts:
    -Arduino - Uno/Mega
    -Standard American two prong plug
    -1 kohm resistor
    -DS18B20 Waterproof Temperature Sensor

    Limitations:
    -
    -

    See www.MichaelRatcliffe.com/Projects for a Pinout and user guide or consult the Zip you got this code from

*/

#include <OneWire.h>
#include <DallasTemperature.h>

//************************* User Defined Variables ********************************************************//

int R1= 1000;
int Ra=25;          //Resistance of powering Pins
int ECPin= A0;
int ECGround=A1;
int ECPower =A4;

//*********** Converting to ppm [Learn to use EC it is much better**************//
// Hana      [USA]        PPMconverion:   0.5
// Eutech    [EU]         PPMconversion:  0.64
// Tranchen  [Australia]  PPMconversion:  0.7
// Why didnt anyone standardise this?

float PPMconversion=0.5;

//*************Compensating for temperature ************************************//
// The value below will change depending on what chemical solution we are measuring
// 0.019 is generaly considered the standard for plant nutrients [google "Temperature compensation EC" for more info
float TemperatureCoef = 0.019; //this changes depending on what chemical we are measuring

//********************** Cell Constant For Ec Measurements *********************//
// Mine was around 2.9 with plugs being a standard size they should all be around the same
// But If you get bad readings you can use the calibration script and fluid to get a better estimate for K
float K=2.34;

//************ Temp Probe Related *********************************************//
#define ONE_WIRE_BUS 10          // Data wire For Temp Probe is plugged into pin 10 on the Arduino
const int TempProbePossitive =8;  //Temp Probe power connected to pin 9
const int TempProbeNegative=9;    //Temp Probe Negative connected to pin 8

//***************************** END Of Recomended User Inputs *****************************************************************//


OneWire oneWire(ONE_WIRE_BUS);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.

float Temperature=10;
float EC=0;
float EC25 =0;
int   ppm =0;

float raw= 0;
float Vin= 5;
float Vdrop= 0;
float Rc= 0;
float buffer=0;

//*********************************Setup - runs Once and sets pins etc ******************************************************//
void setup()
{
    Serial.begin(9600);
    pinMode(TempProbeNegative , OUTPUT);  //seting ground pin as output for tmp probe
    digitalWrite(TempProbeNegative , LOW); //Seting it to ground so it can sink current
    pinMode(TempProbePossitive , OUTPUT); //ditto but for positive
    digitalWrite(TempProbePossitive , HIGH);
    pinMode(ECPin,INPUT);
    pinMode(ECPower,OUTPUT);//Setting pin for sourcing current
    pinMode(ECGround,OUTPUT);//setting pin for sinking current
    digitalWrite(ECGround,LOW);//We can leave the ground connected permanantly

    delay(100);// gives sensor time to settle
    sensors.begin();
    delay(100);
    //** Adding Digital Pin Resistance to [25 ohm] to the static Resistor *********//
    // Consule Read-Me for Why, or just accept it as true
    R1=(R1+Ra);// Taking into acount Powering Pin Resitance

    Serial.println("ElCheapo Arduino EC-PPM measurments");
    Serial.println("By: Michael Ratcliffe  Mike@MichaelRatcliffe.com");
    Serial.println("Free software: you can redistribute it and/or modify it under GNU ");
    Serial.println("");
    Serial.println("Make sure Probe and Temp Sensor are in Solution and solution is well mixed");
    Serial.println("");
    Serial.println("Measurements at 5's Second intervals [Dont read Ec morre than once every 5 seconds]:");
}

void loop()
{
    GetEC();          //Calls Code to Go into GetEC() Loop [Below Main Loop] dont call this more that 1/5 hhz [once every five seconds] or you will polarise the water
    PrintReadings();  // Cals Print routine [below main loop]
    delay(5000);
}

void GetEC()
{
    sensors.requestTemperatures();          // Send the command to get temperatures
    Temperature=sensors.getTempCByIndex(0); //Stores Value in Variable
    //************Estimates Resistance of Liquid ****************//
    digitalWrite(ECPower,HIGH);
    raw= analogRead(ECPin);
    raw= analogRead(ECPin);// This is not a mistake, First reading will be low beause if charged a capacitor
    digitalWrite(ECPower,LOW);

    //***************** Converts to EC **************************//
    Vdrop= (Vin*raw) / 1024.0;
    Rc = (Vdrop*R1) / (Vin-Vdrop);
    Rc = Rc-Ra; //acounting for Digital Pin Resitance
    EC = 1000/ (Rc*K);

    //*************Compensating For Temperaure********************//
    EC25  =  EC / (1+ TemperatureCoef*(Temperature-25.0));
    ppm=(EC25)*(PPMconversion*1000);
}


//***This Loop Is called From Main Loop- Prints to serial usefull info ***//
void PrintReadings()
{
    Serial.print("Rc: ");
    Serial.print(Rc);
    Serial.print(" EC: ");
    Serial.print(EC25);
    Serial.print(" Simens  ");
    Serial.print(ppm);
    Serial.print(" ppm  ");
    Serial.print(Temperature);
    Serial.println(" *C ");
};
#endif


#define CodeForCalibration 0
#if CodeForCalibration
/*
  ElCheapo Arduino EC-PPM measurments Calibration

  This Script is used for calibration of the sensor and fine tuning of the Cell Constant K
  Submerge the sensor and temperature probe in the calibration solution and leave for a while so the temperature probe can settle
  Change the value of the calibration solution to suit the solutiton strength
  Stir the probe to make sure the solution is well mixed and upload the code to the arduino
  Open the terminal for an update of the estimated Cell Constant K [should be around 3] and use this new value in the main EC code.


  28/8/2015  Michael Ratcliffe  Mike@MichaelRatcliffe.com


          This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.


    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.


    You should have received a copy of the GNU General Public License
    along with this program.  If not, see .

    Parts:
    -Arduino - Uno/Mega
    -Standard American two prong plug
    -1 kohm resistor
    -DS18B20 Waterproof Temperature Sensor


    See www.MichaelRatcliffe.com/Projects for a Pinout and user guide or consult the Zip you got this code from

*/


//************************** Libraries Needed To Compile The Script [See Read me In Download] ***************//
// Both below Library are custom ones [ SEE READ ME In Downloaded Zip If You Dont Know how To install Use them or add a pull up resistor to the temp probe

#include <OneWire.h>
#include <DallasTemperature.h>

//************************* User Defined Variables ********************************************************//

float CalibrationEC = 0.762; //EC value of Calibration solution is s/cm

//##################################################################################
//-----------  Do not Replace R1 with a resistor lower than 300 ohms    ------------
//##################################################################################
int R1 = 1000;
int Ra = 25; //Resistance of powering Pins
int ECPin = A0;
int ECGround = A1;
int ECPower = A4;

//*************Compensating for temperature ************************************//
//The value below will change depending on what chemical solution we are measuring
//0.019 is generaly considered the standard for plant nutrients [google "Temperature compensation EC" for more info
float TemperatureCoef = 0.019; //this changes depending on what chemical we are measuring

//************ Temp Probe Related *********************************************//
#define ONE_WIRE_BUS 10          // Data wire For Temp Probe is plugged into pin 10 on the Arduino
const int TempProbePossitive = 8; //Temp Probe power connected to pin 9
const int TempProbeNegative = 9;  //Temp Probe Negative connected to pin 8

//***************************** END Of Recomended User Inputs *****************************************************************//


OneWire oneWire(ONE_WIRE_BUS);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.


float TemperatureFinish = 0;
float TemperatureStart = 0;
float EC = 0;
int ppm = 0;

float raw = 0;
float Vin = 5;
float Vdrop = 0;
float Rc = 0;
float K = 0;
int i = 0;
float buffer = 0;

//*********************************Setup - runs Once and sets pins etc ******************************************************//
void setup()
{
    Serial.begin(9600);
    pinMode(TempProbeNegative , OUTPUT);  //seting ground pin as output for tmp probe
    digitalWrite(TempProbeNegative , LOW); //Seting it to ground so it can sink current
    pinMode(TempProbePossitive , OUTPUT); //ditto but for positive
    digitalWrite(TempProbePossitive , HIGH);
    pinMode(ECPin, INPUT);
    pinMode(ECPower, OUTPUT); //Setting pin for sourcing current
    pinMode(ECGround, OUTPUT); //setting pin for sinking current
    digitalWrite(ECGround, LOW); //We can leave the ground connected permanantly

    delay(100);// gives sensor time to settle
    sensors.begin();
    delay(100);
    //** Adding Digital Pin Resistance to [25 ohm] to the static Resistor *********//
    // Consule Read-Me for Why, or just accept it as true
    R1 = (R1 + Ra);

    Serial.println("ElCheapo Arduino EC-PPM measurments Calibration");
    Serial.println("By: Michael Ratcliffe  Mike@MichaelRatcliffe.com");
    Serial.println("Free software: you can redistribute it and/or modify it under GNU ");
    Serial.println("");
    Serial.println("Make sure Probe and Temp Sensor are in Solution and solution is well mixed");
    Serial.println("");
    Serial.println("Starting Calibration: Estimated Time 60 Seconds:");
};
//******************************************* End of Setup     
//************************************* Main Loop - Runs Forever ***************************************************************//
//Moved Heavy Work To subroutines so you can call them from main loop without cluttering the main loop
void loop()
{
    i = 1;
    buffer = 0;
    sensors.requestTemperatures();// Send the command to get temperatures
    TemperatureStart = sensors.getTempCByIndex(0); //Stores Value in Variable

    //************Estimates Resistance of Liquid ****************//
    while(i <= 10) {
        digitalWrite(ECPower, HIGH);
        raw = analogRead(ECPin);
        raw = analogRead(ECPin); // This is not a mistake, First reading will be low
        digitalWrite(ECPower, LOW);
        buffer = buffer + raw;
        i++;
        delay(5000);
    };

    raw = (buffer / 10);

    sensors.requestTemperatures();// Send the command to get temperatures

    TemperatureFinish = sensors.getTempCByIndex(0); //Stores Value in Variable

    //*************Compensating For Temperaure********************//
    EC = CalibrationEC * (1 + (TemperatureCoef * (TemperatureFinish - 25.0))) ;

    //***************** Calculates R relating to Calibration fluid **************************//
    Vdrop = (((Vin) * (raw)) / 1024.0);

    Rc = (Vdrop * R1) / (Vin - Vdrop);

    Rc = Rc - Ra;

    K = 1000 / (Rc * EC);

    Serial.print("Calibration Fluid EC: ");

    Serial.print(CalibrationEC);

    Serial.print(" S  ");  //add units here

    Serial.print("Cell Constant K");

    Serial.print(K);

    if(TemperatureStart == TemperatureFinish) {
        Serial.println("  Results are Trustworthy");
        Serial.println("  Safe To Use Above Cell Constant in Main EC code");

    } else {
        Serial.println("  Error -Wait For Temperature To settle");
        Serial.print("Temp start=");
        Serial.print(TemperatureStart);
        Serial.print(" Temp end=");
        Serial.println(TemperatureFinish);
    }
}
//*************************** End Of Main Loop *******************************************************************//
#endif
