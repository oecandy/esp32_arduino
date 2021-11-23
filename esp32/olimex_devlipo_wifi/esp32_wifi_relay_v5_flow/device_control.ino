void controll_valve(int switchNo, int onOff){
  switch(switchNo){
    case 1 :
      digitalWrite(RELAY_1_PIN,onOff);
      break;
    case 2 :
      digitalWrite(RELAY_2_PIN,onOff);
      break;
    case 3 :
      digitalWrite(RELAY_3_PIN,onOff);
      break;
    case 4 :
      digitalWrite(RELAY_4_PIN,onOff);
      break;
    case 5 :
      digitalWrite(RELAY_5_PIN,onOff);
      break;
    case 6 :
      digitalWrite(RELAY_6_PIN,onOff);
      break;
  }
}
