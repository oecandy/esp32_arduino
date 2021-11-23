void ICACHE_RAM_ATTR getFlow();

void getFlow1 ()
{
    ++ flowCount1;    //  low가 인식, high가 미인식
}

void getFlow2 ()
{
    ++ flowCount2;    //  low가 인식, high가 미인식
}

void flow_loop(){
  unsigned long currentMillis = millis();

  // 6초마다 count 횟수
  if (currentMillis - flowMillis >= INTERVAL_TIME + timeFlag) {
      flowMillis = currentMillis;
      sendMessage();
      flowCount1 = 0;
      flowCount2 = 0;
      timeFlag = getTimeFlag();
  }
}


void flow_setup(){
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_1_PIN), getFlow1, FALLING);
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_2_PIN), getFlow2, FALLING);
}

void valveControll(){
  if(relayStatusChange){
    switch(switchNo){
      case 1 :
        digitalWrite(RELAY_1_PIN, relayStatus[0]);
        break;
      case 2 :
        digitalWrite(RELAY_2_PIN, relayStatus[1]);
        break;
    }
    relayStatusChange = false;
  }
}

int getTimeFlag(){
  return random(1000, 3000);
}

void reset_loop(){
  unsigned long currentMillis = millis();
  if(resetFlag){
    if (currentMillis - resetMillis >= RESET_TIME) {
      int resultStatus = 0;
      for(int i = 0; i < CHANNEL; i ++){
        resultStatus += relayStatus[i];
      }
      if(resultStatus == 0){
        ESP.restart(); 
      } else {
        resetMillis = currentMillis;
      }
    }
  } else {
    resetMillis = currentMillis;
    resetFlag = 1;
  }
}
