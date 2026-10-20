const byte SOUND_AO = A0;
const byte SOUND_DO = 2;
const byte RED_PIN = 3;
const byte GREEN_PIN = 6;
const byte ACTIVE_LEVEL = LOW;  // 실제 보드가 반대라면 HIGH로 변경
const int PEAK_THRESHOLD = 5;   // 조용할 때 Peak는 0~2, 이 값 이상이면 소리로 판단
const unsigned long HOLD_MS = 300;  // 마지막 소리 후 이 시간 동안은 "소리 나는 중"으로 유지
bool soundOn = false;
unsigned long lastSound = 0;
unsigned long windowAt = 0;
unsigned long lastReport = 0;
int minimumAdc = 1023;
int maximumAdc = 0;
int peakToPeak = 0;

void setup() {
  Serial.begin(9600);
  pinMode(SOUND_DO, INPUT_PULLUP);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  lastSound = millis() - HOLD_MS;
  windowAt = millis();
  Serial.println(F("Sound ready"));
}

void loop() {
  const unsigned long now = millis();
  const int raw = analogRead(SOUND_AO);
  minimumAdc = min(minimumAdc, raw);
  maximumAdc = max(maximumAdc, raw);

  // 마이크 AO는 파형이므로 20ms 구간의 최대-최소 차이로 소리 크기를 봅니다.
  if (now - windowAt >= 20) {
    peakToPeak = maximumAdc - minimumAdc;
    minimumAdc = 1023;
    maximumAdc = 0;
    windowAt = now;
  }

  // DO 신호가 들어오거나 AO 파형이 기준 이상으로 흔들리면 소리가 난 것으로 판단
  const bool active = digitalRead(SOUND_DO) == ACTIVE_LEVEL;
  if (active || peakToPeak >= PEAK_THRESHOLD) {
    lastSound = now;
  }
  soundOn = now - lastSound < HOLD_MS;

  // 소리 나는 중: 초록 켜고 빨강 끔 / 조용함: 빨강 켜고 초록 끔
  digitalWrite(GREEN_PIN, soundOn ? HIGH : LOW);
  digitalWrite(RED_PIN, soundOn ? LOW : HIGH);

  if (now - lastReport >= 100) {
    lastReport = now;
    Serial.print(F("AO:")); Serial.print(raw);
    Serial.print(F(" Peak:")); Serial.print(peakToPeak);
    Serial.print(F(" DO:")); Serial.print(active ? 1 : 0);
    Serial.print(F(" Sound:")); Serial.println(soundOn ? 1 : 0);
  }
}
