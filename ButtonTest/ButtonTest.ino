void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
}

void loop() {
  Serial.print("digital=");
  Serial.print(digitalRead(A0));
  Serial.print(" analog=");
  Serial.println(analogRead(A0));
  delay(200);
}
