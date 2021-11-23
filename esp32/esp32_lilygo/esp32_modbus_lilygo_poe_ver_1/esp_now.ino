void espNowInit() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
    
  } 
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
//  esp_now_register_recv_cb(OnDataRecv);
}

//// callback function that will be executed when data is received 수정 필요 structure를 그냥 던지게 혹은 payload로 변환
//void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
//  memcpy(&myData, incomingData, sizeof(myData));
//  sendPayload();
//  Serial.println("Payload sent...");
//  serialPrint();
///*
//  mqttClient.publish (outTopic, message_buff);
//  mqttClient.subscribe(inTopic);*/
//}
