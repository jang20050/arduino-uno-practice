const int RED_PIN = A5;
const int YELLOW_PIN = A4;
const int GREEN_PIN = A3;
const int BUTTON_PIN = A0;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  // 버튼 한쪽은 A0, 다른 쪽은 GND에 연결 (내부 풀업 사용)
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // 풀업이라 버튼을 누르면 LOW
  int state = digitalRead(BUTTON_PIN) == LOW ? HIGH : LOW;
  digitalWrite(RED_PIN, state);
  digitalWrite(YELLOW_PIN, state);
  digitalWrite(GREEN_PIN, state);
}
