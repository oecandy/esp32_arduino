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
  sprintf(flowChar, "%04d", (int) (FL.ulong_val/10));
  Serial.print("Battery Level :: ");
  Serial.print(BT.float_val);
  Serial.println("V");
  appDataSize = 11;
  appData[0] = 0x42;
  appData[1] = (int)battChar[0];
  appData[2] = (int)battChar[1];
  appData[3] = (int)battChar[2];
//  appData[1] = 48 + voltage/1000;
//  appData[2] = 48 + (voltage%1000)/100;
//  appData[3] = 48 + (voltage%100)/10;
  appData[4] = 0x56;
  if(valveCommand == 1){
    appData[5] = 0x31;
  } else {
    appData[5] = 0x30;
  }
  appData[6] = 0x46;
  appData[7] = (int)flowChar[0];
  appData[8] = (int)flowChar[1];
  appData[9] = (int)flowChar[2];
  appData[10] = (int)flowChar[3];
  
  FL.ulong_val = 0;
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
  if(valveStat == 49){
    valveCommand = 1;
    openValve();
    delay(1000);
    Serial.println("valve opened");
  } else if(valveStat == 48){
    valveCommand = 0;
    closeValve();
    Serial.println("valve closed");
  }
}
