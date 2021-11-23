// callback when data is sent
void OnDataSent (const uint8_t * mac_addr, esp_now_send_status_t status) {
    noInterrupts();
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    interrupts();
}

void espNowInit () {
    noInterrupts();
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);

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

    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }
    
}
