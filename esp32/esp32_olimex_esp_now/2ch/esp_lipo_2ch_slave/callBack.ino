void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  String callMac = macStr;
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           broadcastAddress[0], broadcastAddress[1], broadcastAddress[2], broadcastAddress[3], broadcastAddress[4], broadcastAddress[5]);
  String myMac = macStr;
  memcpy(&recvData, data, sizeof(recvData));
  Serial.println(callMac);
  if(callMac == myMac && recvData.valve_no == VALVE_NO){
    switchNo = recvData.switch_no;
    switch(switchNo){
      case 1 :
        relayStatus[0] = recvData.relay_state;
        break;
      case 2 :
        relayStatus[1] = recvData.relay_state;
        break;
    }
    relayStatusChange = true;
  }
}

// callback when data is sent
void OnDataSent (const uint8_t * mac_addr, esp_now_send_status_t status) {
    char macStr[18];
    Serial.print("Packet to:");
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.print(macStr);
    Serial.print(" send status: ");
    if (status == 0){
        Serial.println("Delivery success");
        Serial.println("flowCount1 is: ");
        Serial.println(flowCount1);
        Serial.println("flowCount2 is: ");
        Serial.println(flowCount2);
    }
    else{
        Serial.println("Delivery fail");
        Serial.println(status);
    }
    timeFlag = getTimeFlag();
}


int getTimeFlag(){
  return random(1000, 3000); // 5~7s interval
  
}
