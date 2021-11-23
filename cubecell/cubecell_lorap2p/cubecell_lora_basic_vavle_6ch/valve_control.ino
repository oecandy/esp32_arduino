void RcSensorSetup(){
  mySwitch.enableTransmit(RF_RX_PIN);
  mySwitch.setPulseLength(346);
}

void valveControl(){
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
//      mySwitch.send(code_switch_right_on_off, 24);
        
      if(relayStatus[3]){
        loop_finish = 0;
      } else {
        loop_finish = 1;
      }
      Serial.println("4 toggle");
      break;
    case 5 :
//      mySwitch.send(code_switch_left_on_off, 24);
      Serial.println("5 toggle");
      break;
    case 6 :
      mySwitch.send(code_switch_6, 24);
      Serial.println("6 toggle");
      break;
  }
  state=TX;
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
