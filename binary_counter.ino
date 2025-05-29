#include<LiquidCrystal.h>
int rs=7,en=8,d4=9,d5=10,d6=11,d7=12;
byte bin=0b00000000;

LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
void setup()
{
  lcd.begin(16,2);
}
void loop()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Number:");
  lcd.print(bin);
  lcd.setCursor(0,1);
  lcd.print(bin,BIN);
  if(bin==0b11111111)
  {
    lcd.setCursor(8,1);
  	lcd.print("Bit !!");
  	delay(2000);
  }
  bin=bin+1;   
  delay(500);  
}
