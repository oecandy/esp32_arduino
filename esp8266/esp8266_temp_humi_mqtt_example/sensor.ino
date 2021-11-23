void get_temp_array_unit(){
  // New DHT sensor readings
  // Read temperature as Celsius (the default)
  temp = dht.readTemperature(false);
  Serial.println(temp);
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //temp = dht.readTemperature(true);
  sumTemp += temp;
  
}
