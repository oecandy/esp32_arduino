void closeValve() {
  digitalWrite(VALVE_OPEN_1_1, HIGH); // 솔레노이드 열기
  digitalWrite(VALVE_OPEN_1_2, LOW);
  delay(3000);
  digitalWrite(VALVE_OPEN_1_1, LOW);
  digitalWrite(VALVE_OPEN_1_2, LOW);
}

void openValve() {
  digitalWrite(VALVE_OPEN_1_1, LOW); // 솔레노이드 닫기
  digitalWrite(VALVE_OPEN_1_2, HIGH);
  delay(3000);
  digitalWrite(VALVE_OPEN_1_1, LOW);
  digitalWrite(VALVE_OPEN_1_2, LOW);
}
