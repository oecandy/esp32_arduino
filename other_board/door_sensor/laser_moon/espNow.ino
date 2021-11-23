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
    esp_now_register_recv_cb(OnDataRecv);
    esp_now_register_send_cb(OnDataSent);
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
    deviceData.device_no = DEVICE_NO;
    deviceData.sensor_type = SENSOR_TYPE;
    deviceData.sensor_data = sensorData;
    deviceData.sensor_state = sensorState;
    deviceData.relay_state[0] = relayStatus[0];
    deviceData.relay_state[1] = relayStatus[1];
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send (broadcastAddress, (uint8_t *) &deviceData, sizeof(deviceData));

    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
        Serial.println(result);
    }
}
