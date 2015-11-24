#include <digitalWriteFast.h>
#include <dht.h>
#include <Wire.h>
#include <I2C_Anything.h>
#include <Servo.h>
#include <IRremote1.h>
#include <LiquidCrystal_I2C.h>
//----------------------------------------------------------------
#define ardAddr 1
#define dhtPin 2
#define servoPin 3
#define receiverPin 4
#define selectPin 11
#define upPin 13
#define downPin 10
#define backPin 12
//----------------------------------------------------------------
int upVal,downVal,selectVal,backVal;
volatile boolean hasData = false;
volatile long ser1,ser2,serSpeed;
unsigned long time1 = 0;
unsigned long time2 = 0;
int pos,posSer1,posSer2,posEx;
int servoType = 1;
int remoteVal;
//----------------------------------------------------------------
Servo servo;
dht DHT;
LiquidCrystal_I2C lcd(0x27,20,4);
IRrecv irrecv(receiverPin);
decode_results results;
//----------------------------------------------------------------
void setup()
{
  //Serial.begin(9600);
  Wire.begin(ardAddr);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  servo.attach(servoPin);
  pos = servo.read();
  irrecv.enableIRIn();
  pinModeFast(selectPin, INPUT_PULLUP);
  pinModeFast(backPin, INPUT_PULLUP);
  pinModeFast(upPin, INPUT_PULLUP);
  pinModeFast(downPin, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,1);
  lcd.print("DANG KHOI DONG");
  lcd.setCursor(3,2);
  lcd.print("VUI LONG CHO...");
}
//----------------------------------------------------------------
void transSer()
{
  switch(ser1)
  {
    case 1: posSer1 = 0;
    break;
    case 2: posSer1 = 23;
    break;
    case 3: posSer1 = 45;
    break;
    case 4: posSer1 = 68;
    break;
    case 5: posSer1 = 90;
    break;
    case 6: posSer1 = 112;
    break;
    case 7: posSer1 = 135;
    break;
    case 8: posSer1 = 158;
    break;
    case 9: posSer1 = 180;
    break;
  }
  switch(ser2)
  {
    case 1: posSer2 = 0;
    break;
    case 2: posSer2 = 23;
    break;
    case 3: posSer2 = 45;
    break;
    case 4: posSer2 = 68;
    break;
    case 5: posSer2 = 90;
    break;
    case 6: posSer2 = 112;
    break;
    case 7: posSer2 = 135;
    break;
    case 8: posSer2 = 158;
    break;
    case 9: posSer2 = 180;
    break;
  }
}
//----------------------------------------------------------------
void transIR()
{
  //Serial.print("Remote Data Received: ");
  switch(results.value)
  {
    case 0xFFA25D: remoteVal = 1; // Serial.println("CH-");
      break;
    case 0xFF629D: remoteVal = 2; // Serial.println("CH");
      break;
    case 0xFFE21D: remoteVal = 3; // Serial.println("CH+");
      break;
    case 0xFF22DD: remoteVal = 4; // Serial.println("PREV");
      break;
    case 0xFF02FD: remoteVal = 5; // Serial.println("NEXT");
      break;
    case 0xFFC23D: remoteVal = 6; // Serial.println("PLAY/PAUSE");
      break;
    case 0xFFE01F: remoteVal = 7; // Serial.println("VOL-");
      break;
    case 0xFFA857: remoteVal = 8; // Serial.println("VOL+");
      break;
    case 0xFF906F: remoteVal = 9; // Serial.println("EQ");
      break;
    case 0xFF6897: remoteVal = 10; // Serial.println("0");
      break;
    case 0xFF9867: remoteVal = 11; // Serial.println("100+");
      break;
    case 0xFFB04F: remoteVal = 12; // Serial.println("200+");
      break;
    case 0xFF30CF: remoteVal = 13; // Serial.println("1");
      break;
    case 0xFF18E7: remoteVal = 14; // Serial.println("2");
      break;
    case 0xFF7A85: remoteVal = 15; // Serial.println("3");
      break;
    case 0xFF10EF: remoteVal = 16; // Serial.println("4");
      break;
    case 0xFF38C7: remoteVal = 17; // Serial.println("5");
      break;
    case 0xFF5AA5: remoteVal = 18; // Serial.println("6");
      break;
    case 0xFF42BD: remoteVal = 19; // Serial.println("7");
      break;
    case 0xFF4AB5: remoteVal = 20; // Serial.println("8");
      break;
    case 0xFF52AD: remoteVal = 21; // Serial.println("9");
      break;
    case 0xFFFFFF: remoteVal = 22; // Serial.println("NHAN GIU");
      break;
  }
}
//----------------------------------------------------------------
void loop()
{
  upVal     = digitalReadFast(upPin);
  downVal   = digitalReadFast(downPin);
  selectVal = digitalReadFast(selectPin);
  backVal   = digitalReadFast(backPin);
  //----------------------------------------------------------------
  if ((unsigned long)(millis()-time1)>500)
  {
    int chk = DHT.read11(dhtPin);
    /* Serial.print("DHT11 Status: ");
    switch (chk)
    {
      case DHTLIB_OK: Serial.println("Ok");
  	break;
      case DHTLIB_ERROR_CHECKSUM: Serial.println("Checksum error");
  	break;
      case DHTLIB_ERROR_TIMEOUT: Serial.println("Time out error");
  	break;
      default: Serial.println("Unknown error");
  	break;
    } */
    if (hasData)
    {
      transSer();
      hasData = false;
    }
    time1 = millis();
  }
  //----------------------------------------------------------------
  if ((unsigned long)(millis()-time2)>serSpeed)
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
    } else if (servoType == 1) // Quay từ phải -> trái
    {
      posEx=-1;
      servo.attach(servoPin);
      pos++;
      servo.write(pos);
      if (pos >= posSer2)
      {
        servoType = 2;
      }
    } else // Quay từ trái -> phải
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
//----------------------------------------------------------------
void receiveEvent(int howMany)
{
  if (howMany>=(sizeof ser1)+(sizeof ser2))
    {
      I2C_readAnything(ser1);
      I2C_readAnything(ser2);
      I2C_readAnything(serSpeed);
      /* Serial.println("Servo Data Received");
      Serial.print(ser1);
      Serial.print(ser2);
      Serial.print(" ");
      Serial.print(serSpeed); */
      hasData = true;
    }
}
//----------------------------------------------------------------
void requestEvent()
{
  if (irrecv.decode(&results))
  {
    transIR();
    irrecv.resume();
    struct
    {
      byte type;
      byte remote;
      byte upVal;
      byte downVal;
      byte selectVal;
      byte backVal;
    } response;
    response.type      = 2;
    response.remote    = remoteVal;
    response.upVal     = upVal;
    response.downVal   = downVal;
    response.selectVal = selectVal;
    response.backVal   = backVal;
    Wire.write((byte *)&response,sizeof response);
  } else
  {
    struct
    {
      byte type;
      byte tem;
      byte hum;
      byte upVal;
      byte downVal;
      byte selectVal;
      byte backVal;
    } response;
    response.type      = 1;
    response.tem       = DHT.temperature;
    response.hum       = DHT.humidity;
    response.upVal     = upVal;
    response.downVal   = downVal;
    response.selectVal = selectVal;
    response.backVal   = backVal;
    Wire.write((byte *)&response,sizeof response);
  }
}
