// callback when data is recv from Master
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // 1. 도어 상태를 물으면, ( 1 or 0 으로 답해준다. )
  // 2. 도어오픈 커맨드(Normally closed)를 보내면, doorOpen() 한 번 실행
  // 3. 경광등 온 커맨드 보내면 sirenOn() 한 번 실행
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.print("Last Packet Recv Data: "); Serial.println(*data);
  Serial.println("");

  memcpy(&recvData, data, sizeof(recvData));

  if(recvData.valve_no == DEVICE_NO){
    switch(recvData.switch_no){
      case 1 :
        if(recvData.relay_state){
          doorOpen();
          Serial.println("open door");
        } else {
          doorClose();
          Serial.println("close door");
        }
        break;
      case 2 :
        if(recvData.relay_state){
          sirenOn();
          Serial.println("open siren");
        } else {
          sirenOff();
          Serial.println("close siren");
        }
        break;
    }
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to:");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status: ");
  
  if (status == 0){
    Serial.println("Delivery success");
    timeFlag = 0;
  } else {
    timeFlag = 500;
    Serial.println("Delivery fail");
    Serial.println(status);
  }
}
