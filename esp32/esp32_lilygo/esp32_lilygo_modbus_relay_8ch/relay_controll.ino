void controll_valve(int valv_no, int switch_stat){
  if (!mb.slave()) {
    if(switch_stat){
      mb.writeCoil(SLAVE_ID, valv_no, true, cbWrite);
    } else {
      mb.writeCoil(SLAVE_ID, valv_no, false, cbWrite);
    }
  }
  mb.task();
  yield();
  delay(100);
}
