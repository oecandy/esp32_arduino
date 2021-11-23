void OnTxDone( void ){
	Serial.println("TX done");
	turnOnRGB(COLOR_RECEIVED,0);
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
  turnOnRGB(COLOR_RECEIVED,0);
  state=RX;
  Radio.Sleep( );
  Serial.printf("%s\r\n",rxpacket);
}
