#include <Wire.h>
#include <I2C_Anything.h>
const byte galAddr=1;
void setup() 
{
  Wire.begin(galAddr);
  Serial.begin(9600);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}
volatile boolean haveData = false;
volatile float x;
volatile long y;
void loop() 
{
 if (haveData)
 {
   Serial.print ("X = ");
   Serial.println(x);  
   Serial.print ("Y = ");
   Serial.println(y);  
   haveData = false;  
 }
}
void receiveEvent (int howMany)
{
  if (howMany>=(sizeof x)+(sizeof y))
   {
    I2C_readAnything(x);   
    I2C_readAnything(y);   
    haveData=true;     
  }
}
void requestEvent()
{
  char data[]={'1','2'};
  Wire.write(data);
}
