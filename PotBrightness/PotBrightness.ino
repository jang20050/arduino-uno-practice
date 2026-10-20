const int RED_PIN = 3;
const int YELLOW_PIN = 5;
const int GREEN_PIN = 6;
const int POT_PIN = A0;

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {
  int value = analogRead(POT_PIN);

  // 0.2초마다 가변저항 값과 전압을 시리얼로 출력
  if (millis() - lastPrint >= 200) {
    lastPrint = millis();
    float voltage = value * 5.0 / 1023.0;
    Serial.print("Value: ");
    Serial.print(value);
    Serial.print("  Voltage: ");
    Serial.print(voltage, 2);
    Serial.println(" V");
  }

  // 가변저항 값 0~1023을 밝기 0~255로 변환해서 PWM 출력
  int brightness = value / 4;
  analogWrite(RED_PIN, brightness);
  analogWrite(YELLOW_PIN, brightness);
  analogWrite(GREEN_PIN, brightness);
}
