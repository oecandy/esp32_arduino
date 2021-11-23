void interval_loop(){
  unsigned long currentMillis = millis();

  // 6초마다 count 횟수
  if (currentMillis - intervalMillis >= INTERVAL_TIME + timeFlag) {
      intervalMillis = currentMillis;
      sendMessage();
      timeFlag = getTimeFlag();
  }
}


//void test_swing_interval_loop(){
//  unsigned long currentMillis = millis();
//
//  // 6초마다 count 횟수
//  if (currentMillis - testMillis >= TEST_TIME) {
//      testMillis = currentMillis;
//      if(test_signal){
//        relayStatus[3] = 1;
//        loop_finish = 0;
//        Serial.println("test start");
//        test_signal = 0;
//      } else {
//        relayStatus[3] = 0;
//        loop_finish = 1;
//        Serial.println("test stop");
//        test_signal = 1;
//      }
//  }
//}


void valveControll(){
  if(relayStatusChange){
    switch(switchNo){
      case 1 :
        mySwitch.send(code_switch_pan_on_off, 24);
        Serial.println("1 toggle");
        break;
      case 2 :
        mySwitch.send(code_switch_pump_on_off, 24);
        Serial.println("2 toggle");
        break;
      case 3 :
        mySwitch.send(code_switch_lifting_on_off, 24);
        Serial.println("3 toggle");
        break;
      case 4 :
//        mySwitch.send(code_switch_right_on_off, 24);
        
        if(relayStatus[3]){
          loop_finish = 0;
        } else {
          loop_finish = 1;
        }
        Serial.println("4 toggle");
        break;
      case 5 :
//        mySwitch.send(code_switch_left_on_off, 24);
        Serial.println("5 toggle");
        break;
      case 6 :
        mySwitch.send(code_switch_6, 24);
        Serial.println("6 toggle");
        break;
    }
    relayStatusChange = false;
  }
}

void swing_loop(){
  if(relayStatus[3] || (!relayStatus[3] && loop_finish)){
    unsigned long currentMillis = millis();
    if (currentMillis - swingMillis >= SWING_INTERVAL || (turnFlag && currentMillis - swingMillis >= SWING_TURN)) {
      swingMillis = currentMillis;
      switch(swingFlag){
        case 1:
          mySwitch.send(code_switch_left_on_off, 24);
          Serial.println("left on");
          Serial.println(swingFlag);
          swingFlag++;
          turnFlag = 0;
          dir = 1;
          break;
        case 2:
          mySwitch.send(code_switch_left_on_off, 24);
          Serial.println("left off");
          Serial.println(swingFlag);
          swingFlag++;
          turnFlag = 1;
          dir = 0;
          break;
        case 3:
          mySwitch.send(code_switch_right_on_off, 24);
          Serial.println("right on");
          Serial.println(swingFlag);
          swingFlag++;
          turnFlag = 0;
          dir = 2;
          break;
        case 4:
          mySwitch.send(code_switch_right_on_off, 24);
          Serial.println("right off");
          Serial.println(swingFlag);
          swingFlag++;
          turnFlag = 1;
          dir = 0;
          if(loop_finish){
            loop_finish = 0;
            swingFlag = 1;
            turnFlag = 0;
          }
          break;
        case 5:
          mySwitch.send(code_switch_right_on_off, 24);
          Serial.println("right on");
          Serial.println(swingFlag);
          swingFlag++;
          turnFlag = 0;
          dir = 2;
          break;
        case 6:
          mySwitch.send(code_switch_right_on_off, 24);
          Serial.println("right off");
          Serial.println(swingFlag);
          swingFlag++;
          turnFlag = 1;
          dir = 0;
          break;
        case 7:
          mySwitch.send(code_switch_left_on_off, 24);
          Serial.println("left on");
          Serial.println(swingFlag);
          swingFlag++;
          turnFlag = 0;
          dir = 1;
          break;
        case 8:
          mySwitch.send(code_switch_left_on_off, 24);
          Serial.println("left off");
          Serial.println(swingFlag);
          swingFlag = 1;
          turnFlag = 1;
          dir = 0;
          if(loop_finish){
            loop_finish = 0;
            turnFlag = 0;
          }
          break;
      }
    }
  }
}

int getTimeFlag(){
  return random(1000, 3000);
}

void RcSensorSetup(){
  mySwitch.enableTransmit(RF_RX_PIN);
  mySwitch.setPulseLength(346);
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
