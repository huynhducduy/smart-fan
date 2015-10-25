#define pirPin1 4
#define pirPin2 3
#define pirPin3 2
#define pirPin4 1
#define pirPin5 0
int pirValue1;
int pirValue2;
int pirValue3;
int pirValue4;
int pirValue5;
void setup()
{
  Serial.begin(9600);
  pinMode(pirPin1,INPUT);
  pinMode(pirPin2,INPUT);
  pinMode(pirPin3,INPUT);
  pinMode(pirPin4,INPUT);
  pinMode(pirPin5,INPUT);
}
void loop()
{
  pirValue1=digitalRead(pirPin1);
  pirValue2=digitalRead(pirPin2);
  pirValue3=digitalRead(pirPin3);
  pirValue4=digitalRead(pirPin4);
  pirValue5=digitalRead(pirPin5);
  Serial.print(pirValue1);
  Serial.print(pirValue2);
  Serial.print(pirValue3);
  Serial.print(pirValue4);
  Serial.println(pirValue5);
  delay(100);
}
