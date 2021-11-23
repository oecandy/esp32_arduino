/* Si7021-A20 I2C Humidity and Temperature Sensor ---------------------------------------------------------------------------- */
void I2C_setup (void)
{
    pinMode(Vext, OUTPUT);
}

void ht_sequence (void)
{
    // Vext ON
    digitalWrite(Vext, LOW);
    Wire.begin();
    delay(50);

    if (!sensor.begin())
    {
        Serial.println("Did not find Si7021 sensor!");
        Wire.end();
        digitalWrite(Vext, HIGH);
        delay(1000);
        return;
    }

    HM.float_val = sensor.readHumidity();
    TP.float_val = sensor.readTemperature();
    Wire.end();

#if CHECK_VALUE
    Serial.print("Humidity:    ");
    Serial.print(HM.float_val, 3);
    Serial.print("\tTemperature: ");
    Serial.println(TP.float_val, 3);
#endif

    // Vext OFF
    digitalWrite(Vext, HIGH);
}
/* --------------------------------------------------------------------------------------------------------------------------- */

/* Read ADC Voltage ---------------------------------------------------------------------------------------------------------- */
void check_battery_voltage (void)
{
  char i;
  word adcBT[6] = { 0, 0, 0, 0, 0, 0 };
  uint32_t sumBT = 0;
  float aveBT = 0;

  for(i=0; i<6; ++i)
  {
    adcBT[i] = analogRead(ADC_BATTERY);    // 배터리 전압 ADC로 읽어오기 (mV)
    delay(1);               // 1ms 대기
  }

  for(i=1; i<6; ++i)
    {
        sumBT = sumBT + (uint32_t)adcBT[i];   // 5번 읽은 전압 합
    #if CHECK_VALUE
        Serial.print("adc = ");
        Serial.print(adcBT[i]);
        Serial.print(", sum = ");
        Serial.println(sumBT);
    #endif
    }
    
  aveBT = (float)sumBT / 5;             // 평균내기

#if DEBUG_MODE
    Serial.print("aveBT = ");
    Serial.println(adcBT);
#endif

    // 전압 분배로 인하여 약 0.62921배 낮아진 값을 ADC로 읽어옴
    // 실제 전압은 읽어온 ADC 값에 낮아진 비율만큼 나눠서 계산해야 함
    // 읽어온 ADC 전압 값을 실제 전압으로 변환
    BT.float_val = aveBT / 0.62921;     // mV

#if CHECK_VALUE
    Serial.print("BTLV = ");
    Serial.println(BT.float_val, 2);
#endif

    // HM4100B 기준 0~4bar
#if DEBUG_MODE
    Serial.print("btLV = ");
    Serial.println(batteryLevel);
#endif
}

void check_pressure_voltage (void)
{
  char i;
  word adcWP[6] = { 0, 0, 0, 0, 0, 0 };
  uint32_t sumWP = 0;
  float aveWP = 0;

    digitalWrite(PWR_5VDC_EN, HIGH);        // pressure voltage ON;
    Wire.begin();
    delay(2);                               // waiting pressure turn on

  for(i=0; i<6; ++i)
  {
    adcWP[i] = analogRead(ADC_PRESSURE);    // 배터리 전압 ADC로 읽어오기
    delay(1);               // 1ms 대기
  }

  for(i=1; i<6; ++i)
    sumWP = sumWP + (uint32_t)adcWP[i];   // 5번 읽은 전압 합
  aveWP = (float)sumWP / 5;             // 평균내기

#if DEBUG_MODE
    Serial.print("aveBT = ");
    Serial.println(adcBT);
#endif

    // 전압 분배로 인하여 약 0.47663배 낮아진 값을 ADC로 읽어옴
    // 실제 전압은 읽어온 ADC 값에 낮아진 비율만큼 나눠서 계산해야 함
    // 읽어온 ADC 전압 값을 실제 전압으로 변환
  WP.float_val = aveWP / 0.47663;     // mV
  Wire.end();

    digitalWrite(PWR_5VDC_EN, LOW);        // pressure voltage OFF;

#if CHECK_VALUE
    Serial.print("WPLV = ");
    Serial.println(WP.float_val, 2);
#endif
    // 압력계 정해지면 압력계 값으로 변경하기
#if DEBUG_MODE
    Serial.print("WPLV = ");
    Serial.println(pressureLevel);
#endif
}


void adc_sequence (unsigned long readTime)
{
    switch(adcStep)
  {
    case 1:
            check_battery_voltage();        // 배터리 전압 읽어서 저장
      ++ adcStep;
      break;

    case 2:
      check_pressure_voltage();       // 압력계 전압 읽어서 저장
      adcStep = 0;
      break;

    default: break;
  }
}

void start_adc_step (void)
{
  adcStep = 1;
}
/* --------------------------------------------------------------------------------------------------------------------------- */
