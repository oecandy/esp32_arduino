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
  int fuel = 4460136;
  int choke = 4460132;
  int ignR = 4460140;
  int ignB = 4460130;
  int ignW = 4460138;
  int power = 4460134;
  int valve1 = 11049256;
  int valve2 = 11049254;


  Serial.println("callBack function start");
  /* Same switch as above, but using decimal code */
  // Default: OFF (RR, BB, WX)
  // ON: RX, BX, WX
  mySwitch.send(fuel, 24); //Now, the fuel valve is OPEN
  delay(300); 
  mySwitch.send(choke, 24); //Now, the fuel valve is OPEN
  delay(300);  //5000
  mySwitch.send(ignR, 24);
  delay(300);  
  mySwitch.send(ignB, 24);
  delay(300); //1000
  // IGN RX, BX, WW
  mySwitch.send(ignW, 24);
  delay(1700);  // IGN delay 1700
  mySwitch.send(ignW, 24);
  delay(300);  
  mySwitch.send(choke, 24); //Now, the fuel valve is OPEN
  delay(5000); //5000  
  // Now, the generator is working!  state: ON  RX BX WX
  
  for (int i = 0; i < 6; ++i) // 30min watering 10min pause * 6 : 4hours routine
  {
  mySwitch.send(valve1, 24); // Valve1 OPEN
  delay(300);    
  mySwitch.send(power, 24); // Pump OPEN
  delay(3000);   // 30Min later 2100000
  mySwitch.send(valve2, 24); // Valve2 OPEN
  delay(300);    
  mySwitch.send(valve1, 24); // Valve1 Close
  delay(3000);   // 30Min later 2100000
  mySwitch.send(power, 24); // Pump Close
  mySwitch.send(valve2, 24); // Valve2 Close
  delay(3000);    // 10Min 600000
  }
  // Turn off the generator : state: OFF RR BB WX
  mySwitch.send(ignR, 24);
  delay(300);  
  mySwitch.send(ignB, 24);
  delay(300);  
  mySwitch.send(fuel, 24); //close the fuel valve
}
