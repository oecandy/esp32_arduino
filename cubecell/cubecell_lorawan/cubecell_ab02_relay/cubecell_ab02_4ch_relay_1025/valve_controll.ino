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
    case 3:
      digitalWrite(VALVE_3_PIN, LOW); // 솔레노이드 열기
      valveCommand[2] = 0;
      break;
    case 4:
      digitalWrite(VALVE_4_PIN, LOW); // 솔레노이드 열기
      valveCommand[3] = 0;
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
    case 3:
      digitalWrite(VALVE_3_PIN, HIGH); // 솔레노이드 열기
      valveCommand[2] = 1;
      break;
    case 4:
      digitalWrite(VALVE_4_PIN, HIGH); // 솔레노이드 열기
      valveCommand[3] = 1;
      break;
  }
}
