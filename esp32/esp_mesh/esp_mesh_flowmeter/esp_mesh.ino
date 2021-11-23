void sendMessage() {
  
  char hexStr[18]; 
   
  String msg = "{\"deviceType\":";
  msg += DEVICE_TYPE;
  msg += ",\"messageType\":1";
  msg += ",\"data\":{\"device_no\":";
  msg += DEVICE_NO;
  msg += ",\"flowmeter\":\"";
  
  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           reg_vals[1], reg_vals[0]); // 실시간 유량
  msg += hexStr;

  msg += "\",\"flowrate\":\"";

  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           reg_vals[3], reg_vals[2]); // 실시간 유속

  msg += hexStr;

  msg += "\",\"foward_int\":\"";

  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           reg_vals[5], reg_vals[4]); // 정방향 정수

  msg += hexStr;

  msg += "\",\"foward_float\":\"";

  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           reg_vals[7], reg_vals[6]); // 정방향 실수

  msg += hexStr;

  msg += "\",\"reverse_int\":\"";

  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           reg_vals[9], reg_vals[8]); // 역방향 정수

  msg += hexStr;

  msg += "\",\"reverse_float\":\"";

  snprintf(hexStr, sizeof(hexStr), "%04X%04X",
           reg_vals[11], reg_vals[10]); // 역방향 실수

  msg += hexStr;
  
  msg += "\"}}";
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
  mesh.onReceive(&receivedCallback);
  mesh.setContainsRoot(true);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}
