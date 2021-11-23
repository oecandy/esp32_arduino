void ICACHE_RAM_ATTR getFlow();

void getFlow ()
{
    ++ flowCount;    //  low가 인식, high가 미인식
}

void flow_loop(){
  unsigned long currentMillis = millis();

  // 6초마다 count 횟수
  if (currentMillis - flowMillis >= INTERVAL_TIME + timeFlag) {
      flowMillis = currentMillis;
      Serial.println("flowCount is: ");
      Serial.println(flowCount);
      sendMessage();
      flowCount = 0;
      timeFlag = getTimeFlag();
  }
}


void flow_setup(){
  attachInterrupt(digitalPinToInterrupt(FLOW_METER_PIN), getFlow, FALLING);
}

void valveControll(){
  if(relayStatusChange)
  { 
    if(relayStatus[0]){
      digitalWrite(RELAY_PIN, HIGH);
      
    } else if(relayStatus[0] == 0){
      digitalWrite(RELAY_PIN, LOW);
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
