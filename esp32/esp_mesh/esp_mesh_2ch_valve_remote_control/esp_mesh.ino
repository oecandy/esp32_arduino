void sendMessage() {
  String msg = "{\"deviceType\":";
  msg += DEVICE_TYPE;
  msg += ",\"channel\":";
  msg += CHANNEL;
  msg += ",\"messageType\":1";
  msg += ",\"data\":{\"valveNo\":";
  msg += VALVE_NO;
  msg += ",\"SW01\":{\"state\":";
  msg += relayStatus[0];
  msg += ",\"flow\":";
  msg += (1000*flowCount1)/(INTERVAL_TIME + timeFlag);
  msg += "},\"SW02\":{\"state\":";
  msg += relayStatus[1];
  msg += ",\"flow\":";
  msg += (1000*flowCount2)/(INTERVAL_TIME + timeFlag);
  msg += "}}}";
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
      relayStatus[(int)doc["switchNo"] - 1] = (int)doc["state"];
      switchNo = (int)doc["switchNo"];
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
  mesh.onReceive(&receivedCallback);
  mesh.setContainsRoot(true);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}
