void setAPMode() {
    //Set device in AP mode to begin with : AP모드로 시작
    WiFi.mode(WIFI_AP);
    // configure device AP mode : AP모드 세팅
    configDeviceAP();
    // This is the mac address of the Slave in AP Mode : MAC 프린팅
    //Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
}

// Init ESP Now with fallback
void initESPNow() {
  WiFi.disconnect();
  
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

void espSetStaMode () {
    noInterrupts();
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register peer
    //esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer        
    if (esp_now_add_peer(& peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
}
    interrupts();
}

void sendPayload (){
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send (broadcastAddress, (uint8_t *) &payload, sizeof(payload));
    for (int i = 0; i < 32; i++){
    printf("%d, \n", payload[i]);
}

    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }
}

void sendAxisInfo (){
    esp_err_t result = esp_now_send (broadcastAddress, (uint8_t *) &axis, sizeof(axis));
//    for (int i = 0; i < 32; i++){
//    printf("%d, \n", axis[i]);
//}
    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }
    
}
