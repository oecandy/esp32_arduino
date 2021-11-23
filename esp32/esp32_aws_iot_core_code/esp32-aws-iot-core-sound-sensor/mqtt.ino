void publishMessage() {
  StaticJsonDocument<200> doc;
  doc["sip_thingname"] = THINGS_NAME;
  timeClient.update();
  doc["sendTime"] = timeClient.getEpochTime();
  doc["payload"]["collTime"] = coll_timestatmp;
  doc["payload"]["data"]["WP032"] = result.leq;
  doc["payload"]["data"]["WP033"] = result.lmax;
  doc["payload"]["data"]["WP034"] = result.l10;
  
  
  char json_buffer[512];
  serializeJson(doc, json_buffer); // print to client
  Serial.println(json_buffer);

  client.publish(AWS_IOT_PUBLISH_TOPIC, json_buffer);
}

void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

//  StaticJsonDocument<200> doc;
//  deserializeJson(doc, payload);
//  const char* message = doc["message"];
}
