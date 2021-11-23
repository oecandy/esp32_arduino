void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println(topic);
  DeserializationError error = deserializeJson(doc, (char*)payload);
  if(error){
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  } else {
    if((int)doc["SW01"] == 1){
      valveData.valve_no = 1;
      valveData.switch_no = 1;
      valveData.relay_state = 1;
      Serial.println("valve1 open");
      sendData();
    } else if ((int)doc["SW01"] == 2){
      valveData.valve_no = 1;
      valveData.switch_no = 1;
      valveData.relay_state = 0;
      Serial.println("valve1 close");
      sendData();
    }
    if((int)doc["SW02"] == 1){
      valveData.valve_no = 1;
      valveData.switch_no = 2;
      valveData.relay_state = 1;
      Serial.println("valve2 open");
      sendData();
    } else if ((int)doc["SW02"] == 2){
      valveData.valve_no = 1;
      valveData.switch_no = 2;
      valveData.relay_state = 0;
      Serial.println("valve2 close");
      sendData();
    }
    if((int)doc["SW03"] == 1){
      valveData.valve_no = 2;
      valveData.switch_no = 1;
      valveData.relay_state = 1;
      Serial.println("valve3 open");
      sendData();
    } else if ((int)doc["SW03"] == 2){
      valveData.valve_no = 2;
      valveData.switch_no = 1;
      valveData.relay_state = 0;
      Serial.println("valve3 close");
      sendData();
    }
    if((int)doc["SW04"] == 1){
      valveData.valve_no = 2;
      valveData.switch_no = 2;
      valveData.relay_state = 1;
      Serial.println("valve4 open");
      sendData();
    } else if ((int)doc["SW04"] == 2){
      valveData.valve_no = 2;
      valveData.switch_no = 2;
      valveData.relay_state = 0;
      Serial.println("valve4 close");
      sendData();
    }
    
  }
}

void wifiMqttInit() {
  //WiFi.onEvent(WiFiEvent);
  
  mqttClient.setServer(mqttServerIp, mqttServerPort);
  mqttClient.setCallback(callback);
  mqttClient.setBufferSize(maxMqttBufferSize);
  mqttClient.subscribe(SUB_TOPIC);
}

//void mqttPublishMessage(){
//  String jsonData = "{\"valveNo\":";
//  jsonData += myData.valve_no;
//  jsonData += ",\"SW01\":{\"state\":";
//  jsonData += myData.relay_state[0];
//  jsonData += ",\"flow\":";
//  jsonData += myData.flow_count[0];
//  jsonData += "},\"SW02\":{\"state\":";
//  jsonData += myData.relay_state[1];
//  jsonData += ",\"flow\":";
//  jsonData += myData.flow_count[1];
//  jsonData += "}}";
//  
//  mqttClient.publish(DATA_TOPIC, jsonData.c_str());
//}

void mqttPublishData(){
  for(int i = 0; i < 4; i ++){
    flow_status[i] = flow_status[i] >= 100 ? 0 : flow_status[i]; 
  }
  
  String jsonData = "{\"relayChannel\":";
  jsonData += 4;
  jsonData += ",\"serialNo\":\"";
  jsonData += SERIAL_NO;
  jsonData += "\",\"valve\":{";
  jsonData += "\"SW01\":{\"state\":";
  jsonData += valve_status[0];
  jsonData += ",\"flowCount\":";
  jsonData += flow_status[0];
  jsonData += "},\"SW02\":{\"state\":";
  jsonData += valve_status[1];
  jsonData += ",\"flowCount\":";
  jsonData += flow_status[1];
  jsonData += "},\"SW03\":{\"state\":";
  jsonData += valve_status[2];
  jsonData += ",\"flowCount\":";
  jsonData += flow_status[2];
  jsonData += "},\"SW04\":{\"state\":";
  jsonData += valve_status[3];
  jsonData += ",\"flowCount\":";
  jsonData += flow_status[4];
  jsonData += "}}}";
  
  mqttClient.publish (DATA_TOPIC, jsonData.c_str());
}

void mqttPublishHealthCheck(){
  String jsonData = "{\"serialNo\":\"";
  jsonData += SERIAL_NO;
  jsonData += "\",\"channel\":";
  jsonData += CHANNEL;
  jsonData += ",\"macAddress\":\"";
  jsonData += WiFi.macAddress();
  jsonData += "\"}";
  mqttClient.publish (PING_TOPIC, jsonData.c_str());
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESPMODBUS-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      mqttClient.subscribe(SUB_TOPIC);
//       Once connected, publish an announcement...
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqttSendLoop(){
  unsigned long currentMillis = millis();

  if (currentMillis - mqttMillis >= INTERVAL_TIME) {
      mqttMillis = currentMillis;
      mqttPublishData();
  }
}
