void ICACHE_RAM_ATTR getFlow();
// flow
void getFlow ()
{
    ++ flowCount;    //  low가 인식, high가 미인식
}

void interval_loop(){
  unsigned long currentMillis = millis();

  // 6초마다 count 횟수
  if (currentMillis - intervalMillis >= INTERVAL_TIME) {
      intervalMillis = currentMillis;
      send_message();
      flowCount = 0;
  }
}


void flow_setup(){
  attachInterrupt(digitalPinToInterrupt(FLOWMETER_PIN), getFlow, FALLING);
}
