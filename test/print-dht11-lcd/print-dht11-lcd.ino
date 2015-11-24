#include <dht.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define DHTPIN 2
dht DHT;
LiquidCrystal_I2C lcd(0x27,20,4);
byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, degree);
}
void loop() {
  int chk = DHT.read11(DHTPIN);
  lcd.setCursor(0,0);
  lcd.print("Nhiet Do: ");
  lcd.setCursor(10,0);
  lcd.print(DHT.temperature,1);
  lcd.write(1);
  lcd.print("C");
  /////////////////////////////////
  lcd.setCursor(0,1);
  lcd.print("Do Am: ");
  lcd.setCursor(10,1);
  lcd.print(DHT.humidity,1);
  lcd.print("%");
  ////////////////////////////////
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);
  delay(500);
}
