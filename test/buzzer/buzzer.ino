void setup() {
}
void loop() {
  analogWrite(5, 255); // Tat
  delay(1000);
  analogWrite(5, 0); // Bat
  delay(1000);
}
