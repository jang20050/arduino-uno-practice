const int RED_PIN = A5;
const int YELLOW_PIN = A4;
const int GREEN_PIN = A3;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void lightOnly(int pin) {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(pin, HIGH);
  delay(1000);
}

void loop() {
  lightOnly(GREEN_PIN);
  lightOnly(YELLOW_PIN);
  lightOnly(RED_PIN);
}
