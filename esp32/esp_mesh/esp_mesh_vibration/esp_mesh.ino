void sendMessage() {
  String msg = "{\"deviceType\":";
  msg += DEVICE_TYPE;
  msg += ",\"messageType\":1";
  msg += ",\"data\":{\"device_no\":";
  msg += DEVICE_NO;
  msg += ",\"x_g_val\":";
  msg += getMaxValue(x, COLLECT_COUNT);
  msg += ",\"y_g_val\":";
  msg += getMaxValue(y, COLLECT_COUNT);
  msg += ",\"z_g_val\":";
  msg += getMaxValue(z, COLLECT_COUNT);
  msg += ",\"battery\":";
  msg += getBatteryLevel();
  msg += "}}";
  Serial.println(msg);
  mesh.sendBroadcast( msg );
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
//  mesh.sendBroadcast("{\"messageType\":3,\"deviceType\":1,\"message\":\"connected\"}");
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
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.setContainsRoot(true);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}
