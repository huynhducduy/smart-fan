#include <Wire.h>
#include <I2C_Anything.h>
const byte galAddr=1;
void setup() 
{
  Wire.begin();
  Serial.begin(9600);
}
void loop() 
{
  float x = 3.14;  
  long y = 4;
  Wire.beginTransmission(galAddr);
  I2C_writeAnything(x);
  I2C_writeAnything(y);
  Wire.endTransmission();
  Wire.requestFrom(galAddr,2);
  if (Wire.available()>0)
  {
    Serial.println((char)Wire.read()-48);
  }
  delay(1000);
}
