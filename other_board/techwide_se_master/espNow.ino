// Init ESP Now with fallback
void initESPNow() {
//  WiFi.disconnect();
  //Set device in STA mode to begin with
  WiFi.mode(WIFI_STA);
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
} 

void sendPayload () {
    
    if (myData.payload[0] == 1 || myData.payload[0] == 3 || myData.payload[0] == 5) {
        count = 0;
        if (myData.payload[0] == 1) {
            axisName = "x";
            axisIdx = 1;
            targetTemp = myData.payload[1];
            targetBattery = myData.payload[2];
            targetBootCnt = myData.payload[3];
            for(int i = 0; i < 4; i++){
              Serial.print("axis[");
              Serial.print(i);
              Serial.print("] : ");
              Serial.println(myData.payload[i]);
            }
            Serial.println("x start : ");
        }
        if (myData.payload[0] == 3) {
            axisName = "y";
            axisIdx = 2;
            Serial.println("y start : ");
        }
        if (myData.payload[0] == 5) {
            axisName = "z";
            axisIdx = 3;
            Serial.println("z start : ");
        }
//        }
    } else {
//      for (int i = 0; i < 32; i++){
//        String payloadStr = String (myData.payload[i]);
//        Serial.println(payloadStr);
//      }
      count++;
      mergePayload(mqttPayload, myData.payload, count);
      if(count == 64){
        String jsonData = "{\"val_no\":";
        jsonData += valNo;
        jsonData += ",\"eng_min\":\"-16\",\"eng_max\":\"16\",\"datacnt\":";
        jsonData += dataCnt;
        jsonData += ",\"gain\":";
        jsonData += gain;
        jsonData += ",\"val\":[";
        
        for(int i = 0; i < 2048; i++){
          if(i == 2047){
            jsonData += (String)mqttPayload[i] + "]}";
          } else {
            jsonData += (String)mqttPayload[i] + ",";
          }
        }
        Serial.println(jsonData);
        mqttPublishMessage(jsonData);
      }
    }
}

void mergePayload (int16_t *resultPayload, int16_t *partPayload, int count){
    for (int i = 0; i < 32; i++){
      resultPayload[((count - 1) * 32) + i] = partPayload[i];
    }
}
