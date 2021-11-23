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
  BT.float_val = getBatteryVoltage();
  sprintf(battChar, "%03d", (int) (BT.float_val/10));
  Serial.print("Battery Level :: ");
  Serial.print(BT.float_val);
  Serial.println("V");
  appDataSize = 11;
  appData[0] = 0x42;
  appData[1] = (int)battChar[0];
  appData[2] = (int)battChar[1];
  appData[3] = (int)battChar[2];
  appData[4] = 0x56;
  if(valveCommand[0] == 1){
    appData[5] = 0x31;
  } else {
    appData[5] = 0x30;
  }
  appData[6] = 0x46;
  appData[7] = 0x30;
  appData[8] = 0x30;
  appData[9] = 0x30;
  appData[10] = 0x30;
}



//downlink data handle function example
void downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
  Serial.printf("+REV DATA:%s,RXSIZE %d,PORT %d\r\n",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  Serial.print("+REV DATA:");
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++)
  {
    Serial.printf("%02X",mcpsIndication->Buffer[i]);
  }
  Serial.println();
  valveStat = mcpsIndication->Buffer[5];
  Serial.printf("%02X",mcpsIndication->Buffer[5]);
  Serial.println();
  Serial.println(valveStat);
  if(valveStat == 49 || valveStat == 48){
    valveCommand[0] = 1;
    openValve();
    Serial.println("valve opened");
    delay(15000);
    closeValve();
    Serial.println("valve closed");
  }
}
