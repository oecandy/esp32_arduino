void ICACHE_RAM_ATTR getFlow();
// flow
void getFlow ()
{
    ++ flowCount;    //  low가 인식, high가 미인식
}

void flow_loop(){
  unsigned long currentMillis = millis();

  // 6초마다 count 횟수
  if (currentMillis - flowMillis >= INTERVAL_TIME) {
      flowMillis = currentMillis;
      mqtt_publish_data();
      flowCount = 0;
  }
}


void flow_setup(){
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_PIN), getFlow, FALLING);
}
