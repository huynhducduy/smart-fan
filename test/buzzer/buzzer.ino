void setup() {
}
void loop() {
  analogWrite(5, 255); // Tắt
  delay(1000);
  analogWrite(5, 0); // Bật
  delay(1000);
}
