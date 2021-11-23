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

bool getFireSensorDatas(){
  uint8_t result,j;
  
  // Read 10 registers starting at 0x3100)
  result = node.readHoldingRegisters(0, REGISTER_SIZE);
  if (result == node.ku8MBSuccess){
    TP.int_val = String(node.getResponseBuffer(0)).toInt();
    HM.int_val = String(node.getResponseBuffer(1)).toInt();
    fireState = String(node.getResponseBuffer(6)).toInt();
    
    return 1;
  } else {
    Serial.print("failed, error code : ");
    Serial.println(result, HEX);
    return 0;
  }
}
