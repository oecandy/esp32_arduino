void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr ){
    rssi=rssi;
    rxSize=size;
    memcpy(rxpacket, payload, size );
    rxpacket[size]='\0';
    turnOnRGB(COLOR_RECEIVED,0);
    Radio.Sleep( );
    Serial.printf("\r\nreceived packet \"%s\" with rssi %d , length %d\r\n",rxpacket,rssi,rxSize);
    
    softwareSerial.write(rxpacket); // sending data to orangepi
}
