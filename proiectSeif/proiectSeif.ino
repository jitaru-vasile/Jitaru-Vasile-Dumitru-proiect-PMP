#include <Keypad.h>
#include<LiquidCrystal.h>
#include <Servo.h>
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD "123456789"
#define REMOTEXY_SERVER_PORT 6377

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,1,0,0,0,20,0,10,43,1,
  2,1,19,50,22,11,15,8,31,31,
  79,78,0,79,70,70,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t led; // =1 if switch ON and =0 if OFF 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;

#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_LED 7
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
int buzzer = 40;


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
  RemoteXY_Init (); 
  
  pinMode (PIN_LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  noTone(buzzer);//Set buzzerPin as output
 
  col = 0;
  lcd.begin(16, 2); 
  //Serial.begin(9600);
   servo.attach(38);
  servo.write(angle);
}
 
void loop(){
   RemoteXY_Handler ();
  digitalWrite(PIN_LED, (RemoteXY.led==0)?LOW:HIGH);
    unsigned char i, j ;
  noTone(buzzer);
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
       if(RemoteXY.led==0)
      {
         lcd.setCursor(0,0);
         lcd.print("SE ASTEAPTA");
        
      }else
      
        verificaParola();
      }
}
