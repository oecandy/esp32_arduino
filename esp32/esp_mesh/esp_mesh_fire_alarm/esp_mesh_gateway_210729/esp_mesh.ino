void initEspMesh(){
//  mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE);  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.setRoot(true);
  mesh.setContainsRoot(true);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}

void sendMessage() {
  String msg = "{\"messageType\":3,\"deviceType\":0,\"message\":\"ping\"}";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 3, TASK_SECOND * 5 ));
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  DeserializationError error = deserializeJson(doc, msg.c_str());
  if(error){
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  } else {
    if(doc["messageType"] == 1){
      String result;
      serializeJson(doc["data"], result);
      String topic;
      switch((int)doc["deviceType"]){
        case 1:
          topic = VALVE_1CH_TOPIC;
          mqttClient.publish(topic.c_str(), result.c_str());
          break;
        case 2:
          topic = VALVE_2CH_TOPIC;
          mqttClient.publish(topic.c_str(), result.c_str());
          break;
        case 3:
          topic = VALVE_6CH_TOPIC;
          mqttClient.publish(topic.c_str(), result.c_str());
          break;
        case 5:
          topic = VIB_DATA_TOPIC;
          mqttClient.publish(topic.c_str(), result.c_str());
          break;
        case 10:
          topic = BUZZER_DATA_TOPIC;
          mqttClient.publish(topic.c_str(), result.c_str());
          break;
        case 11:
          topic = FIRE_DATA_TOPIC;
          topic += doc["data"]["serialNo"].as<String>();
          topic += "/data";
          mqttClient.publish(topic.c_str(), result.c_str());
          break;
      }
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
