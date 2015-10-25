#include <dht.h>
#include <Wire.h>
#include <I2C_Anything.h>
#include <Servo.h>
#include <IRremote1.h>
#include <LiquidCrystal_I2C.h>
////////////////////////////////////////////////
#define ardAddr 1
#define dhtPin 2
#define servoPin 3
#define receiverPin 4
#define selectPin 11
#define upPin 13
#define downPin 10
#define backPin 12
////////////////////////////////////////////////
int upValue,downValue,selectValue,backValue;
volatile boolean haveData = false;
volatile long ser1,ser2,serSpeed;
unsigned long time1 = 0;
unsigned long time2 = 0;
int pos,posSer1,posSer2,posEx;
int servoType = 1;
int remoteValue;
////////////////////////////////////////////////
Servo servo;
dht DHT;
LiquidCrystal_I2C lcd(0x27,20,4);
IRrecv irrecv(receiverPin);
decode_results results;
////////////////////////////////////////////////
void setup() 
{
  delay(500);
  Serial.begin(9600);
  Wire.begin(ardAddr);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  servo.attach(servoPin);
  pos = servo.read();
  irrecv.enableIRIn();
  pinMode(selectPin, INPUT_PULLUP);
  pinMode(backPin, INPUT_PULLUP);
  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,1);
  lcd.print("DANG KHOI DONG");
  lcd.setCursor(3,2);
  lcd.print("VUI LONG CHO...");
}
//////////////////////////////////////////////////////
void transSer() // Dịch vị trí Servo
{
  switch(ser1)
  {
    case 1: posSer1=0;
    break;
    case 2: posSer1=23;
    break;
    case 3: posSer1=45;
    break;
    case 4: posSer1=68;
    break;
    case 5: posSer1=90;
    break;
    case 6: posSer1=112;
    break;
    case 7: posSer1=135;
    break;
    case 8: posSer1=158;
    break;
    case 9: posSer1=180;
    break;
  }
  switch(ser2)
  {
    case 1: posSer2=0;
    break;
    case 2: posSer2=23;
    break;
    case 3: posSer2=45;
    break;
    case 4: posSer2=68;
    break;
    case 5: posSer2=90;
    break;
    case 6: posSer2=112;
    break;
    case 7: posSer2=135;
    break;
    case 8: posSer2=158;
    break;
    case 9: posSer2=180;
    break;
  }
}
//////////////////////////////////////////////////////////////
void translateIR() // Dịch tín hiệu IR
{
  switch(results.value)
  {
    case 0xFFA25D: remoteValue = 1; // Serial.println("CH-");
    break;
    case 0xFF629D: remoteValue = 2; // Serial.println("CH");
    break;
    case 0xFFE21D: remoteValue = 3; // Serial.println("CH+");
    break;
    case 0xFF22DD: remoteValue = 4; // Serial.println("PREV");
    break;
    case 0xFF02FD: remoteValue = 5; // Serial.println("NEXT");
    break;
    case 0xFFC23D: remoteValue = 6; // Serial.println("PLAY/PAUSE");
    break;
    case 0xFFE01F: remoteValue = 7; // Serial.println("VOL-");
    break;
    case 0xFFA857: remoteValue = 8; // Serial.println("VOL+");
    break;
    case 0xFF906F: remoteValue = 9; // Serial.println("EQ");
    break;
    case 0xFF6897: remoteValue = 10; // Serial.println("0");
    break;
    case 0xFF9867: remoteValue = 11; // Serial.println("100+");
    break;
    case 0xFFB04F: remoteValue = 12; // Serial.println("200+");
    break;
    case 0xFF30CF: remoteValue = 13; // Serial.println("1");
    break;
    case 0xFF18E7: remoteValue = 14; // Serial.println("2");
    break;
    case 0xFF7A85: remoteValue = 15; // Serial.println("3");
    break;
    case 0xFF10EF: remoteValue = 16; // Serial.println("4");
    break;
    case 0xFF38C7: remoteValue = 17; // Serial.println("5");
    break;
    case 0xFF5AA5: remoteValue = 18; // Serial.println("6");
    break;
    case 0xFF42BD: remoteValue = 19; // Serial.println("7");
    break;
    case 0xFF4AB5: remoteValue = 20; // Serial.println("8");
    break;
    case 0xFF52AD: remoteValue = 21; // Serial.println("9");
    break;
    case 0xFFFFFF: remoteValue = 22; // Serial.println("NHAN GIU");
    break;
  }
}
////////////////////////////////////////////////////////////////////////
void loop() 
{
  // Lấy tín hiệu từ button
  upValue = digitalRead(upPin);
  downValue = digitalRead(downPin);
  selectValue = digitalRead(selectPin);
  backValue = digitalRead(backPin);
  /////////////////////////////////////////////////////
  if ((unsigned long)(millis()-time1)>500) // Nhận tín hiệu từ DHT và dịch servo
  {
    int chk = DHT.read11(dhtPin);
    if (haveData)
    {
      transSer();
      haveData = false;  
    }
    time1 = millis();
  }
  ////////////////////////////////////////////////////
  if ((unsigned long)(millis()-time2)>serSpeed) // Điều khiển servo
  {
    if (posSer1 == posSer2) // Nếu đứng im
    {
      if (posSer1 != posEx)
      {
        servo.attach(servoPin);
        if (pos > posSer1)
        {
          pos--;
          servo.write(pos);
        } else if (pos < posSer1)
        {
          pos++;
          servo.write(pos);
        } else if (pos == posSer1)
        {
          servo.detach();
          posEx = posSer1;
        }
      }
    } else if (servoType == 1) // Quay từ phái - trái
    {
      posEx=-1;
      servo.attach(servoPin);
      pos++;
      servo.write(pos);
      if (pos >= posSer2)
      {
        servoType = 2;
      }
    } else // Quay từ trái - phải
    {
      posEx=-1;
      servo.attach(servoPin);
      pos--;
      servo.write(pos);
      if (pos <= posSer1)
      {
        servoType = 1;
      }
    }
    time2 = millis();
  }
}
//////////////////////////////////////////////////
void receiveEvent(int howMany) // Nhận tín hiệu để điều khiển servo
{
  if (howMany>=(sizeof ser1)+(sizeof ser2))
    {
      I2C_readAnything(ser1);   
      I2C_readAnything(ser2);
      I2C_readAnything(serSpeed);  
      //Serial.print(ser1);
      //Serial.print(ser2);
      //Serial.println(serSpeed); 
      haveData=true;    
    }
}
//////////////////////////////////////////////////
void requestEvent()
{
  if (irrecv.decode(&results)) // Nhận tín hiệu IR
  {
    translateIR();
    irrecv.resume(); 
    struct 
    {
      byte type;
      byte remote;
      byte upValue;
      byte downValue;
      byte selectValue;
      byte backValue;
    } response;
    response.type = 2;
    response.remote = remoteValue;
    response.upValue = upValue;
    response.downValue = downValue;
    response.selectValue = selectValue;
    response.backValue = backValue;
    Wire.write((byte *)&response,sizeof response);
  } else
  {
    struct 
    {
      byte type;
      byte tem;
      byte hum;
      byte upValue;
      byte downValue;
      byte selectValue;
      byte backValue;
    } response;
    response.type = 1;
    response.tem = DHT.temperature;  
    response.hum = DHT.humidity;
    response.upValue = upValue;
    response.downValue = downValue;
    response.selectValue = selectValue;
    response.backValue = backValue;
    Wire.write((byte *)&response,sizeof response);
  }
}
