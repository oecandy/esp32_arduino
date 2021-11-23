void sendMessage() {
  String msg = "{\"deviceType\":";
  msg += DEVICE_TYPE;
  msg += ",\"messageType\":1";
  msg += ",\"data\":{\"device_no\":";
  msg += DEVICE_NO;
  msg += ",\"state\":"; // 1 : 화재 발생, 0 : 정상
  msg += registerDatas[6];
  msg += ",\"temperature\":";
  msg += (float)registerDatas[0]/10;
  msg += ",\"humidity\":";
  msg += (float)registerDatas[1]/10;
  msg += ",\"serialNo\":\"";
  msg += SERIAL_NO;
  msg += "\"}}";
  mesh.sendBroadcast( msg );
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void initEspMesh(){
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.setContainsRoot(true);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}
