void pin_setup(){
  for(int i=0; i<RELAY_TYPE ; i++){
    pinMode(RELAY[i], OUTPUT);
    command_status[i] = 0;
  }
}

void controll_valve(int valv_no, int switch_stat){
  if(valv_no%2==0){
    if(switch_stat==0){
      digitalWrite(RELAY[valv_no], LOW);
    }else{
      digitalWrite(RELAY[valv_no], HIGH);
    }
  }else{
    if(switch_stat==0){
      digitalWrite(RELAY[valv_no], HIGH);
    }else{
      digitalWrite(RELAY[valv_no], LOW);
    }
  }
  //yield();
  delay(100);
}
