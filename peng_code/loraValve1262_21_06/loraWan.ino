/* LoRa ---------------------------------------------------------------------------------------------------------------------- */
uint8_t check_com_count = 0;

void count_LoRa_join (void)
{
  if(IsLoRaMacNetworkJoined)  // join이면
    check_com_count = 0;
  else
    ++ check_com_count;
}
/* --------------------------------------------------------------------------------------------------------------------------- */


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
    appDataSize = 21;
    appData[0] = 0x42; // 'B'attery
    sprintf(battChar, "%03d", (int) (BT.float_val/10));
    appData[1] = (int)battChar[0];      // 3 Int value
    appData[2] = (int)battChar[1];      // value
    appData[3] = (int)battChar[2];      // value
    appData[4] = 0x56; // 'V'avle
    appData[5] = 0x30;
//    if(valveCommand == 1){
//      appData[5] = 0x31;
//    } else {
//      appData[5] = 0x30;
//    }
    sprintf(flowChar, "%04d", (int) (FL.ulong_val/15));
    appData[6] = 0x46; // 'F'lowmeter
    appData[7] = (int)flowChar[0];      // 4s Int value
    appData[8] = (int)flowChar[1];      // value
    appData[9] = (int)flowChar[2];      // value
    appData[10] = (int)flowChar[3];      // value
    appData[11] = 0x54; // 'T'emperature
    appData[12] = 0x30;      // 3 Int val
    appData[13] = 0x30;      // value
    appData[14] = 0x30;      // value
    appData[15] = 0x48; // 'H'umidity
    appData[16] = 0x30;      // 3 Int val
    appData[17] = 0x30;      // value
    appData[18] = 0x30;      // value

    FL.ulong_val = 0;
    nodeWoke = 1;               // adc flow valve 제어 시작
    tx_buf = 0;
}

const char* getCharArray(float f){
  const char* result;
  String strVal = (int)f + "";
  result = strVal.c_str();
  return result;
}

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
    secondCommand = 1;
    relayGeneratorOn();
    Serial.println("operation Generator");
    startGtime = millis();
    delay(1000); 
    relayOn();
    Serial.println("valve opened");
    delay(1000);
    relayOff();
    Serial.println("valve closed");
  } else if(valveStat == 48){
    relayGeneratorOff();
    Serial.println("shut down Generator");
  }
}
