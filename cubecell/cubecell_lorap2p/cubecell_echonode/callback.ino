void OnTxDone( void ){
  Serial.print("TX done......");
  turnOnRGB(0,0);
  state=RX;
}

void OnTxTimeout( void ){
    Radio.Sleep( );
    Serial.print("TX Timeout......");
    state=TX;
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr ){
    Rssi=rssi;
    rxSize=size;
    memcpy(txpacket, payload, size );
    txpacket[size]='\0';
    turnOnRGB(COLOR_RECEIVED,0);
    Radio.Sleep( );

    Serial.printf("\r\nreceived packet \"%s\" with Rssi %d , length %d\r\n",txpacket,Rssi,rxSize);
    
    turnOnRGB(0,0);

    state=TX;
}
