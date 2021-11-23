#define SensorPin 2
#define LedPin 7

void setup() {
  Serial.begin(115200);
  pinMode(SensorPin, INPUT);
  pinMode(LedPin, OUTPUT);
}

void loop() {
  int sensor = digitalRead(SensorPin);

  if (sensor == HIGH) {
    digitalWrite(LedPin, HIGH);
    Serial.println("ON");
  } else {
    digitalWrite(LedPin, LOW);
    Serial.println("OFF");
  }
}
