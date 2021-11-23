void controll_valve(int valv_no, int switch_stat){
  if (!mb.slave()) {
    if(switch_stat){
      mb.writeCoil(1, valv_no, true, cbWrite);
    } else {
      mb.writeCoil(1, valv_no, false, cbWrite);
    }
  }
  mb.task();
  yield();
  delay(100);
}
