void setup() {
  // put your setup code here, to run once:
pinMode(1,OUTPUT);
pinMode(2,OUTPUT);
pinMode(3,OUTPUT);
pinMode(12, INPUT);
}

void loop() {
  int lectura = digitalRead(12);
  if ( lectura == 1) {
    digitalWrite(1, 1);
    digitalWrite(2, 1);
    digitalWrite(3, 1);
    delay(2000);
  } else {
    digitalWrite(1, 0);
    digitalWrite(2, 0);
    digitalWrite(3, 0);
  }
}