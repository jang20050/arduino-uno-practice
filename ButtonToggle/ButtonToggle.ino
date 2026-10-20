const int RED_PIN = A5;
const int YELLOW_PIN = A4;
const int GREEN_PIN = A3;
const int BUTTON_PIN = A0;

bool ledOn = false;
int lastButton = HIGH;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  // 버튼 한쪽은 A0, 다른 쪽은 GND에 연결 (내부 풀업 사용)
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int button = digitalRead(BUTTON_PIN);

  if (button != lastButton) {
    // 떼어져 있다가 눌린 순간에만 상태를 바꿈
    if (button == LOW) {
      ledOn = !ledOn;
      digitalWrite(RED_PIN, ledOn);
      digitalWrite(YELLOW_PIN, ledOn);
      digitalWrite(GREEN_PIN, ledOn);
    }
    delay(50);  // 누를 때와 뗄 때의 버튼 떨림(채터링) 무시
  }

  lastButton = button;
}
