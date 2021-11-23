void initTempSensor(){
  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example, also can be left in blank for default address use
  // Also there is a table with all addres possible for this sensor, you can connect multiple sensors
  // to the same i2c bus, just configure each sensor with a different address and define multiple objects for that
  //  A2 A1 A0 address
  //  0  0  0   0x18  this is the default address
  //  0  0  1   0x19
  //  0  1  0   0x1A
  //  0  1  1   0x1B
  //  1  0  0   0x1C
  //  1  0  1   0x1D
  //  1  1  0   0x1E
  //  1  1  1   0x1F
  
  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
  }else {
    Serial.println("Found MCP9808!");
  }
  

  tempsensor.setResolution(MCP9808_RESOLUTION); // sets the resolution mode of reading, the modes are defined in the table bellow:
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms
}


int16_t getTemperature(){
  tempsensor.wake(); // wake up, ready to read!
  // Read the temperature
  float c_temp = tempsensor.readTempC();

  // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
  tempsensor.shutdown_wake(1);

  return  (int16_t) (c_temp * 100);
}
