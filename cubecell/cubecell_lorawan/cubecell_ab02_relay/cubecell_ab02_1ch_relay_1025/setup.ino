void pinSetup(){
  pinMode(VALVE_1_PIN, OUTPUT);
  pinMode(FLOWMETER_PIN, INPUT_PULLUP);
  attachInterrupt(FLOWMETER_PIN,getFlow,FALLING);
}

void getFlow (void){
  FL.ulong_val++;
}
