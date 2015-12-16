#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include <Servo.h>
#include <IRremote.h>
//--------------------------------------------------------------------------------------------------------------------
#define relayPin1 A0 // Bơm - IN1
#define relayPin2 A1 // Yếu - IN2
#define relayPin3 A2 // Trung Bình - IN3
#define relayPin4 A3 // Mạnh - IN4
// A4,A5: LCD
#define pirPin5 7 // Nhìn từ sau quạt: 54321
#define pirPin4 1
#define pirPin3 2
#define pirPin2 3
#define pirPin1 4
#define buzzerPin 5
#define dhtPin 6
//#define hcsr05Pin 7
#define irReceiverPin 8
#define servoPin 9 // ~
#define selectBtnPin 11
#define upBtnPin 13
#define downBtnPin 10
#define backBtnPin 12
//--------------------------------------------------------------------------------------------------------------------
#define posVal1 0
#define posVal2 23
#define posVal3 45
#define posVal4 68
#define posVal5 90
#define posVal6 112
#define posVal7 135
#define posVal8 158
#define posVal9 180
#define servoSpeedMin 20
#define servoSpeedMax 50
//-----------------------------------------------------------------------------------------------------------------
#define maxTime 600
//--------------------------------------------------------------------------------------------------------------------
byte degree[8] = {0B01110,0B01010,0B01110,0B00000,0B00000,0B00000,0B00000,0B00000};
byte cursor[8] = {0B00000,0B00000,0B01110,0B01110,0B01110,0B01110,0B00000,0B00000};
byte downArrow[8] = {0B00000,0B00000,0B00000,0B11111,0B01110,0B00100,0B00000,0B00000};
byte upArrow[8] = {0B00000,0B00000,0B00100,0B01110,0B11111,0B00000,0B00000,0B00000};
int isEeprom,servoType,servo1,servo2,powerType,powerVal,pumpType,pumpConfig,pumpVal,powerConfig1,powerConfig2,powerConfig3,remoteVal,upBtnVal,downBtnVal,selectBtnVal,backBtnVal,pirVal1,pirVal2,pirVal3,pirVal4,pirVal5,ser1,ser2,posSer1,posSer2,pos,posEx,servoDir,serSpeed,buzzerVal,currentPage,currentCursor,changeCursor,changePage,changeVal,currentPower,currentPump,beginSerVal,chkDht,turnOffAfter,turnOnAfter,tem,hum;
unsigned long time1,time2,time3,time4,time5,time6;
//--------------------------------------------------------------------------------------------------------------------
Servo SERVO;
dht DHT;
LiquidCrystal_I2C LCD(0x27,20,4);
IRrecv IRREC(irReceiverPin);
decode_results IRVAL;
//--------------------------------------------------------------------------------------------------------------------
void eepromUpdate(int address, byte val)
{
  if (EEPROM.read(address) != val ) {
    EEPROM.write(address, val);
  }
}
//--------------------------------------------------------------------------------------------------------------------
boolean hasHuman()
{
  if (pirVal1 == 1 || pirVal2 == 1 || pirVal3 == 1 || pirVal4 == 1 || pirVal5 == 1)
  {
    return true;
  } else
  {
    return false;
  }
}
//--------------------------------------------------------------------------------------------------------------------
void readSettings()
{
  /*
    Param: isEeprom
    Results: servoType,servo1,servo2,servoSpeed,powerType,powerVal,pumpType,pumpConfig,pumpVal,powerConfig1,powerConfig2,powerConfig3
  */
  isEeprom = EEPROM.read(0);
  if (isEeprom != 1)
  {
    eepromUpdate(0,1);
    eepromUpdate(1,1);
    eepromUpdate(2,1);
    eepromUpdate(3,9);
    eepromUpdate(4,40);
    eepromUpdate(5,1);
    eepromUpdate(6,1);
    eepromUpdate(7,1);
    eepromUpdate(8,70);
    eepromUpdate(9,0);
    eepromUpdate(10,25);
    eepromUpdate(11,27);
    eepromUpdate(12,30);
  }
  //--------------------------------------------------------------------------------------------------------------------
  servoType = EEPROM.read(1);
  servo1 = EEPROM.read(2);
  servo2 = EEPROM.read(3);
  serSpeed = EEPROM.read(4);
  powerType = EEPROM.read(5);
  powerVal = EEPROM.read(6);
  pumpType = EEPROM.read(7);
  pumpConfig = EEPROM.read(8);
  powerConfig1 = EEPROM.read(10);
  powerConfig2 = EEPROM.read(11);
  powerConfig3 = EEPROM.read(12);
  //Serial.println("SETTINGS LOADED");
}
//--------------------------------------------------------------------------------------------------------------------
void readRemote()
{
  /*
    Result: remoteVal
  */
  if (IRREC.decode(&IRVAL))
  {
    switch(IRVAL.value)
    {
      case 0xFFA25D: remoteVal = 1;  //Serial.println("Remote Data Received: CH-");
        break;
      case 0xFF629D: remoteVal = 2;  //Serial.println("Remote Data Received: CH");
        break;
      case 0xFFE21D: remoteVal = 3;  //Serial.println("Remote Data Received: CH+");
        break;
      case 0xFF22DD: remoteVal = 4;  //Serial.println("Remote Data Received: PREV");
        break;
      case 0xFF02FD: remoteVal = 5;  //Serial.println("Remote Data Received: NEXT");
        break;
      case 0xFFC23D: remoteVal = 6;  //Serial.println("Remote Data Received: PLAY/PAUSE");
        break;
      case 0xFFE01F: remoteVal = 7;  //Serial.println("Remote Data Received: VOL-");
        break;
      case 0xFFA857: remoteVal = 8;  //Serial.println("Remote Data Received: VOL+");
        break;
      case 0xFF906F: remoteVal = 9;  //Serial.println("Remote Data Received: EQ");
        break;
      case 0xFF6897: remoteVal = 10;  //Serial.println("Remote Data Received: 0");
        break;
      case 0xFF9867: remoteVal = 11;  //Serial.println("Remote Data Received: 100+");
        break;
      case 0xFFB04F: remoteVal = 12;  //Serial.println("Remote Data Received: 200+");
        break;
      case 0xFF30CF: remoteVal = 13;  //Serial.println("Remote Data Received: 1");
        break;
      case 0xFF18E7: remoteVal = 14;  //Serial.println("Remote Data Received: 2");
        break;
      case 0xFF7A85: remoteVal = 15;  //Serial.println("Remote Data Received: 3");
        break;
      case 0xFF10EF: remoteVal = 16;  //Serial.println("Remote Data Received: 4");
        break;
      case 0xFF38C7: remoteVal = 17;  //Serial.println("Remote Data Received: 5");
        break;
      case 0xFF5AA5: remoteVal = 18;  //Serial.println("Remote Data Received: 6");
        break;
      case 0xFF42BD: remoteVal = 19;  //Serial.println("Remote Data Received: 7");
        break;
      case 0xFF4AB5: remoteVal = 20;  //Serial.println("Remote Data Received: 8");
        break;
      case 0xFF52AD: remoteVal = 21;  //Serial.println("Remote Data Received: 9");
        break;
      case 0xFFFFFF: remoteVal = 22;  //Serial.println("NHAN GIU");
        break;
    };
    IRREC.resume();
  }
}
//--------------------------------------------------------------------------------------------------------------------
void readButton()
{
  /*
    Results: upBtnVal,downBtnVal,selectBtnVal,backBtnVal
  */
  upBtnVal     = digitalRead(upBtnPin);
  downBtnVal   = digitalRead(downBtnPin);
  selectBtnVal = digitalRead(selectBtnPin);
  backBtnVal   = digitalRead(backBtnPin);
  //Serial.print("Button data received: ");
  //Serial.print(upBtnVal);
  //Serial.print(downBtnVal);
  //Serial.print(selectBtnVal);
  //Serial.println(backBtnVal);
}
//--------------------------------------------------------------------------------------------------------------------
void readDht()
{
  /*
    Results: tem,hum
  */
  chkDht = DHT.read11(dhtPin);
  //Serial.print("DHT11 Status: ");
  /* switch (chkDht)
  {
    case DHTLIB_OK: //Serial.println("Ok");
	break;
    case DHTLIB_ERROR_CHECKSUM: //Serial.println("Checksum error");
	break;
    case DHTLIB_ERROR_TIMEOUT: //Serial.println("Time out error");
	break;
    default: //Serial.println("Unknown error");
	break;
  } */
  if (DHT.temperature > 0 && DHT.temperature < 50 && DHT.humidity > 0 && DHT.humidity < 100) {
    tem = DHT.temperature;
    hum = DHT.humidity;
  }
  //Serial.print("DHT11 Data: ");
  //Serial.print(tem);
  //Serial.print(" ");
  //Serial.println(hum);
}
//--------------------------------------------------------------------------------------------------------------------
void readPir()
{
  /*
    Results: pirVal1,pirVal2,pirVal3,pirVal4,pirVal5
  */
  pirVal1 = digitalRead(pirPin1);
  pirVal2 = digitalRead(pirPin2);
  pirVal3 = digitalRead(pirPin3);
  pirVal4 = digitalRead(pirPin4);
  pirVal5 = digitalRead(pirPin5);
  //Serial.print("PIR Data received: ");
  //Serial.print(pirVal5);
  //Serial.print(pirVal4);
  //Serial.print(pirVal3);
  //Serial.print(pirVal2);
  //Serial.println(pirVal1);
}
//--------------------------------------------------------------------------------------------------------------------
void handleRemote()
{
  switch (remoteVal)
  {
    case 1:
      if (servoType == 1)
      {
        servoType = 0;
      } else {
        servoType = 1;
      }
      buzzerVal = 1;
      changeVal = 1;
      eepromUpdate(0,servoType);
    break;
    case 2:
      if (powerType == 1)
      {
        powerType = 0;
      } else {
        powerType = 1;
      }
      buzzerVal = 1;
      changeVal = 1;
      eepromUpdate(5,powerType);
    break;
    case 3:
      if (pumpType == 1)
      {
        pumpType = 0;
      } else {
        pumpType = 1;
      }
      buzzerVal = 1;
      changeVal = 1;
      eepromUpdate(7,pumpType);
    break;
    case 4:
      if (serSpeed > servoSpeedMin)
      {
        serSpeed -= 5;
        changeVal = 1;
        eepromUpdate(4,serSpeed);
        buzzerVal = 1;
      }
    break;
    case 5:
      if (serSpeed < servoSpeedMax)
      {
        serSpeed += 5;
        changeVal = 1;
        eepromUpdate(4,serSpeed);
        buzzerVal = 1;
      }
    break;
    case 6:
      pumpType = 0;
      if (pumpVal == 1)
      {
        pumpVal = 0;
      } else {
        pumpVal = 1;
      }
      changeVal = 1;
      eepromUpdate(9,pumpVal);
      buzzerVal = 1;
    break;
    case 7:
      if (powerVal > 0)
      {
        powerType = 0;
        powerVal--;
        changeVal = 1;
        eepromUpdate(6,powerVal);
        buzzerVal = 1;
      }
    break;
    case 8:
      if (powerVal < 3)
      {
        powerType = 0;
        powerVal++;
        changeVal = 1;
        eepromUpdate(6,powerVal);
        buzzerVal = 1;
      }
    break;
    case 9:
      if (beginSerVal == 0)
      {
        beginSerVal = 1; // Bắt đầu lưu vị trí
        servoType = 0;
        buzzerVal = 1;
      }
    break;
    case 13:
      if (beginSerVal == 1)
      {
        servo1 = 1;
        changeVal = 1;
        beginSerVal = 2;
        eepromUpdate(2,1);
        buzzerVal = 1;
      } else if (beginSerVal == 2 && 1 >= servo1)
      {
        servo2 = 1;
        changeVal = 1;
        beginSerVal = 0;
        eepromUpdate(3,1);
        buzzerVal = 1;
      }
    break;
    case 14:
      if (beginSerVal == 1)
      {
        servo1 = 2;
        changeVal = 1;
        beginSerVal = 2;
        eepromUpdate(2,2);
        buzzerVal = 1;
      } else if (beginSerVal == 2 && 2 >= servo1)
      {
        servo2 = 2;
        changeVal = 1;
        beginSerVal = 0;
        eepromUpdate(3,2);
        buzzerVal = 1;
      }
    break;
    case 15:
      if (beginSerVal == 1)
      {
        servo1 = 3;
        changeVal = 1;
        beginSerVal = 2;
        eepromUpdate(2,3);
        buzzerVal = 1;
      } else if (beginSerVal == 2 && 3 >= servo1)
      {
        servo2 = 3;
        changeVal = 1;
        beginSerVal = 0;
        eepromUpdate(3,3);
        buzzerVal = 1;
      }
    break;
    case 16:
      if (beginSerVal == 1)
      {
        servo1 = 4;
        changeVal = 1;
        beginSerVal = 2;
        eepromUpdate(2,4);
        buzzerVal = 1;
      } else if (beginSerVal == 2 && 4 >= servo1)
      {
        servo2 = 4;
        changeVal = 1;
        beginSerVal = 0;
        eepromUpdate(3,4);
        buzzerVal = 1;
      }
    break;
    case 17:
      if (beginSerVal == 1)
      {
        servo1 = 5;
        changeVal = 1;
        beginSerVal = 2;
        eepromUpdate(2,5);
        buzzerVal = 1;
      } else if (beginSerVal == 2 && 5 >= servo1)
      {
        servo2 = 5;
        changeVal = 1;
        beginSerVal = 0;
        eepromUpdate(3,5);
        buzzerVal = 1;
      }
    break;
    case 18:
      if (beginSerVal == 1)
      {
        servo1 = 6;
        changeVal = 1;
        beginSerVal = 2;
        eepromUpdate(2,6);
        buzzerVal = 1;
      } else if (beginSerVal == 2 && 6 >= servo1)
      {
        servo2 = 6;
        changeVal = 1;
        beginSerVal = 0;
        eepromUpdate(3,6);
        buzzerVal = 1;
      }
    break;
    case 19:
      if (beginSerVal == 1)
      {
        servo1 = 7;
        changeVal = 1;
        beginSerVal = 2;
        eepromUpdate(2,7);
        buzzerVal = 1;
      } else if (beginSerVal == 2 && 7 >= servo1)
      {
        servo2 = 7;
        changeVal = 1;
        beginSerVal = 0;
        eepromUpdate(3,7);
        buzzerVal = 1;
      }
    break;
    case 20:
      if (beginSerVal == 1)
      {
        servo1 = 8;
        changeVal = 1;
        beginSerVal = 2;
        eepromUpdate(2,8);
        buzzerVal = 1;
      } else if (beginSerVal == 2 && 8 >= servo1)
      {
        servo2 = 8;
        changeVal = 1;
        beginSerVal = 0;
        eepromUpdate(3,8);
        buzzerVal = 1;
      }
    break;
    case 21:
      if (beginSerVal == 1)
      {
        servo1 = 9;
        changeVal = 1;
        beginSerVal = 2;
        eepromUpdate(2,9);
        buzzerVal = 1;
      } else if (beginSerVal == 2 && 9 >= servo1)
      {
        servo2 = 9;
        changeVal = 1;
        beginSerVal = 0;
        eepromUpdate(3,9);
        buzzerVal = 1;
      }
    break;
  }
  remoteVal = 0;
}
//--------------------------------------------------------------------------------------------------------------------
void handleButton()
{
  //--------------------------------------------------------------------------------------------------------------------
  if (upBtnVal == 0)
  {
    if (currentPage == 6)
    {
      switch (currentCursor)
      {
        case 1: 
          changeCursor = 1;
          changePage = 1;
          currentCursor = 3;
          currentPage = 3;
        break;
        case 2:
          changeCursor = 1;
          currentCursor = 1;
        break;
        case 3: if (currentPower == 0 && turnOnAfter < maxTime)
        {
          turnOnAfter++;
          changeVal = 1;
        }
        break;
        case 4: if (currentPower > 0 && turnOffAfter < maxTime)
        {
          turnOffAfter++;
          changeVal = 1;
        }
        break;
      }
    } else if (currentCursor > 1 && currentCursor <= 3)
    {
      changeCursor = 1;
      currentCursor--;
    } else if (currentPage == 2)
    {
      switch (currentCursor)
      {
        case 4:
          if (servoType == 1)
          {
            servoType = 0;
          } else
          {
            servoType = 1;
          }
          changeVal = 1;
        break;
        case 5: if (servo1 < 9 && servo1 < servo2)
        {
          servo1++;
          changeVal = 1;
        }
        break;
        case 7: if (servo2 < 9)
        {
          servo2++;
          changeVal = 1;
        }
        break;
        case 6: if (serSpeed < servoSpeedMax)
        {
          serSpeed += 5;
          changeVal = 1;
        }
        break;
      }
    } else if (currentPage == 3)
    {
      switch (currentCursor)
      {
        case 4:
          if (powerType == 1)
          {
            powerType = 0;
          } else {
            powerType = 1;
          }
          changeVal=1;
        break;
        case 6: if (powerVal < 3)
        {
          powerVal++;
          changeVal = 1;
        }
        break;
      }
    } else if (currentPage == 4)
    {
      switch (currentCursor)
      {
        case 4:
          if (pumpType == 1)
          {
            pumpType = 0;
          } else
          {
            pumpType = 1;
          }
          changeVal = 1;
        break;
        case 5: if (pumpConfig < 90)
        {
          pumpConfig++;
          changeVal = 1;
        }
        break;
        case 6:
          pumpVal=!pumpVal;
          changeVal = 1;
        break;
      }
    } else if (currentPage == 5)
    {
      switch (currentCursor)
      {
        case 4: if (powerConfig1 < 60 && powerConfig1+1 < powerConfig2)
        {
          powerConfig1++;
          changeVal = 1;
        }
        break;
        case 5: if (powerConfig2 < 60 && powerConfig2+1 < powerConfig3)
        {
          powerConfig2++;
          changeVal = 1;
        }
        break;
        case 6: if (powerConfig3 < 60)
        {
          powerConfig3++;
          changeVal = 1;
        }
        break;
      }
    }
    upBtnVal = 1;
  } else
  //--------------------------------------------------------------------------------------------------------------------
  if (downBtnVal == 0)
  {
    if (currentPage == 6 )
    {
      switch (currentCursor)
      {
        case 1:
        changeCursor = 1;
        currentCursor = 2;
        case 3: if (turnOnAfter > 0)
          turnOnAfter--;
          changeVal = 1;
        break;
        case 4: if (turnOffAfter > 0)
          turnOffAfter--;
          changeVal = 1;
        break;
      }
    } else if (currentPage == 3 && currentCursor == 3)
    {
      changeCursor = 1;
      changePage = 1;
      currentCursor = 1;
      currentPage = 6;
    } else if (currentCursor < 3 && currentCursor >= 1)
    {
      changeCursor = 1;
      currentCursor++;
    } else if (currentPage == 2)
    {
      switch (currentCursor)
      {
        case 4:
          if (servoType == 1)
          {
            servoType = 0;
          } else {
            servoType = 1;
          }
          changeVal=1;
        break;
        case 5: if (servo1 > 1)
        {
          servo1--;
          changeVal = 1;
        }
        break;
        case 7: if (servo2 > 1 && servo2 > servo1)
        {
          servo2--;
          changeVal = 1;
        }
        break;
        case 6: if (serSpeed > servoSpeedMin)
        {
          serSpeed -= 5;
          changeVal = 1;
        }
        break;
      }
    } else if (currentPage == 3)
    {
      switch (currentCursor)
      {
        case 4:
          if (powerType == 1)
          {
            powerType = 0;
          } else {
            powerType = 1;
          }
          changeVal=1;
        break;
        case 6: if (powerVal > 0)
        {
          powerVal--;
          changeVal = 1;
        }
        break;
      }
    } else if (currentPage == 4)
    {
      switch (currentCursor)
      {
        case 4:
          if (pumpType == 1)
          {
            pumpType = 0;
          } else {
            pumpType = 1;
          }
          changeVal = 1;
        break;
        case 5: if (pumpConfig > 20)
        {
          pumpConfig--;
          changeVal = 1;
        }
        break;
        case 6:
          pumpVal=!pumpVal;
          changeVal = 1;
        break;
      }
    } else if (currentPage == 5)
    {
      switch (currentCursor)
      {
        case 4: if (powerConfig1 > 0)
        {
          powerConfig1--;
          changeVal = 1;
        }
        break;
        case 5: if (powerConfig2 > 0 && powerConfig2-1 > powerConfig1)
        {
          powerConfig2--;
          changeVal = 1;
        }
        break;
        case 6: if (powerConfig3 > 0 && powerConfig3-1 > powerConfig2)
        {
          powerConfig3--;
          changeVal = 1;
        }
        break;
      }
    }
    downBtnVal = 1;
  } else
  //--------------------------------------------------------------------------------------------------------------------
  if (selectBtnVal == 0)
  {
    if (currentPage == 6 && (currentCursor == 1 || currentCursor == 2))
    {
      changeCursor = 1;
      currentCursor+=2;
    } else if (currentPage == 1)
    {
      changePage = 1;
      changeCursor = 1;
      currentPage = currentCursor+1;
      currentCursor = 1;
    } else if (currentCursor == 2 && currentPage == 3)
    {
      changePage = 1;
      currentPage = 5;
      changeCursor = 1;
      currentCursor = 1;
    } else if (currentCursor >= 1 && currentCursor <= 3)
    {
      changeCursor = 1;
      currentCursor += 3;
    } else if (currentCursor == 5 && currentPage == 2)
    {
      changeCursor = 1;
      currentCursor = 7;
    }
    selectBtnVal = 1;
  } else
  //--------------------------------------------------------------------------------------------------------------------
  if (backBtnVal == 0)
  {
    if (currentPage == 6 && (currentCursor == 1 || currentCursor == 2))
    {
      changePage = 1;
      currentPage = 3;
      changeCursor = 1;
      currentCursor = 3;
    } else if (currentPage == 6 && (currentCursor == 3 || currentCursor == 4))
    {
      changeCursor = 1;
      currentCursor-=2;
    } else if (currentCursor > 3 && currentCursor <= 6)
    {
      changeCursor = 1;
      currentCursor -= 3;
    } else if (currentCursor >=1 && currentCursor <=3 && currentPage == 5)
    {
      changePage = 1;
      changeCursor = 1;
      currentCursor = 2;
      currentPage = 3;
    } else if (currentCursor >=1 && currentCursor <=3 && currentPage != 1)
    {
      changePage = 1;
      changeCursor = 1;
      currentCursor = currentPage-1;
      currentPage = 1;
    } else if (currentCursor == 7)
    {
      changeCursor = 1;
      currentCursor = 5;
    }
    backBtnVal = 1;
  }
}
//--------------------------------------------------------------------------------------------------------------------
void handleServo()
{
  /*
    Param: currentPower;
    Results: ser1,ser2 => posSer1,posSer2
  */
  if (currentPower >=1)
  {
    if (servoType == 1)
    {
      if (pirVal5 == 1) { ser2 = 9; }
      else if (pirVal4 == 1) { ser2 = 7; }
      else if (pirVal3 == 1) { ser2 = 5; }
      else if (pirVal2 == 1) { ser2 = 3; }
      else if (pirVal1 == 1) { ser2 = 1; }
      if (pirVal1 == 1) { ser1 = 1; }
      else if (pirVal2 == 1) { ser1 = 3; }
      else if (pirVal3 == 1) { ser1 = 5; }
      else if (pirVal4 == 1) { ser1 = 7; }
      else if (pirVal5 == 1) { ser1 = 9; }
    } else {
      ser1 = servo1;
      ser2 = servo2;
    }
  } else {
    ser1 = 5;
    ser2 = 5;
  }
  switch(ser1)
  {
    case 1: posSer1 = posVal1;
      break;
    case 2: posSer1 = posVal2;
      break;
    case 3: posSer1 = posVal3;
      break;
    case 4: posSer1 = posVal4;
      break;
    case 5: posSer1 = posVal5;
      break;
    case 6: posSer1 = posVal6;
      break;
    case 7: posSer1 = posVal7;
      break;
    case 8: posSer1 = posVal8;
      break;
    case 9: posSer1 = posVal9;
      break;
  };
  switch(ser2)
  {
    case 1: posSer2 = posVal1;
      break;
    case 2: posSer2 = posVal2;
      break;
    case 3: posSer2 = posVal3;
      break;
    case 4: posSer2 = posVal4;
      break;
    case 5: posSer2 = posVal5;
      break;
    case 6: posSer2 = posVal6;
      break;
    case 7: posSer2 = posVal7;
      break;
    case 8: posSer2 = posVal8;
      break;
    case 9: posSer2 = posVal9;
      break;
  };
  //Serial.print("Servo Pos Data: ");
  //Serial.print(posSer1);
  //Serial.print(" ");
  //Serial.println(posSer2);
}
//--------------------------------------------------------------------------------------------------------------------
void ctrlServo()
{
  /*
    Param: posSer1,posSer2,pos,posEx,servoDir,serSpeed
  */
  if (posSer1 == posSer2) // Nếu đứng im
  {
    if (posSer1 != posEx)
    {
      SERVO.attach(servoPin);
      if (pos > posSer1)
      {
        pos--;
        SERVO.write(pos);
      } else if (pos < posSer1)
      {
        pos++;
        SERVO.write(pos);
      } else if (pos == posSer1)
      {
        SERVO.detach();
        posEx = posSer1;
      }
    }
  } else if (servoDir == 0) // Quay từ phải -> trái
  {
    posEx=-1;
    SERVO.attach(servoPin);
    pos++;
    SERVO.write(pos);
    if (pos >= posSer2)
    {
      servoDir = 1;
    }
  } else // Quay từ trái -> phải
  {
    posEx=-1;
    SERVO.attach(servoPin);
    pos--;
    SERVO.write(pos);
    if (pos <= posSer1)
    {
      servoDir = 0;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------
void ctrlLcd()
{
  //--------------------------------------------------------------------------------------------------------------------
  if (changePage == 1)
  {
    LCD.clear();
    changePage = 0;
    changeVal = 1;
    changeCursor = 1;
    switch (currentPage)
    {
      case 1:
        LCD.setCursor(1,0);
        LCD.print("Quay quat");
        LCD.setCursor(1,1);
        LCD.print("Cong suat");
        LCD.setCursor(1,2);
        LCD.print("Phun suong");
        break;
       case 2:
         LCD.setCursor(0,0);
         LCD.print("Cai dat quay");
         LCD.setCursor(1,1);
         LCD.print("Tu dong:");
         LCD.setCursor(1,2);
         LCD.print("Bang tay:");
         LCD.setCursor(1,3);
         LCD.print("Toc do:");
       break;
       case 3:
         LCD.setCursor(0,0);
         LCD.print("Cai dat cong suat");
         LCD.setCursor(19,0);
         LCD.write(3);
         LCD.setCursor(1,1);
         LCD.print("Tu dong:");
         LCD.setCursor(1,2);
         LCD.print("Cai dat nhiet do");
         LCD.setCursor(1,3);
         LCD.print("Bang tay:");
       break;
       case 4:
         LCD.setCursor(0,0);
         LCD.print("Cai dat phun suong");
         LCD.setCursor(1,1);
         LCD.print("Tu dong:");
         LCD.setCursor(1,2);
         LCD.print("Cai dat do am:");
         LCD.setCursor(1,3);
         LCD.print("Bang tay:");
       break;
       case 5:
         LCD.setCursor(0,0);
         LCD.print("Cai dat nhiet do");
         LCD.setCursor(1,1);
         LCD.print("So 1:");
         LCD.setCursor(1,2);
         LCD.print("So 2:");
         LCD.setCursor(1,3);
         LCD.print("So 3:");
       break;
       case 6:
         LCD.setCursor(0,0);
         LCD.print("Cai dat thoi gian");
         LCD.setCursor(19,0);
         LCD.write(4);
         LCD.setCursor(1,1);
         LCD.print("Bat sau:");
         LCD.setCursor(1,2);
         LCD.print("Tat sau:");
       break;
    }
  }
  //--------------------------------------------------------------------------------------------------------------------
  if (changeVal == 1)
  {
    changeVal = 0;
    switch (currentPage)
    {
      case 1:
        LCD.setCursor(0,3);
	LCD.print(tem,1);
	LCD.write(1);
	LCD.print("C ");
	LCD.setCursor(7,3);
	LCD.print(hum,1);
	LCD.print("% ");
	LCD.setCursor(13,3);
	LCD.print(currentPower);
	LCD.setCursor(17,3);
	if (currentPump == 1)
	{
	  LCD.print("Bat");
	} else
	{
	  LCD.print("Tat");
	}
        /* LCD.setCursor(15,0);
        LCD.print(pirVal1);
        LCD.print(pirVal2);
        LCD.print(pirVal3);
        LCD.print(pirVal4);
        LCD.println(pirVal5); */
      break;
      case 2:
        LCD.setCursor(17,1);
        if (servoType == 1)
        {
          LCD.print("Bat");
        } else {
          LCD.print("Tat");
        }
        LCD.setCursor(17,2);
        LCD.print(servo1);
        LCD.setCursor(19,2);
        LCD.print(servo2);
        LCD.setCursor(18,3);
        LCD.print(serSpeed);
        eepromUpdate(1,servoType);
        eepromUpdate(2,servo1);
        eepromUpdate(3,servo2);
        eepromUpdate(4,serSpeed);
      break;
      case 3:
        LCD.setCursor(17,1);
        if (powerType == 1)
        {
          LCD.print("Bat");
        } else {
          LCD.print("Tat");
        }
        LCD.setCursor(19,3);
        LCD.print(powerVal);
        eepromUpdate(5,powerType);
        eepromUpdate(6,powerVal);
      break;
      case 4:
        LCD.setCursor(17,1);
        if (pumpType == 1)
        {
          LCD.print("Bat");
        } else {
          LCD.print("Tat");
        }
        LCD.setCursor(18,2);
        LCD.print(pumpConfig);
        LCD.setCursor(17,3);
        if (pumpVal == true)
        {
          LCD.print("Bat");
        } else {
          LCD.print("Tat");
        }
        eepromUpdate(7,pumpType);
        eepromUpdate(8,pumpConfig);
        eepromUpdate(9,pumpVal);
      break;
      case 5:
        LCD.setCursor(18,1);
        LCD.print(powerConfig1);
        LCD.setCursor(18,2);
        LCD.print(powerConfig2);
        LCD.setCursor(18,3);
        LCD.print(powerConfig3);
        eepromUpdate(10,powerConfig1);
        eepromUpdate(11,powerConfig2);
        eepromUpdate(12,powerConfig3);
      break;
      case 6:
        LCD.setCursor(15,1);
        LCD.print(turnOnAfter);
        LCD.print("  ");
        LCD.setCursor(15,2);
        LCD.print(turnOffAfter);
        LCD.print("  ");
      break;
    }
  }
  //--------------------------------------------------------------------------------------------------------------------
  if (changeCursor == 1)
  {
    changeCursor = 0;
    switch (currentPage)
    {
      case 1:
        LCD.setCursor(0,0);
        LCD.print(" ");
        LCD.setCursor(0,1);
        LCD.print(" ");
        LCD.setCursor(0,2);
        LCD.print(" ");
        LCD.setCursor(0,currentCursor-1);
        LCD.write(2);
       break;
       case 2:
         LCD.setCursor(0,1);
         if (currentCursor == 1) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(0,2);
         if (currentCursor == 2) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(0,3);
         if (currentCursor == 3) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(16,1);
         if (currentCursor == 4) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(16,2);
         if (currentCursor == 5) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(18,2);
         if (currentCursor == 7) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(17,3);
         if (currentCursor == 6) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
       break;
       case 3:
         LCD.setCursor(0,1);
         if (currentCursor == 1) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(0,2);
         if (currentCursor == 2) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(0,3);
         if (currentCursor == 3) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(16,1);
         if (currentCursor == 4) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(18,3);
         if (currentCursor == 6) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
       break;
       case 4:
         LCD.setCursor(0,1);
         if (currentCursor == 1) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(0,2);
         if (currentCursor == 2) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(0,3);
         if (currentCursor == 3) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(16,1);
         if (currentCursor == 4) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(17,2);
         if (currentCursor == 5) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(16,3);
         if (currentCursor == 6) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
       break;
       case 5:
         LCD.setCursor(0,1);
         if (currentCursor == 1) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(0,2);
         if (currentCursor == 2) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(0,3);
         if (currentCursor == 3) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(17,1);
         if (currentCursor == 4) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(17,2);
         if (currentCursor == 5) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(17,3);
         if (currentCursor == 6) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
       break;
       case 6:
         LCD.setCursor(0,1);
         if (currentCursor == 1) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(0,2);
         if (currentCursor == 2) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(14,1);
         if (currentCursor == 3) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
         LCD.setCursor(14,2);
         if (currentCursor == 4) {
           LCD.write(2);
         } else {
           LCD.print(" ");
         }
       break;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------
void ctrlPower()
{
  if (powerType == 1)
  {
    if (hasHuman())
    {
      if (tem > powerConfig1) {
        if (currentPower != 1)
        {
          digitalWrite(relayPin4, HIGH);
          digitalWrite(relayPin3, HIGH);
          digitalWrite(relayPin2, LOW);
          currentPower = 1;
          buzzerVal = 1;
        }
      } else if (hum > powerConfig2) {
        if (currentPower != 2)
        {
          digitalWrite(relayPin4, HIGH);
          digitalWrite(relayPin3, LOW);
          digitalWrite(relayPin2, HIGH);
          currentPower = 2;
          buzzerVal = 1;
        }
      } else if (tem > powerConfig3) {
        if (currentPower != 3)
        {
          digitalWrite(relayPin4, LOW);
          digitalWrite(relayPin3, HIGH);
          digitalWrite(relayPin2, HIGH);
          currentPower = 3;
          buzzerVal = 1;
        }
      } else {
        if (currentPower != 0)
        {
          digitalWrite(relayPin4, HIGH);
          digitalWrite(relayPin3, HIGH);
          digitalWrite(relayPin2, HIGH);
          currentPower = 0;
          buzzerVal = 1;
        }
      }
    } else {
      if (currentPower != 0)
      {
        digitalWrite(relayPin4, HIGH);
        digitalWrite(relayPin3, HIGH);
        digitalWrite(relayPin2, HIGH);
        currentPower = 0;
        buzzerVal = 1;
      }
    }
  } else
  {
    switch (powerVal)
    {
      case 1:
        if (currentPower != 1)
        {
          digitalWrite(relayPin4, HIGH);
          digitalWrite(relayPin3, HIGH);
          digitalWrite(relayPin2, LOW);
          currentPower = 1;
        }
      break;
      case 2:
        if (currentPower != 2)
        {
          digitalWrite(relayPin4, HIGH);
          digitalWrite(relayPin3, LOW);
          digitalWrite(relayPin2, HIGH);
          currentPower = 2;
        }
      break;
      case 3:
        if (currentPower != 3)
        {
          digitalWrite(relayPin4, LOW);
          digitalWrite(relayPin3, HIGH);
          digitalWrite(relayPin2, HIGH);
          currentPower = 3;
        }
      break;
      case 0:
        if (currentPower != 0)
        {
          digitalWrite(relayPin4, HIGH);
          digitalWrite(relayPin3, HIGH);
          digitalWrite(relayPin2, HIGH);
          currentPower = 0;
        }
      break;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------
void ctrlPump()
{
  if (currentPower >=1)
  {
    if (pumpType == 1)
    {
      if (hasHuman() == true)
      {
        if (hum < pumpConfig)
        {
          if (currentPump == 0)
          {
            digitalWrite(relayPin1, HIGH);
            currentPump = 1;
            buzzerVal = 1;
          }
        }
        else
        {
          if (currentPump == 1)
          {
            digitalWrite(relayPin1, LOW);
            currentPump = 0;
            buzzerVal = 1;
          }
        }
      } else
      {
        if (currentPump == 1)
        {
          digitalWrite(relayPin1, LOW);
          currentPump = 0;
          buzzerVal = 1;
        }
      }
    } else
    {
      if (pumpVal == 1)
      {
        if (currentPump == 0)
        {
          digitalWrite(relayPin1, HIGH);
          currentPump = 1;
        }
      } else
      {
        if (currentPump == 1)
        {
          digitalWrite(relayPin1, LOW);
          currentPump = 0;
        }
      }
    }
  } else {
    if (currentPump == 1)
    {
      digitalWrite(relayPin1, LOW);
      currentPump = 0;
      buzzerVal = 1;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------
void ctrlBuzzer()
{
  /*
    Param: buzzerVal
  */
  if (buzzerVal == 1)
  {
    analogWrite(buzzerPin, 0);
    buzzerVal = 0;
  } else {
    analogWrite(buzzerPin, 255);
  }
}
//--------------------------------------------------------------------------------------------------------------------
void setup()
{
  //Serial.begin(9600);
  //--------------------------------------------------------------------------------------------------------------------
  SERVO.attach(servoPin);
  pos = SERVO.read();
  //--------------------------------------------------------------------------------------------------------------------
  IRREC.enableIRIn();
  //--------------------------------------------------------------------------------------------------------------------
  pinMode(selectBtnPin, INPUT_PULLUP);
  pinMode(backBtnPin, INPUT_PULLUP);
  pinMode(upBtnPin, INPUT_PULLUP);
  pinMode(downBtnPin, INPUT_PULLUP);
  //--------------------------------------------------------------------------------------------------------------------
  pinMode(pirPin1, INPUT);
  pinMode(pirPin2, INPUT);
  pinMode(pirPin3, INPUT);
  pinMode(pirPin4, INPUT);
  pinMode(pirPin5, INPUT);
  //--------------------------------------------------------------------------------------------------------------------
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  pinMode(relayPin4, OUTPUT);
  //--------------------------------------------------------------------------------------------------------------------
  digitalWrite(relayPin4, HIGH);
  digitalWrite(relayPin3, HIGH);
  digitalWrite(relayPin2, HIGH);
  digitalWrite(relayPin1, LOW);
  //--------------------------------------------------------------------------------------------------------------------
  LCD.init();
  LCD.backlight();
  LCD.createChar(1, degree);
  LCD.createChar(2, cursor);
  LCD.createChar(3, downArrow);
  LCD.createChar(4, upArrow);
  //--------------------------------------------------------------------------------------------------------------------
  LCD.setCursor(3,1);
  LCD.print("DANG KHOI DONG");
  LCD.setCursor(3,2);
  LCD.print("VUI LONG CHO...");
  delay(1000);
  //--------------------------------------------------------------------------------------------------------------------
  readSettings();
  readDht();
  currentPage = 1;
  currentCursor = 1;
  currentPower = 0;
  currentPump = 0;
  changePage = 1;
  changeCursor = 1;
  changeVal = 1;
  ctrlLcd();
}
//--------------------------------------------------------------------------------------------------------------------
void loop()
{
  //--------------------------------------------------------------------------------------------------------------------
  if ((unsigned long)(millis()-time1) > 200)
  {
    readRemote();
    handleRemote();
    readButton();
    handleButton();
    ctrlLcd();
    time1 = millis();
  }
  //--------------------------------------------------------------------------------------------------------------------
  if ((unsigned long)(millis()-time2) > 500)
  {
    ctrlPower();
    ctrlPump();
    handleServo();
    time2 = millis();
  }
  //--------------------------------------------------------------------------------------------------------------------
  if ((unsigned long)(millis()-time3) > 1000)
  {
    readDht();
    if (servoType == 1)
    {
      readPir();
    }
    changeVal = 1;
    time3 = millis();
  }
  //--------------------------------------------------------------------------------------------------------------------
  if ((unsigned long)(millis()-time4) > serSpeed)
  {
    ctrlServo();
    time4 = millis();
  }
  //--------------------------------------------------------------------------------------------------------------------
  if ((unsigned long)(millis()-time5) > 100)
  {
    ctrlBuzzer();
    time5 = millis();
  }
  //--------------------------------------------------------------------------------------------------------------------
  if ((unsigned long)(millis()-time6) > 60000)
  {
    if (turnOnAfter >0)
    {
      turnOnAfter--;
      if (turnOnAfter == 0)
      {
        powerType = 0;
        if (powerVal == 0) {powerVal = 3;}
        eepromUpdate(6,powerVal);
        buzzerVal = 1;
      }
      changeVal = 1;
    }
    if (turnOffAfter >0)
    {
      turnOffAfter--;
      if (turnOffAfter == 0)
      {
        powerType = 0;
        powerVal = 0;
        eepromUpdate(6,powerVal);
        buzzerVal = 1;
      }
      changeVal = 1;
    }
    time6 = millis();
  }
}
