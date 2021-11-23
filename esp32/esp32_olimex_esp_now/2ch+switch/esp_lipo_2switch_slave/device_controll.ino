void checkSwitchStatus(){
  relayStatus[0] = digitalRead(SWITCH_1_PIN);
  relayStatus[1] = digitalRead(SWITCH_2_PIN);
  if(bootSwitch){
    bootSwitch = false;
    relayStatusChange = true;
  } else {
    if(recentSwtich[0] != relayStatus[0]){
      changeSwitch = 1;
      relayStatusChange = true;
    } else if(recentSwtich[1] != relayStatus[1]){
      changeSwitch = 2;
      relayStatusChange = true;
    }
  }
  
  recentSwtich[0] = relayStatus[0];
  recentSwtich[1] = relayStatus[1];

  
  delay(100);
}
