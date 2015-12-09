#define relayPin1 A0
void setup()
{
  pinMode(relayPin1,OUTPUT);
}
void loop()
{
  boolean value=0;
  value=!value;
  digitalWrite(relayPin1,value);
  delay(1000);
  value=!value;
  digitalWrite(relayPin1,value);
  delay(1000);
}
