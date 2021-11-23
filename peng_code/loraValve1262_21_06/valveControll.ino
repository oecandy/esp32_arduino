void relayOn(){
  delay(100);
  digitalWrite(SEQUENCE_SWITCH,LOW);
  delay(100);
  digitalWrite(SEQUENCE_SWITCH,HIGH);
}

void relayOff(){
  delay(100);
  digitalWrite(SEQUENCE_SWITCH,LOW);
}

void relayGeneratorOn(){
  delay(100);
  digitalWrite(VALVE_PWR_ON,LOW);
  delay(100);
  digitalWrite(VALVE_PWR_ON,HIGH);
}

void relayGeneratorOff(){
  delay(100);
  digitalWrite(VALVE_PWR_ON,LOW);
}
