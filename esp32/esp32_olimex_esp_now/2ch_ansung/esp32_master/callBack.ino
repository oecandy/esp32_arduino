// callback function that will be executed when data is received 수정 필요 structure를 그냥 던지게 혹은 payload로 변환
void OnDataRecv (const uint8_t * mac, const uint8_t * incomingData, int len) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    
    memcpy(&myData, incomingData, sizeof(myData));

    if(myData.valve_no == 1){
      valve_status[0] = myData.relay_state[0];
      valve_status[1] = myData.relay_state[1];
      flow_status[0] = myData.flow_count[0];
      flow_status[1] = myData.flow_count[1];
    } else if(myData.valve_no == 2){
      valve_status[2] = myData.relay_state[0];
      valve_status[3] = myData.relay_state[1];
      flow_status[2] = myData.flow_count[0];
      flow_status[3] = myData.flow_count[1];
    }
}

// callback when data is sent from Master to Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: "); Serial.println(macStr);
  Serial.print("Last Packet Send Status: "); Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
