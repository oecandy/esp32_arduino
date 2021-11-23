void sendMessage() {
  String msg = "{\"deviceType\":";
  msg += DEVICE_TYPE;
  msg += ",\"channel\":";
  msg += CHANNEL;
  msg += ",\"messageType\":1";
  msg += ",\"data\":{\"valveNo\":";
  msg += VALVE_NO;
  msg += ",\"state\":";
  msg += relayStatus[0];
  msg += ",\"flow\":";
  msg += (1000*flowCount)/(INTERVAL_TIME + timeFlag);
  msg += "}}";
  mesh.sendBroadcast( msg );
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  DeserializationError error = deserializeJson(doc, msg.c_str());

  if(error){
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  } else {
    if(doc["messageType"] == 2 && doc["valveNo"] == VALVE_NO && doc["deviceType"] == DEVICE_TYPE){
      relayStatus[0] = (int)doc["state"];
      relayStatusChange = true;
      Serial.println("valve control");
      Serial.println((int)doc["state"]);
    }
  }
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
