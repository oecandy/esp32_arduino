void wifi_setup(){
  
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
    
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void pin_setup(){
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(FLOW_METER_1_PIN, INPUT_PULLUP);
}
