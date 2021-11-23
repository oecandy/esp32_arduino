void closeValve() {
  digitalWrite(VALVE_1_PIN, LOW); // 솔레노이드 열기
}

void openValve() {
  digitalWrite(VALVE_1_PIN, HIGH); // 솔레노이드 열기
}
