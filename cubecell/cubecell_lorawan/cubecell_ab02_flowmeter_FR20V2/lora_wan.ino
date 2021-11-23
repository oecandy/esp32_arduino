/* Prepares the payload of the frame */
static void prepareTxFrame( uint8_t port )
{
  /*appData size is LORAWAN_APP_DATA_MAX_SIZE which is defined in "commissioning.h".
  *appDataSize max value is LORAWAN_APP_DATA_MAX_SIZE.
  *if enabled AT, don't modify LORAWAN_APP_DATA_MAX_SIZE, it may cause system hanging or failure.
  *if disabled AT, LORAWAN_APP_DATA_MAX_SIZE can be modified, the max value is reference to lorawan region and SF.
  *for example, if use REGION_CN470, 
  *the max value for different DR can be found in MaxPayloadOfDatarateCN470 refer to DataratesCN470 and BandwidthsCN470 in "RegionCN470.h".
  */
  getFlowSensorDatas();
  
  appDataSize = 10;
  appData[0] = 0x49;

  char hexStr[20];
  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           registerDatas[4],registerDatas[5]); // 실시간 유량 정수 2byre hex + 소수 2byte hex
  getHexByteArr(hexStr);
  
  appData[1] = hexByte[0];
  appData[2] = hexByte[1];
  appData[3] = hexByte[2];
  appData[4] = hexByte[3];
  
  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           registerDatas[6],registerDatas[7]); // 실시간 유량 정수 2byre hex + 소수 2byte hex
  getHexByteArr(hexStr);
  
  appData[5] = 0x50;
  appData[6] = hexByte[0];
  appData[7] = hexByte[1];
  appData[8] = hexByte[2];
  appData[9] = hexByte[3];

}

// Convert Hex string to hex byte array
void getHexByteArr(char* hexStr){
  
  unsigned long hexLong = strtoul(hexStr, NULL, 16);
  for(int i = 3; i >= 0; i--){
    hexByte[i] = hexLong & 0xFF;
    hexLong >>= 8;
    Serial.printf("%02X\n", hexByte[i]);
  }
}

// Not used
void downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
  Serial.printf("+REV DATA:%s,RXSIZE %d,PORT %d\r\n",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  Serial.print("+REV DATA:");
}
