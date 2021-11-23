void pinSetup(){
  pinMode(VALVE_OPEN_1_1, OUTPUT);
  pinMode(VALVE_OPEN_1_2, OUTPUT);
  pinMode(FLOW_IN, INPUT);
  attachInterrupt(FLOW_IN,getFlow,FALLING);
}

void getFlow (void){
  FL.ulong_val++;
}
