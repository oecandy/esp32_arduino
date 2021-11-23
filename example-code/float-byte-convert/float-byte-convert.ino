uint16_t data_convert_array[2] = {};

void setup()
{
  Serial.begin(9600);
  

}


void convert_byte(float x){
  long *ptr;
  int *pointer;
  
  ptr = (long*)&x;
  pointer = (int*)&x;
  long m = *ptr;
  Serial.println(m, HEX);  //shows:3FAF5C29
  //---------------------
  int n = *pointer;
  Serial.println(n, HEX); //shows: 5C29
//  pointer++;
  Serial.println(*pointer, HEX); //shows: 3FAF
  Serial.println(*pointer);
  
  data_convert_array[1] = (uint16_t)*pointer;
  data_convert_array[0] = (uint16_t)n;
}

void loop()
{
  float x  = 28.8;
  convert_byte(x);
  Serial.println(data_convert_array[1]);
  Serial.println(data_convert_array[0]);
  
  delay(5000);
}
