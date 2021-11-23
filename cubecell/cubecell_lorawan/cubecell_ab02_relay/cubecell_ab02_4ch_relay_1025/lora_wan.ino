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
  appDataSize = 14;
  appData[0] = 0x42;
  appData[1] = (int)battChar[0];
  appData[2] = (int)battChar[1];
  appData[3] = (int)battChar[2];
//  appData[1] = 48 + voltage/1000;
//  appData[2] = 48 + (voltage%1000)/100;
//  appData[3] = 48 + (voltage%100)/10;
  appData[4] = 0x56;
  if(valveCommand[0] == 1){
    appData[5] = 0x31;
  } else {
    appData[5] = 0x30;
  }
  if(valveCommand[1] == 1){
    appData[6] = 0x31;
  } else {
    appData[6] = 0x30;
  }
  if(valveCommand[2] == 1){
    appData[7] = 0x31;
  } else {
    appData[7] = 0x30;
  }
  if(valveCommand[3] == 1){
    appData[8] = 0x31;
  } else {
    appData[8] = 0x30;
  }
  appData[9] = 0x46;
  appData[10] = (int)flowChar[0];
  appData[11] = (int)flowChar[1];
  appData[12] = (int)flowChar[2];
  appData[13] = (int)flowChar[3];
  
  FL.ulong_val = 0;
}



//downlink data handle function example
void downLinkDataHandle(McpsIndication_t *mcpsIndication) // 1:on 2:on => VkFMVkUxMQ==, 1:off 2:off => VkFMVkUwMA==
{
  Serial.printf("+REV DATA:%s,RXSIZE %d,PORT %d\r\n",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  Serial.print("+REV DATA:");
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++)
  {
    Serial.printf("%02X",mcpsIndication->Buffer[i]);
  }
  Serial.println();
  valveStat = mcpsIndication->Buffer[5];
  Serial.print("ValveNo :: ");
  Serial.printf("%02X",mcpsIndication->Buffer[5]);
  Serial.println();
  Serial.println(valveStat);
  int valveNo;
  switch(valveStat){
    case 49:
      valveNo = 1;
      break;
    case 50:
      valveNo = 2;
      break;
    case 51:
      valveNo = 3;
      break;
    case 52:
      valveNo = 4;
      break;
    default:
      valveNo = 0;
      break;
  }

  valveStat = mcpsIndication->Buffer[6];
  Serial.print("State :: ");
  Serial.printf("%02X",mcpsIndication->Buffer[6]);
  Serial.println();
  Serial.println(valveStat);
  
  switch(valveStat){
    case 49:
      openValve(valveNo);
      break;
    case 48:
      closeValve(valveNo);
      break;
    default:
      break;
  }

  
}
