// 반응 속도 게임: 카운트다운이 초록색일 때 버튼을 누르면 +1, 그 외에는 -1
// 공통 캐소드(COM = GND), A~G 각각에 330Ω 저항을 사용합니다.
const byte SEGMENT_PINS[] = {2, 3, 4, 5, 6, 7, 8};  // A, B, C, D, E, F, G
const byte BUTTON_PIN = 10;
const byte RED_PIN = 11;
const byte ORANGE_PIN = 12;
const byte GREEN_PIN = 13;
const byte DIGITS[] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66,
  0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

const unsigned long COUNTDOWN_MS = 10000;  // 카운트다운 시간 (변경 가능)
const unsigned long ORANGE_MS = 5000;      // 남은 시간이 이 값 이하면 주황
const unsigned long GREEN_MS = 2000;       // 남은 시간이 이 값 이하면 초록
const byte WIN_SCORE = 9;
const unsigned long WIN_BLINK_MS = 3000;   // 9점 달성 후 깜빡이는 시간

byte score = 0;
unsigned long roundStart = 0;
bool winning = false;
unsigned long winStart = 0;
int lastReading = HIGH;
int stableButton = HIGH;
unsigned long changedAt = 0;
long lastSecond = -1;

void showNumber() {
  for (byte segment = 0; segment < 7; segment++) {
    digitalWrite(SEGMENT_PINS[segment], bitRead(DIGITS[score], segment));
  }
}

void setLights(bool red, bool orange, bool green) {
  digitalWrite(RED_PIN, red);
  digitalWrite(ORANGE_PIN, orange);
  digitalWrite(GREEN_PIN, green);
}

void startRound() {
  roundStart = millis();
  lastSecond = -1;
}

// 버튼이 눌린 순간에만 true (35ms 떨림 방지)
bool buttonPressed() {
  const int reading = digitalRead(BUTTON_PIN);
  const unsigned long now = millis();
  if (reading != lastReading) changedAt = now;
  lastReading = reading;
  if (now - changedAt >= 35 && reading != stableButton) {
    stableButton = reading;
    return stableButton == LOW;
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  for (byte pin : SEGMENT_PINS) pinMode(pin, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RED_PIN, OUTPUT);
  pinMode(ORANGE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  Serial.println(F("Game ready: press the button while green"));
  showNumber();
  startRound();
}

void loop() {
  const unsigned long now = millis();

  // 9점 달성: 3초 동안 모든 LED 깜빡인 뒤 0점으로 초기화
  if (winning) {
    const bool on = (now - winStart) / 250 % 2 == 0;
    setLights(on, on, on);
    if (now - winStart >= WIN_BLINK_MS) {
      winning = false;
      score = 0;
      showNumber();
      Serial.println(F("Reset"));
      startRound();
    }
    return;
  }

  unsigned long elapsed = now - roundStart;
  if (elapsed >= COUNTDOWN_MS) {
    // 카운트다운이 끝나면 다시 처음부터
    startRound();
    elapsed = 0;
  }
  const unsigned long remaining = COUNTDOWN_MS - elapsed;
  const bool green = remaining <= GREEN_MS;
  const bool orange = !green && remaining <= ORANGE_MS;
  setLights(!green && !orange, orange, green);

  const long second = (remaining + 999) / 1000;
  if (second != lastSecond) {
    lastSecond = second;
    Serial.print(F("Countdown:")); Serial.print(second);
    Serial.println(green ? F(" GREEN") : orange ? F(" ORANGE") : F(" RED"));
  }

  if (buttonPressed()) {
    if (green) {
      score++;
      Serial.print(F("Hit! Score:")); Serial.println(score);
      showNumber();
      if (score >= WIN_SCORE) {
        Serial.println(F("WIN!"));
        winning = true;
        winStart = now;
        return;
      }
      // 초록에서 한 번 점수를 얻으면 새 카운트다운 시작 (연타로 점수 올리기 방지)
      startRound();
    } else {
      if (score > 0) score--;  // 0점 아래로 내려가지 않음
      Serial.print(F("Miss! Score:")); Serial.println(score);
      showNumber();
    }
  }
}
