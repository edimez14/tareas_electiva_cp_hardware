const int pinLED1 = 9;   
const int pinLED2 = 10;  
const int pinLED3 = 11;  
const int pinLED4 = 12;  

const int pinLDR = A0;   

int valorLuz = 0;        

void setup() {
  pinMode(pinLED1, OUTPUT);
  pinMode(pinLED2, OUTPUT);
  pinMode(pinLED3, OUTPUT);
  pinMode(pinLED4, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  valorLuz = analogRead(pinLDR);
  
  Serial.print("Valor de luz: ");
  Serial.println(valorLuz);
  
  if (valorLuz < 200) {
    digitalWrite(pinLED1, HIGH);
    digitalWrite(pinLED2, HIGH);
    digitalWrite(pinLED3, HIGH);
    digitalWrite(pinLED4, HIGH);
  } 
  else if (valorLuz >= 200 && valorLuz < 400) {
    digitalWrite(pinLED1, HIGH);
    digitalWrite(pinLED2, HIGH);
    digitalWrite(pinLED3, HIGH);
    digitalWrite(pinLED4, LOW);
  } 
  else if (valorLuz >= 400 && valorLuz < 600) {
    digitalWrite(pinLED1, HIGH);
    digitalWrite(pinLED2, HIGH);
    digitalWrite(pinLED3, LOW);
    digitalWrite(pinLED4, LOW);
  } 
  else if (valorLuz >= 600 && valorLuz < 800) {
    digitalWrite(pinLED1, HIGH);
    digitalWrite(pinLED2, LOW);
    digitalWrite(pinLED3, LOW);
    digitalWrite(pinLED4, LOW);
  } 
  else {
    digitalWrite(pinLED1, LOW);
    digitalWrite(pinLED2, LOW);
    digitalWrite(pinLED3, LOW);
    digitalWrite(pinLED4, LOW);
  }
  
  delay(100);
}