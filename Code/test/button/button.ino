void setup() {
  Serial.begin(9600);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
}
void loop() {
  int sensorVal10 = digitalRead(10);
  if (sensorVal10 == LOW) {
    Serial.println("10");
  }
  int sensorVal11 = digitalRead(11);
  if (sensorVal11 == LOW) {
    Serial.println("11");
  }
  int sensorVal12 = digitalRead(12);
  if (sensorVal12 == LOW) {
    Serial.println("12");
  }
  int sensorVal13 = digitalRead(13);
  if (sensorVal13 == LOW) {
    Serial.println("13");
  }
  delay(200);
}
