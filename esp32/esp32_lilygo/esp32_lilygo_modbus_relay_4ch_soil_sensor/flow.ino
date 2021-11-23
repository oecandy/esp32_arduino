void ICACHE_RAM_ATTR getFlow();
// flow
void getFlow1 ()
{
    ++ flowCount1;    //  low가 인식, high가 미인식
}

void getFlow2 ()
{
    ++ flowCount2;    //  low가 인식, high가 미인식
}

void getFlow3 ()
{
    ++ flowCount3;    //  low가 인식, high가 미인식
}

void getFlow4 ()
{
    ++ flowCount4;    //  low가 인식, high가 미인식
}

void flow_loop(){
  unsigned long currentMillis = millis();

  // 6초마다 count 횟수
  if (currentMillis - flowMillis >= INTERVAL_TIME) {
      flowMillis = currentMillis;
      mqtt_publish_data();
      send_sensor_data();
      flowCount1 = 0;
      flowCount2 = 0;
      flowCount3 = 0;
      flowCount4 = 0;
  }
}


void flow_setup(){
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_1_PIN), getFlow1, FALLING);
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_2_PIN), getFlow2, FALLING);
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_3_PIN), getFlow3, FALLING);
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_4_PIN), getFlow4, FALLING);
}
