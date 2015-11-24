#include <Servo.h>
Servo servo;
int pos=0;
int posSer1=0;
int posSer2=180;
unsigned long time2 = 0;
int servoType = 1;
void setup()
{
  servo.attach(3);
}
void loop()
{
  if ( (unsigned long) (millis() - time2) > 35) // 15ms/độ
  {
    if (posSer1 == posSer2) // Nếu đứng im
    {
      if (pos > posSer1)
      {
        pos--;
        servo.write(pos);
      }
      if (pos < posSer1)
      {
        pos++;
        servo.write(pos);
      }
    } else if (servoType == 1) // Quay từ phái - trái
    {
      Serial.println(servo.read());
      pos++;
      servo.write(pos);
      if (pos >= posSer2)
      {
        servoType=2;
      }
    } else // Quay từ trái - phải
    {
      pos--;
      servo.write(pos);
      if (pos <= posSer1)
      {
        servoType=1;
      }
    }
    time2 = millis();
  }
}
