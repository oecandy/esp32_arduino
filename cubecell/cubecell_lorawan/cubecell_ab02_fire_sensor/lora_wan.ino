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
  getFireSensorDatas();
  sprintf(tempChar, "%04d", TP.int_val);
  sprintf(humiChar, "%04d", HM.int_val);
  
  appDataSize = 12;
  appData[0] = 0x54;
  appData[1] = (int)tempChar[0];
  appData[2] = (int)tempChar[1];
  appData[3] = (int)tempChar[2];
  appData[4] = (int)tempChar[3];
  appData[5] = 0x46;
  appData[6] = (int)humiChar[0];
  appData[7] = (int)humiChar[1];
  appData[8] = (int)humiChar[2];
  appData[9] = (int)humiChar[3];
  appData[10] = 0x41;
  if(fireState == 1){
    appData[11] = 0x31;
  } else {
    appData[11] = 0x30;
  }
}



//downlink data handle function example
void downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
  Serial.printf("+REV DATA:%s,RXSIZE %d,PORT %d\r\n",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  Serial.print("+REV DATA:");
}
