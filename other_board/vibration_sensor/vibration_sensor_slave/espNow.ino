// Init ESP Now with fallback
void initESPNow() {
  
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
    esp_now_register_send_cb(OnDataSent); // [콜백 로직 검토 필요] 보내면
    
    //Register peer
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    
    //Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }
    esp_now_register_recv_cb(OnDataRecv); // [콜백 로직 검토 및 추가 필요] 받으면  callBack 탭 참조
    esp_now_register_send_cb(OnDataSent); // [콜백 로직 검토 필요] 보내면
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

void sendPayload (){
        
    sensorData.device_no = CHANNEL;
    sensorData.x_val = getMaxValue(x, COLLECT_COUNT);
    sensorData.y_val = getMaxValue(y, COLLECT_COUNT);
    sensorData.z_val = getMaxValue(z, COLLECT_COUNT);

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send (broadcastAddress, (uint8_t *) &sensorData, sizeof(sensorData));

    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
        Serial.println(result);
    }
}
