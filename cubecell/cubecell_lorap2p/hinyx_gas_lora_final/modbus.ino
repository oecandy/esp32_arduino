void preTransmission(){
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission(){
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setupModbus(){
  // Modbus slave ID 1
  node.begin(SLAVE_ID, softwareSerial);
  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}


void setupModbusPin(){
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}


bool getGasSensorDatas(){
  uint8_t result,j;
  
  // Read 10 registers starting at 0x3100)
  result = node.readHoldingRegisters(0, REGISTER_SIZE);
  if (result == node.ku8MBSuccess){
    registerDatas[2] = String(node.getResponseBuffer(2)).toInt();
    registerDatas[3] = String(node.getResponseBuffer(3)).toInt();
    registerDatas[4] = String(node.getResponseBuffer(4)).toInt();
    registerDatas[5] = String(node.getResponseBuffer(5)).toInt();
    registerDatas[6] = String(node.getResponseBuffer(6)).toInt();
    readingCnt++;
    return 1;
  } else {
    Serial.print("failed, error code : ");
    Serial.println(result, HEX);
    return 0;
  }
}
