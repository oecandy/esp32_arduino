void closeValve(int valveNo) {
  switch(valveNo){
    case 1:
      digitalWrite(VALVE_1_PIN, LOW); // 솔레노이드 열기
      valveCommand[0] = 0;
      break;
    case 2:
      digitalWrite(VALVE_2_PIN, LOW); // 솔레노이드 열기
      valveCommand[1] = 0;
      break;
  }
}

void openValve(int valveNo) {
  switch(valveNo){
    case 1:
      digitalWrite(VALVE_1_PIN, HIGH); // 솔레노이드 열기
      valveCommand[0] = 1;
      break;
    case 2:
      digitalWrite(VALVE_2_PIN, HIGH); // 솔레노이드 열기
      valveCommand[1] = 1;
      break;
  }
}
