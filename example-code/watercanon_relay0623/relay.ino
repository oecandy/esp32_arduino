void pin_setup(){
  pinMode(RELAY_FOG_ON,OUTPUT);
  pinMode(RELAY_FOG_OFF,OUTPUT);
  pinMode(RELAY_SWING_ON,OUTPUT);
  pinMode(RELAY_SWING_OFF,OUTPUT);
  pinMode(RELAY_LIFTING_ON,OUTPUT);
  pinMode(RELAY_LIFTING_OFF,OUTPUT);
}

void toggle(int RELAY){
  digitalWrite(RELAY, LOW);
  delay(1000);
  digitalWrite(RELAY, HIGH);
}

void controll_relay(int cmd_no){  // which_cmd = cmd_no MQTT쪽이랑 맞춰야 함
  switch (cmd_no) {
      case 1:
        toggle(RELAY_FOG_ON);
        break;
      case 2:
        toggle(RELAY_FOG_OFF);
        break;
      case 3:
        toggle(RELAY_SWING_ON);
        break;
      case 4:
        toggle(RELAY_SWING_OFF);
        break;
      case 5:
        toggle(RELAY_LIFTING_ON);
        break;
      case 6:
        toggle(RELAY_LIFTING_OFF);
        break;
      default:
        break;
    }
}
