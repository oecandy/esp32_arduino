// callback when data is recv from Master 마스터에서 데이터 받으면 콜백
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
//  [타이머 변수 수정하는 데이터인지 체크]
//  [타이머 데이터이면 받은 값으로 타이머 변수 덮어쓰기]
  noInterrupts();
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  memcpy(&recvData, data, 64);

  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.print("Last Packet Recv Data: "); Serial.println(*data);

  if(recvData.payload[0] == 1){
    TIME_TO_SLEEP = (int) recvData.payload[1];
    gain = (int) recvData.payload[2];
    mac1 = mac_addr[0];
    mac2 = mac_addr[1];
    mac3 = mac_addr[2];
    mac4 = mac_addr[3];
    mac5 = mac_addr[4];
    mac6 = mac_addr[5];
    
    switchAP = 0;
  }
  
  
  interrupts();
  esp_sleep_enable_timer_wakeup(1 * uS_TO_S_FACTOR);  // set wakeup timing
  esp_deep_sleep_start(); // he's sleep until uS_TO_S_FACTOR comes
  
}

// callback when data is sent
void OnDataSent (const uint8_t * mac_addr, esp_now_send_status_t status) {
    noInterrupts();
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    interrupts();
}
