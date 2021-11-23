// config AP SSID : 시리얼 세팅, 프리픽스 Slave:로 시작해야 마스터 인식 테크와이드 시리얼 룰 고려해서 변경
void configDeviceAP() {
  String Prefix = "Slave:";   // FSSM-BL8V2(무선센서모듈 모델명)
  String Mac = WiFi.macAddress();
  String SSID = Prefix + Mac;
  String Password = "123456789";
  bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}
