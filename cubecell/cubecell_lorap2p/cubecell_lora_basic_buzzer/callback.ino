void OnTxDone( void ){
	Serial.println("TX done");
	state=RX;
}

void OnTxTimeout( void ){
  Radio.Sleep();
  Serial.println("TX Timeout");
  state=TX;
}
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr){
  Rssi=rssi;
  rxSize=size;
  memcpy(rxpacket, payload, size );
  rxpacket[size]='\0';
  state=RX;
  Radio.Sleep( );
    
  Serial.printf("\r\nreceived packet \"%s\" with Rssi %d , length %d\r\n",rxpacket,Rssi,rxSize);
  Serial.println("wait to send next packet");
  Serial.printf("%s\r\n",rxpacket);

  DeserializationError error = deserializeJson(doc, rxpacket);

  if(error){
    return;
  } else {
    if(doc["messageType"] == 2 && doc["data"]["valveNo"] == VALVE_NO && doc["deviceType"] == DEVICE_TYPE){
      String data = doc["data"];
      DeserializationError error = deserializeJson(doc, data.c_str());
      
      relayStatus[0] = 1;
      switchControl();  // after RxDone, control valve according to 'relayStatus[0]'
    }
  }
}
