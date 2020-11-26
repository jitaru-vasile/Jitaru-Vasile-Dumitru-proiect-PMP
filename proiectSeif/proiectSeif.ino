#include <Keypad.h>
#include<LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const byte ROWS = 4; 
const byte COLS = 4; 
int col = 0;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
char parolaIntrodusa[6];
byte rowPins[ROWS] = {22, 24, 26, 28}; 
byte colPins[COLS] = {30, 32, 34, 36};  
char parolaActuala[6]={'1','2','3','4','5'};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
Servo servo;
int angle = 10;
void verificaParola()
{
  int ok = 1;
  for(int i=0;i<=5;i++)
    if(parolaIntrodusa[i] != parolaActuala[i] )
    {
      lcd.setCursor(0,0);
      lcd.print("PAROLA GRESITA");
      ok = 0;
      break;
     }
  if (ok == 1)
  {
      lcd.setCursor(0,0);
      lcd.print("PAROLA CORECTA");
      servo.write(angle+90);
    }
}
void setup(){
  col = 0;
  lcd.begin(16, 2); 
  //Serial.begin(9600);
   servo.attach(38);
  servo.write(angle);
}
  
void loop(){
  char customKey = customKeypad.getKey();
   if(customKey == 'A')
      {
         setup();
        }else
  if (col != 5)
    { 
      if (customKey){
        parolaIntrodusa[col] = customKey;
        lcd.setCursor(col,0);
        lcd.print(customKey);
        delay(400);
        lcd.setCursor(col,0);
        lcd.print('*');
        col++;
     }
    }
    else
    {
      verificaParola();
      }
}
