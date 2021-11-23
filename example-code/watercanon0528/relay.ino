void pin_setup(){
  pinMode(RELAY_FOG_ON,OUTPUT);
  pinMode(RELAY_FOG_OFF,OUTPUT);
  pinMode(RELAY_SWING_ON,OUTPUT);
  pinMode(RELAY_SWING_OFF,OUTPUT);
  pinMode(RELAY_LIFTING_ON,OUTPUT);
  pinMode(RELAY_LIFTING_OFF,OUTPUT);
}



void control_relay(int cmd_no){  // which_cmd = cmd_no MQTT쪽이랑 맞춰야 함
  if(relayStatusChange){
    unsigned long currentMillis = millis();
    switch(control_step){
      case 1:
        Serial.println(control_step);
        Serial.println(cmd_no);
        execute_relay(cmd_no, true);
        controlMillis = currentMillis;
        control_step++;
        break;
      case 2:
        if(controlMillis + 1000 < currentMillis){
          Serial.println(control_step);
          Serial.println(cmd_no);
          execute_relay(cmd_no, false);
          control_step++; 
        }
        break;
      case 3:
        Serial.println(control_step);
        Serial.println(cmd_no);
        relayStatusChange = false;
        control_step = 0;
        break;
    }
  }
  
}

void execute_relay(int cmd_no, bool onOff){
  switch (cmd_no) {
      case 1:
        if(onOff){
          digitalWrite(RELAY_FOG_ON, HIGH);
        } else {
          digitalWrite(RELAY_FOG_ON, LOW);
        }
        break;
      case 2:
        if(onOff){
          digitalWrite(RELAY_FOG_OFF, HIGH);
        } else {
          digitalWrite(RELAY_FOG_OFF, LOW);
        }
        break;
      case 3:
        if(onOff){
          digitalWrite(RELAY_SWING_ON, HIGH);
        } else {
          digitalWrite(RELAY_SWING_ON, LOW);
        }
        break;
      case 4:
        if(onOff){
          digitalWrite(RELAY_SWING_OFF, HIGH);
        } else {
          digitalWrite(RELAY_SWING_OFF, LOW);
        }
        break;
      case 5:
        if(onOff){
          digitalWrite(RELAY_LIFTING_ON, HIGH);
        } else {
          digitalWrite(RELAY_LIFTING_ON, LOW);
        }
        break;
      case 6:
        if(onOff){
          digitalWrite(RELAY_LIFTING_OFF, HIGH);
        } else {
          digitalWrite(RELAY_LIFTING_OFF, LOW);
        }
        break;
      default:
        break;
    }
}
