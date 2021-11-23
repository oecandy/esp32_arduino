void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(wifiCount < 15){
      wifiCount ++;
    } else {
      wifiCount = 0;
//      WiFi.reconnect();
      ESP.restart();
    }
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void bright_1(){
  if(brightness1 < 255){
    brightness1++;
    Serial.println(brightness1);
    analogWrite(LED_PIN_1, brightness1);
  }
}

void no_bright_1(){
  if(brightness1 > 0){
    brightness1--;
    Serial.println(brightness1);
    analogWrite(LED_PIN_1, brightness1);
  }
}

void bright_2(){
  if(brightness2 < 255){
    brightness2++;
    Serial.println(brightness2);
    analogWrite(LED_PIN_2, brightness2);
  }
}

void no_bright_2(){
  if(brightness2 > 0){
    brightness2--;
    Serial.println(brightness2);
    analogWrite(LED_PIN_2, brightness2);
  }
}
