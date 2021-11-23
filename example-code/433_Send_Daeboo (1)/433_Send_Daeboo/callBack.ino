void callBack(){
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(23);
  
  // Optional set protocol (default is 1, will work for most outlets)
   mySwitch.setProtocol(1);

  // Optional set pulse length.
  mySwitch.setPulseLength(345);
  
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(12);

  // Switch codes
  int ignR = 4460136;
  int ignB = 4460132;
  int ignW = 4460140;
  int fuel = 4460130;
  int vv1 = 4460138;
  int vv2 = 4460134;
  int pump = 4107650;


  Serial.println("callBack function start");
  /* Same switch as above, but using decimal code */
  // Default: OFF (RR, BB, WX)
  // ON: RX, BX, WX
  mySwitch.send(fuel, 24); //Now, the fuel valve is OPEN
  delay(300);  
  mySwitch.send(ignR, 24);
  delay(300);  
  mySwitch.send(ignB, 24);
  delay(1000);
  // IGN RX, BX, WW
  mySwitch.send(ignW, 24);
  delay(1300);  // IGN delay
  mySwitch.send(ignW, 24);
  delay(300);  
  // Now, the generator is working!  state: ON  RX BX WX
  
  for (int i = 0; i < 6; ++i) // 30min watering 10min pause * 6 : 4hours routine
  {
  mySwitch.send(vv1, 24); // Valve1 OPEN
  delay(300);    
  mySwitch.send(pump, 24); // Pump OPEN
  delay(1000);   // 30Min later
  mySwitch.send(vv2, 24); // Valve2 OPEN
  delay(300);    
  mySwitch.send(vv1, 24); // Valve1 Close
  delay(1000);   // 30Min later
  mySwitch.send(pump, 24); // Pump Close
  mySwitch.send(vv2, 24); // Valve2 Close
  delay(1000);    // 10Min
  }
  // Turn off the generator : state: OFF RR BB WX
  mySwitch.send(ignR, 24);
  delay(300);  
  mySwitch.send(ignB, 24);
  delay(300);  
  mySwitch.send(fuel, 24); //close the fuel valve
}
