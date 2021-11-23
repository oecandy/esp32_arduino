void intervalLoop(){
  unsigned long currentMillis = millis();

  // 6초마다 count 횟수
  if (currentMillis - intervalMillis >= INTERVAL_TIME + timeFlag) {
      intervalMillis = currentMillis;
      sendMessage();
      timeFlag = getTimeFlag();
  }
}

void recieveRcMessage(){
  if (mySwitch.available()) {
    if (mySwitch.getReceivedValue()==codeSwitchBuzzerOn || mySwitch.getReceivedValue()==codeMainSwitchBuzzerOn){
      relayStatus[0] = 1;
      switchControl(1);
    }
    Serial.println(mySwitch.getReceivedValue());
    mySwitch.resetAvailable();
  }
}

int getTimeFlag(){
  return random(1000, 3000);
}

void RcSensorSetup(){
  mySwitch.enableReceive(RF_RX_PIN);
}

void switchControl(int switchNo){
  if(switchNo == 1){
    digitalWrite(SWITCH_PIN, HIGH);
    controlStep = 1;
    controlMillis = millis();
  }
}

void controlLoop(){
  unsigned long currentMillis = millis();
  if(currentMillis - controlMillis >= CONTROL_TIME){
    controlStep = 0;
    digitalWrite(SWITCH_PIN, LOW);
    relayStatus[0] = 0;
  }
}

void resetLoop(){
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

void pinSetUp(){
  pinMode(SWITCH_PIN, OUTPUT);
}
