#include <Wire.h>
#include <I2C_Anything.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
////////////////////////////////////////////////
#define ardAddr 1
#define pirPin1 4
#define pirPin2 3
#define pirPin3 2
#define pirPin4 1
#define pirPin5 0
#define relayPin1 A0 // Bơm - IN1
#define relayPin2 A1 // Mạnh - IN2
#define relayPin3 A2 // Trung Bình - IN3
#define relayPin4 A3 // Yếu - IN4
#define buzzPin 5
////////////////////////////////////////////////
byte degree[8] = {0B01110,0B01010,0B01110,0B00000,0B00000,0B00000,0B00000,0B00000};
byte cursor[8] = {0B00000,0B00000,0B01110,0B01110,0B01110,0B01110,0B00000,0B00000};
int buzzValue,pirValue1,pirValue2,pirValue3,pirValue4,pirValue5,ser1,ser2,selectValue,upValue,downValue,backValue;
volatile long type,tem,hum,remote;
unsigned long time1 = 0;
unsigned long time2 = 0;
unsigned long time3 = 0;
int currentPage = 1;
int currentCursor = 1;
int changeCursor = 0;
int changePage = 0;
int changeValue = 0;
int currentPower, currentPump, beginSerValue;
int isEeprom,servoType,servo1,servo2,servoSpeed,powerType,powerValue,powerConfig1,powerConfig2,powerConfig3,pumpType,pumpConfig,pumpValue;
//////////////////////////////////////////////////
LiquidCrystal_I2C lcd(0x27,20,4);
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  ///////////////////////////
  pinMode(pirPin1,INPUT);
  pinMode(pirPin2,INPUT);
  pinMode(pirPin3,INPUT);
  pinMode(pirPin4,INPUT);
  pinMode(pirPin5,INPUT);
  ////////////////////////////
  pinMode(relayPin1,OUTPUT);
  pinMode(relayPin2,OUTPUT);
  pinMode(relayPin3,OUTPUT);
  pinMode(relayPin4,OUTPUT);
  ////////////////////////////
  digitalWrite(relayPin4,HIGH);
  digitalWrite(relayPin3,HIGH);
  digitalWrite(relayPin2,HIGH);
  digitalWrite(relayPin1,HIGH);
  ////////////////////////////
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, degree);
  lcd.createChar(2, cursor);
  ///////////////////////////
  lcd.setCursor(0,0);
  lcd.write(2);
  /////////////////////////////
  lcd.setCursor(1,0);
  lcd.print("Quay quat");
  lcd.setCursor(1,1);
  lcd.print("Cong suat");
  lcd.setCursor(1,2);
  lcd.print("Phun suong");
  /////////////////////////////
  isEeprom = EEPROM.read(0);
  if (isEeprom != 1) // Ghi các giá trị mặc định cho lần đầu tiên
  {
    EEPROM.write(0,1);
    EEPROM.write(1,1);
    EEPROM.write(2,1);
    EEPROM.write(3,9);
    EEPROM.write(4,40);
    EEPROM.write(5,1);
    EEPROM.write(6,1);
    EEPROM.write(7,1);
    EEPROM.write(8,70);
    EEPROM.write(9,0);
    EEPROM.write(10,30);
    EEPROM.write(11,27);
    EEPROM.write(12,25);
  }
  // Đọc giá trị cài đặt
  servoType = EEPROM.read(1);
  servo1 = EEPROM.read(2);
  servo2 = EEPROM.read(3);
  servoSpeed = EEPROM.read(4);
  powerType = EEPROM.read(5);
  powerValue = EEPROM.read(6);
  pumpType = EEPROM.read(7);
  pumpConfig = EEPROM.read(8);
  pumpValue = EEPROM.read(9);
  powerConfig1 = EEPROM.read(10);
  powerConfig2 = EEPROM.read(11);
  powerConfig3 = EEPROM.read(12);
}
/////////////////////////////////////////
boolean checkTemHum() // Kiểm tra giá trị từ DHT
{
  if ((tem != 25 && hum != 25) && tem<=60 && tem>=0 && hum>=20 && hum<=90)
  {
    return true;
  } else
  {
    return false;
  }
}
/////////////////////////////////////////
void handleServo() // Đọc tín hiệu từ PIR và dịch sang servo
{
  // Serial.print(pirValue5);
  // Serial.print(pirValue4);
  // Serial.print(pirValue3);
  // Serial.print(pirValue2);
  // Serial.println(pirValue1);
  if (currentPower >=1)
  {
    if (servoType == 1)
    {
      if (pirValue5 == 1) { ser1 = 9; }
      if (pirValue4 == 1) { ser1 = 7; }
      if (pirValue3 == 1) { ser1 = 5; }
      if (pirValue2 == 1) { ser1 = 3; }
      if (pirValue1 == 1) { ser1 = 1; }
      if (pirValue1 == 1) { ser2 = 1; }
      if (pirValue2 == 1) { ser2 = 3; }
      if (pirValue3 == 1) { ser2 = 5; }
      if (pirValue4 == 1) { ser2 = 7; }
      if (pirValue5 == 1) { ser2 = 9; }
    } else {
      ser1=servo1;
      ser2=servo2;
    }
  } else {
    ser1=5;
    ser2=5;
  }
}
///////////////////////////////////////////
boolean humanScan() // Kiểm tra có người xung quanh
{
  if (pirValue1 == 1 || pirValue2 == 1 || pirValue3 == 1 || pirValue4 == 1 || pirValue5 ==1)
  {
    return true;
  } else {
    return false;
  }
}
///////////////////////////////////////////
void handlePower() // Điều khiển công suất
{
  if (powerType == 1)
  {
    if (humanScan() == true)
    {
      if (tem > powerConfig1) {
        if (currentPower != 1)
        {
          digitalWrite(relayPin4,HIGH);
          digitalWrite(relayPin3,HIGH);
          digitalWrite(relayPin2,LOW);
          currentPower = 1;
        }
      } else if (tem > powerConfig2) {
        if (currentPower != 2)
        {
          digitalWrite(relayPin4,HIGH);
          digitalWrite(relayPin3,LOW);
          digitalWrite(relayPin2,HIGH);
          currentPower = 2;
        }
      } else if (tem > powerConfig3) {
        if (currentPower != 3)
        {
          digitalWrite(relayPin4,LOW);
          digitalWrite(relayPin3,HIGH);
          digitalWrite(relayPin2,HIGH);
          currentPower = 3;
        }
      } else {
        if (currentPower != 0)
        {
          digitalWrite(relayPin4,HIGH);
          digitalWrite(relayPin3,HIGH);
          digitalWrite(relayPin2,HIGH);
          currentPower = 0;
        }
      }
    } else {
      if (currentPower != 0)
      {
        digitalWrite(relayPin4,HIGH);
        digitalWrite(relayPin3,HIGH);
        digitalWrite(relayPin2,HIGH);
        currentPower = 0;
      }
    }
  } else
  {
    switch (powerValue)
    {
      case 1:
        if (currentPower != 1)
        {
          digitalWrite(relayPin4,HIGH);
          digitalWrite(relayPin3,HIGH);
          digitalWrite(relayPin2,LOW);
          currentPower = 1;
        }
      break;
      case 2:
        if (currentPower != 2)
        {
          digitalWrite(relayPin4,HIGH);
          digitalWrite(relayPin3,LOW);
          digitalWrite(relayPin2,HIGH);
          currentPower = 2;
        }
      break;
      case 3:
        if (currentPower != 3)
        {
          digitalWrite(relayPin4,LOW);
          digitalWrite(relayPin3,HIGH);
          digitalWrite(relayPin2,HIGH);
          currentPower = 3;
        }
      break;
      case 0:
        if (currentPower != 0)
        {
          digitalWrite(relayPin4,HIGH);
          digitalWrite(relayPin3,HIGH);
          digitalWrite(relayPin2,HIGH);
          currentPower = 0;
        }
      break;
    }
  }
}
/////////////////////////////////////////////////////////
void handlePump() // Điều khiển bơm
{
  if (currentPower >=1)
  {
    if (pumpType == 1)
    {
      if (humanScan() == true)
      {
        if (hum<pumpConfig)
        {
          if (currentPump == 0)
          {
            digitalWrite(relayPin1,LOW);
            currentPump = 1;
          }
        }
        else
        {
          if (currentPump == 1)
          {
            digitalWrite(relayPin1,HIGH);
            currentPump = 0;
          }
        }
      } else
      {
        if (currentPump == 1)
        {
          digitalWrite(relayPin1,HIGH);
          currentPump = 0;
        }
      }
    } else 
    {
      if (pumpValue == 1)
      {
        if (currentPump == 0)
        {
          digitalWrite(relayPin1,LOW);
          currentPump = 1;
        }
      } else
      {
        if (currentPump == 1)
        {
          digitalWrite(relayPin1,HIGH);
          currentPump = 0;
        }
      }
    }
  } else {
    if (currentPump == 1)
    {
      digitalWrite(relayPin1,HIGH);
      currentPump = 0;
    }
  }
}
/////////////////////////////////////////////////////////
void handlePage1() // Điểu khiển giao diện trang 1
{
  if (currentPage == 1)
  {
    lcd.setCursor(0,3);
    lcd.print(tem,1);
    lcd.write(1);
    lcd.print("C ");
    lcd.setCursor(7,3);
    lcd.print(hum,1);
    lcd.print("% ");
    lcd.setCursor(13,3);
    lcd.print(currentPower);
    lcd.setCursor(17,3);
    if (currentPump == 1)
    {
      lcd.print("Bat");
    } else 
    {
      lcd.print("Tat");
    }
  }
}
///////////////////////////////////////////
void handleLcd() // Điều khiển giao diện người dùng
{
  /////////////////////////////////////////////
  if (changePage == 1)
  {
    lcd.clear();
    switch(currentPage)
    {
      case 1:
        lcd.setCursor(1,0);
        lcd.print("Quay quat");
        lcd.setCursor(1,1);
        lcd.print("Cong suat");
        lcd.setCursor(1,2);
        lcd.print("Phun suong");
        break;
       case 2:
         lcd.setCursor(0,0);
         lcd.print("Cai dat quay");
         lcd.setCursor(1,1);
         lcd.print("Tu dong:");
         lcd.setCursor(1,2);
         lcd.print("Bang tay:");
         lcd.setCursor(1,3);
         lcd.print("Toc do:");
       break;
       case 3:
         lcd.setCursor(0,0);
         lcd.print("Cai dat cong suat");
         lcd.setCursor(1,1);
         lcd.print("Tu dong:");
         lcd.setCursor(1,2);
         lcd.print("Cai dat nhiet do");
         lcd.setCursor(1,3);
         lcd.print("Bang tay:");
       break;
       case 4:
         lcd.setCursor(0,0);
         lcd.print("Cai dat phun suong");
         lcd.setCursor(1,1);
         lcd.print("Tu dong:");
         lcd.setCursor(1,2);
         lcd.print("Cai dat do am:");
         lcd.setCursor(1,3);
         lcd.print("Bang tay:");
       break;
       case 5:
         lcd.setCursor(0,0);
         lcd.print("Cai dat nhiet do");
         lcd.setCursor(1,1);
         lcd.print("So 1:");
         lcd.setCursor(1,2);
         lcd.print("So 2:");
         lcd.setCursor(1,3);
         lcd.print("So 3:");
       break;
    }
    changePage=0;
    changeValue=1;
  }
  //////////////////////////////////////////
  if (changeValue == 1) // Khi giá trị trên trang thay đổi
  {
    handlePage1();
    changeValue=0;
    switch (currentPage)
    {
      case 2:
        lcd.setCursor(17,1);
        if (servoType == 1)
        {
          lcd.print("Bat");
        } else {
          lcd.print("Tat");
        }
        lcd.setCursor(17,2);
        lcd.print(servo1);
        lcd.setCursor(19,2);
        lcd.print(servo2);
        lcd.setCursor(18,3);
        lcd.print(servoSpeed);
        EEPROM.write(1,servoType);
        EEPROM.write(2,servo1);
        EEPROM.write(3,servo2);
        EEPROM.write(4,servoSpeed);
      break;
      case 3:
        lcd.setCursor(17,1);
        if (powerType == 1)
        {
          lcd.print("Bat");
        } else {
          lcd.print("Tat");
        }
        lcd.setCursor(19,3);
        lcd.print(powerValue);
        EEPROM.write(5,powerType);
        EEPROM.write(6,powerValue);
      break;
      case 4:
        lcd.setCursor(17,1);
        if (pumpType == 1)
        {
          lcd.print("Bat");
        } else {
          lcd.print("Tat");
        }
        lcd.setCursor(18,2);
        lcd.print(pumpConfig);
        lcd.setCursor(17,3);
        if (pumpValue == true)
        {
          lcd.print("Bat");
        } else {
          lcd.print("Tat");
        }
        EEPROM.write(7,pumpType);
        EEPROM.write(8,pumpConfig);
        EEPROM.write(9,pumpValue);
      break;
      case 5:
        lcd.setCursor(18,1);
        lcd.print(powerConfig1);
        lcd.setCursor(18,2);
        lcd.print(powerConfig2);
        lcd.setCursor(18,3);
        lcd.print(powerConfig3);
        EEPROM.write(10,powerConfig1);
        EEPROM.write(11,powerConfig2);
        EEPROM.write(12,powerConfig3);
      break;
    }
  }
  //////////////////////////////////////////////////////////////
  if (changeCursor == 1) // Khi trỏ thay đổi
  {
    switch (currentPage)
    {
      case 1:
        lcd.setCursor(0,0);
        lcd.print(" ");
        lcd.setCursor(0,1);
        lcd.print(" ");
        lcd.setCursor(0,2);
        lcd.print(" ");
        lcd.setCursor(0,currentCursor-1);
        lcd.write(2);
       break;
       case 2:
         lcd.setCursor(0,1);
         if (currentCursor == 1) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(0,2);
         if (currentCursor == 2) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(0,3);
         if (currentCursor == 3) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(16,1);
         if (currentCursor == 4) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(16,2);
         if (currentCursor == 5) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(18,2);
         if (currentCursor == 7) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(17,3);
         if (currentCursor == 6) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
       break;
       case 3:
         lcd.setCursor(0,1);
         if (currentCursor == 1) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(0,2);
         if (currentCursor == 2) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(0,3);
         if (currentCursor == 3) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(16,1);
         if (currentCursor == 4) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(18,3);
         if (currentCursor == 6) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
       break;
       case 4:
         lcd.setCursor(0,1);
         if (currentCursor == 1) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(0,2);
         if (currentCursor == 2) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(0,3);
         if (currentCursor == 3) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(16,1);
         if (currentCursor == 4) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(17,2);
         if (currentCursor == 5) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(16,3);
         if (currentCursor == 6) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
       break;
       case 5:
         lcd.setCursor(0,1);
         if (currentCursor == 1) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(0,2);
         if (currentCursor == 2) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(0,3);
         if (currentCursor == 3) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(17,1);
         if (currentCursor == 4) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(17,2);
         if (currentCursor == 5) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
         lcd.setCursor(17,3);
         if (currentCursor == 6) {
           lcd.write(2);
         } else {
           lcd.print(" ");
         }
       break;
    }
    changeCursor = 0;
  }
}
///////////////////////////////////////////////////////////////
void handleButton() // Nhận và xử lí tín hiệu từ nút nhấn
{
  upValue = Wire.read();
  downValue = Wire.read();
  selectValue = Wire.read();
  backValue = Wire.read();
  /////////////////////////////////////////////////////////////////////////////
  if (upValue == 0) // Nút lên
  {
    if (currentCursor > 1 && currentCursor <= 3) // Khi đang ở hàng trỏ 1
    {
      changeCursor=1;
      currentCursor--;
    } else if (currentPage == 2) // Trang 2
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
          changeValue=1;
        break;
        case 5: if (servo1 < 9 && servo1 < servo2)
        {
          servo1++;
          changeValue=1;
        }
        break;
        case 7: if (servo2 < 9)
        {
          servo2++;
          changeValue=1;
        }
        break;
        case 6: if (servoSpeed < 50)
        {
          servoSpeed+=5;
          changeValue=1;
        }
        break;
      }
    } else if (currentPage == 3) // Trang 3
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
          changeValue=1;
        break;
        case 6: if (powerValue < 3)
        {
          powerValue++;
          changeValue=1;
        }
        break;
      }
    } else if (currentPage == 4) // Trang 4
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
          changeValue=1;
        break;
        case 5: if (pumpConfig < 90)
        {
          pumpConfig++;
          changeValue=1;
        }
        break;
        case 6: 
          pumpValue=!pumpValue;
          changeValue=1;
        break;
      }
    } else if (currentPage == 5) // Trang 5
    {
      switch (currentCursor)
      {
        case 4: if (powerConfig1 < 60)
        {
          powerConfig1++;
          changeValue=1;
        }
        break;
        case 5: if (powerConfig2 < 60 && powerConfig2+1 < powerConfig1)
        {
          powerConfig2++;
          changeValue=1;
        }
        break;
        case 6: if (powerConfig3 < 60 && powerConfig3+1 < powerConfig2)
        {
          powerConfig3++;
          changeValue=1;
        }
        break;
      }
    }
  } else
  ////////////////////////////////////////////////////////////////////////////
  if (downValue == 0) // Nút xuống
  {
    if (currentCursor < 3 && currentCursor >= 1) // Khi đang ở hàng trỏ 1
    {
      changeCursor=1;
      currentCursor++;
    } else if (currentPage == 2) // Trang 2
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
          changeValue=1;
        break;
        case 5: if (servo1 > 1)
        {
          servo1--;
          changeValue=1;
        }
        break;
        case 7: if (servo2 > 1 && servo2 > servo1)
        {
          servo2--;
          changeValue=1;
        }
        break;
        case 6: if (servoSpeed > 10)
        {
          servoSpeed-=5;
          changeValue=1;
        }
        break;
      }
    } else if (currentPage == 3) // Trang 3
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
          changeValue=1;
        break;
        case 6: if (powerValue > 0)
        {
          powerValue--;
          changeValue=1;
        }
        break;
      } 
    } else if (currentPage == 4) // Trang 4
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
          changeValue=1;
        break;
        case 5: if (pumpConfig > 20)
        {
          pumpConfig--;
          changeValue=1;
        }
        break;
        case 6: 
          pumpValue=!pumpValue;
          changeValue=1;
        break;
      }
    } else if (currentPage == 5) // Trang 5
    {
      switch (currentCursor)
      {
        case 4: if (powerConfig1 > 0 && powerConfig1-1 > powerConfig2)
        {
          powerConfig1--;
          changeValue=1;
        }
        break;
        case 5: if (powerConfig2 > 0 && powerConfig2-1 > powerConfig3)
        {
          powerConfig2--;
          changeValue=1;
        }
        break;
        case 6: if (powerConfig3 > 0)
        {
          powerConfig3--;
          changeValue=1;
        }
        break;
      }
    }
  } else
  ////////////////////////////////////////////////////////////////////////////
  if (selectValue == 0) // Nút chọn
  {
    if (currentPage == 1) // Nếu ở trang 1: chuyển trang
    {
      changePage=1;
      changeCursor=1;
      currentPage=currentCursor+1;
      currentCursor=1;
    } else if (currentCursor == 2 && currentPage == 3) // Nếu ở trang 3 - trỏ 2 => Chuyển sang trang 5
    {
      changePage=1;
      currentPage=5;
      changeCursor=1;
      currentCursor=1;
    } else if (currentCursor >= 1 && currentCursor <= 3) // Chuyển sang hàng 2
    {
      changeCursor=1;
      currentCursor+=3;
    } else if (currentCursor == 5 && currentPage == 2) // Nếu ở trang 2 - trỏ 5 => Chuyển trỏ 7
    {
      changeCursor=1;
      currentCursor=7;
    }
  } else
  //////////////////////////////////////////////////////////////////////////////
  if (backValue == 0) // Nút quay lại
  {
    if (currentCursor > 3 && currentCursor <= 6) // Quay về hàng 1
    {
      changeCursor=1;
      currentCursor-=3;
    } else if (currentCursor >=1 && currentCursor <=3 && currentPage == 5) // Quay về trang 3
    {
      changePage=1;
      changeCursor=1;
      currentCursor=2;
      currentPage=3;
    } else if (currentCursor >=1 && currentCursor <=3 && currentPage != 1) // Quay về trang chủ
    {
      changePage=1;
      changeCursor=1;
      currentPage=1;
    } else if (currentCursor == 7)
    {
      changeCursor=1;
      currentCursor=5;
    }
  }
}
/////////////////////////////////////////////////////////
void handleRemote() // Xử lý tín hiệu từ remote
{
  //Serial.println(remote);
  switch (remote)
  {
    case 1: 
      if (servoType == 1)
      {
        servoType = 0;
        buzzValue = 1;
      } else {
        servoType = 1;
        buzzValue = 1;
      }
      changeValue=1;
      EEPROM.write(0,servoType);
    break;
    case 2:
      if (powerType == 1)
      {
        powerType = 0;
        buzzValue = 1;
      } else {
        powerType = 1;
        buzzValue = 1;
      }
      changeValue=1;
      EEPROM.write(5,powerType);
    break;
    case 3:
      if (pumpType == 1)
      {
        pumpType = 0;
        buzzValue = 1;
      } else {
        pumpType = 1;
        buzzValue = 1;
      }
      changeValue=1;
      EEPROM.write(7,pumpType);
    break;
    case 4:
      if (servoSpeed > 10)
      {
        servoSpeed-=5;
        changeValue=1;
        EEPROM.write(4,servoSpeed);
        buzzValue = 1;
      }
    break;
    case 5:
      if (servoSpeed < 50)
      {
        servoSpeed+=5;
        changeValue=1;
        EEPROM.write(4,servoSpeed);
        buzzValue = 1;
      }
    break;
    case 6:
      pumpType=0;
      pumpValue=!pumpValue;
      changeValue=1;
      EEPROM.write(9,pumpValue);
      buzzValue = 1;
    break;
    case 7:
      if (powerValue > 0)
      {
        powerType=0;
        powerValue--;
        changeValue=1;
        EEPROM.write(6,powerValue);
        buzzValue = 1;
      }
    break;
    case 8:
      if (powerValue < 3)
      {
        powerType=0;
        powerValue++;
        changeValue=1;
        EEPROM.write(6,powerValue);
        buzzValue = 1;
      }
    break;
    case 9:
      if (beginSerValue == 0)
      {
        beginSerValue=1; // Bắt đầu lưu
        servoType=0;
        buzzValue = 1;
      }
    break;
    case 13:
    if (beginSerValue == 1)
    {
      servo1=1;
      changeValue=1;
      beginSerValue=2;
      EEPROM.write(2,1);
      buzzValue = 1;
    } else if (beginSerValue == 2 && 1 >= servo1)
    {
      servo2=1;
      changeValue=1;
      beginSerValue=0;
      EEPROM.write(3,1);
      buzzValue = 1;
    }
    break;
    case 14:
    if (beginSerValue == 1)
    {
      servo1=2;
      changeValue=1;
      beginSerValue=2;
      EEPROM.write(2,2);
      buzzValue = 1;
    } else if (beginSerValue == 2 && 2 >= servo1)
    {
      servo2=2;
      changeValue=1;
      beginSerValue=0;
      EEPROM.write(3,2);
      buzzValue = 1;
    }
    break;
    case 15:
    if (beginSerValue == 1)
    {
      servo1=3;
      changeValue=1;
      beginSerValue=2;
      EEPROM.write(2,3);
      buzzValue = 1;
    } else if (beginSerValue == 2 && 3 >= servo1)
    {
      servo2=3;
      changeValue=1;
      beginSerValue=0;
      EEPROM.write(3,3);
      buzzValue = 1;
    }
    break;
    case 16:
    if (beginSerValue == 1)
    {
      servo1=4;
      changeValue=1;
      beginSerValue=2;
      EEPROM.write(2,4);
      buzzValue = 1;
    } else if (beginSerValue == 2 && 4 >= servo1)
    {
      servo2=4;
      changeValue=1;
      beginSerValue=0;
      EEPROM.write(3,4);
      buzzValue = 1;
    }
    break;
    case 17:
    if (beginSerValue == 1)
    {
      servo1=5;
      changeValue=1;
      beginSerValue=2;
      EEPROM.write(2,5);
      buzzValue = 1;
    } else if (beginSerValue == 2 && 5 >= servo1)
    {
      servo2=5;
      changeValue=1;
      beginSerValue=0;
      EEPROM.write(3,5);
      buzzValue = 1;
    }
    break;
    case 18:
    if (beginSerValue == 1)
    {
      servo1=6;
      changeValue=1;
      beginSerValue=2;
      EEPROM.write(2,6);
      buzzValue = 1;
    } else if (beginSerValue == 2 && 6 >= servo1)
    {
      servo2=6;
      changeValue=1;
      beginSerValue=0;
      EEPROM.write(3,6);
      buzzValue = 1;
    }
    break;
    case 19:
    if (beginSerValue == 1)
    {
      servo1=7;
      changeValue=1;
      beginSerValue=2;
      EEPROM.write(2,7);
      buzzValue = 1;
    } else if (beginSerValue == 2 && 7 >= servo1)
    {
      servo2=7;
      changeValue=1;
      beginSerValue=0;
      EEPROM.write(3,7);
      buzzValue = 1;
    }
    break;
    case 20:
    if (beginSerValue == 1)
    {
      servo1=8;
      changeValue=1;
      beginSerValue=2;
      EEPROM.write(2,8);
      buzzValue = 1;
    } else if (beginSerValue == 2 && 8 >= servo1)
    {
      servo2=8;
      changeValue=1;
      beginSerValue=0;
      EEPROM.write(3,8);
      buzzValue = 1;
    }
    break;
    case 21:
    if (beginSerValue == 1)
    {
      servo1=9;
      changeValue=1;
      beginSerValue=2;
      EEPROM.write(2,9);
      buzzValue = 1;
    } else if (beginSerValue == 2 && 9 >= servo1)
    {
      servo2=9;
      changeValue=1;
      beginSerValue=0;
      EEPROM.write(3,9);
      buzzValue = 1;
    }
    break;
  }
}
///////////////////////////////////////////////////////////////
void loop() 
{
  handleLcd();
  /////////////////////////////////////////////////////////////
  if ((unsigned long)(millis()-time1)>150) // Nhận tín hiệu từ arduino
  {
    Wire.requestFrom(ardAddr,7);
    if (Wire.available()>0)
    {
      type = Wire.read();
      if (type == 1)
      {
        tem = Wire.read();
        hum = Wire.read();
        if (checkTemHum() == true)
        {
          handlePower();
          handlePump();
          handlePage1();
        }
      } else if (type == 2)
      { 
        remote = Wire.read();
        handleRemote();
      }
      handleButton();
    }
    time1 = millis();
  }
  //////////////////////////////////////////////////////////////
  if ((unsigned long)(millis()-time2)>1000) // Gửi tín hiệu để điều khiển servo
  {
    pirValue1=digitalRead(pirPin1);
    pirValue2=digitalRead(pirPin2);
    pirValue3=digitalRead(pirPin3);
    pirValue4=digitalRead(pirPin4);
    pirValue5=digitalRead(pirPin5);
    handleServo();
    Wire.beginTransmission(ardAddr);
    I2C_writeAnything(ser1);
    I2C_writeAnything(ser2);
    I2C_writeAnything(servoSpeed);
    Wire.endTransmission();
    time2 = millis();
  }
  ////////////////////////////////////////////////////////////
  if ((unsigned long)(millis()-time3)>50) // Điều khiển còi
  {
    if (buzzValue == 1)
    {
      analogWrite(buzzPin, 0);
      buzzValue = 0;
    } else {
      analogWrite(buzzPin, 255);
    }
    time3 = millis();
  }
}
