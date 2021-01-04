#include <Keypad.h>
#include<LiquidCrystal.h>
#include <Servo.h>
// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT
#define REMOTEXY_DEBUGLOGS Serial
#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD ""
#define REMOTEXY_SERVER_PORT 6377
#define PIN_LED 7

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,6,0,0,0,75,0,10,141,0,
  7,37,7,37,20,5,8,16,36,6,
  129,0,6,4,18,6,8,83,97,102,
  101,66,111,120,32,0,129,0,6,29,
  76,4,8,84,121,112,101,32,116,104,
  101,32,112,97,115,115,75,101,121,32,
  116,111,32,97,99,99,101,115,115,32,
  116,104,101,32,115,97,102,101,66,111,
  120,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  char passKey[6];  // string UTF8 end zero  

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

#define PIEZO   46
#define NOTE_G4  392
#define NOTE_C5  523
#define NOTE_G5  784
#define NOTE_C6  1047
int OpenMelody[] = {NOTE_G5, NOTE_C6};
int OpenNoteDurations[] = {12, 8};

int CloseMelody[] = {NOTE_C6, NOTE_G5};
int CloseNoteDurations[] = {12, 8};

#define playOpenMelody() playMelody(OpenMelody, OpenNoteDurations, 2)
#define playCloseMelody() playMelody(CloseMelody, CloseNoteDurations, 2)

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
char masterPass[6] = {'1','3','5','7','9'};
char mainPass[6] = {'1','2','3','4','5'};
byte rowPins[ROWS] = {22, 24, 26, 28}; 
byte colPins[COLS] = {30, 32, 34, 36};  
char pass[6]={'0','0','0','0','0'};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
Servo myservo;
int angle = 10;
char passW[6] ={'1','2','3','4','5'};
int permission = 0;

//Daca se apasa "A" -> backspace
//Daca se apasa "B" pe tastatura atunci se va putea seta parola care trebuie introdusa
//Daca se apasa "C" pe tastatura atunci se va putea introduce sau reintroduce parola in cazul in care aceasta este gresita
//Daca se apasa "D" pe tastatura atunci se va putea introduce Master-Cod-ul cu care se va putea deschide seiful chiar daca nu se stie parola in sine

void setup() 
{
  RemoteXY_Init (); 
  pinMode(PIEZO, OUTPUT); 
  pinMode (PIN_LED, OUTPUT);
  //Serial.begin(115200);
  col = 0;
  lcd.begin(16, 2); 
  myservo.attach(38);
  ServoClose();
 
  
}

void playMelody(int *melody, int *noteDurations, int notesLength)
{
  pinMode(PIEZO, OUTPUT);

  for (int thisNote = 0; thisNote < notesLength; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(PIEZO, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(PIEZO);
  }
}

void checkWifiPass(){
  permission = 1;
  for(int i = 0; i < 5; i++){
    if(RemoteXY.passKey[i] != passW[i])
      permission = 0;
      break;
  }
}

void ServoOpen()
{
  for (int pos = 90; pos >= 0; pos -= 5) {
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void ServoClose()
{
  for (int pos = 0; pos <= 90; pos += 5) {
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void backSpace(){
  col--;
  lcd.setCursor(col,0);
  lcd.print(" ");
}

void backSpace1(int i){
  i--;
  lcd.setCursor(i,0);
  lcd.print(" ");
}

void verificaParola()
{
  int ok = 1;
  for(int i=0;i<=5;i++)
    if(pass[i] != mainPass[i] ){
      lcd.setCursor(0,0);
      lcd.print("PAROLA GRESITA");
      lcd.setCursor(1,0);
      lcd.print("PRESS C TO RETRY");
      ok = 0;
      playCloseMelody();
      
      break;
    }
  if (ok == 1){ 
      while(permission==0){
        lcd.setCursor(0,0);
        lcd.print("SE ASTEAPTA");
        delay(400);  
      }
      clearLCD();
      lcd.setCursor(0,0);
      lcd.print("PAROLA CORECTA");
      ServoOpen();
      playOpenMelody();
      delay(400);
      clearLCD();
  }
}

void clearLCD(){
  lcd.setCursor(col,0);
  lcd.print("                ");
  lcd.setCursor(col,1);
  lcd.print("                ");  
}

void clearLCD1(){
  lcd.setCursor(1,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");  
}

void typePassword(){
  col = 0;
  clearLCD();
  lcd.setCursor(col,0);
  lcd.print("Intruduceti");
  lcd.setCursor(col,1);
  lcd.print("parola");
  for(int i = 0; i<5 ;i ++){  
     col = i;
     lcd.setCursor(i,0);
    
    char key = customKeypad.getKey();
    if (key){
       pass[i] = key;
       if(pass[i] == 'A'){
         backSpace1(i);
         i=i-2;
       }
       else{
           lcd.print(pass[i]); 
           if(i == 0){
                clearLCD1();
            }  
           delay(400);
           lcd.setCursor(i,0);
           lcd.print('*');
       }
   }else
    i--;
  }
  col++;
  verificaParola();
}
int contor = 0;
void loop() 
{ 
  RemoteXY_Handler ();
  if(RemoteXY.connect_flag == 1){
     
     checkWifiPass();
  }
  digitalWrite(PIN_LED, (RemoteXY.connect_flag==0)?LOW:HIGH);
  if(permission == 0){
    lcd.setCursor(0,0);
    lcd.print("SE ASTEAPTA");
    lcd.setCursor(0,1);
    lcd.print("PAROLA WIfI");
    delay(400);
  }
  else{
    contor++;
    if(contor == 1){
      lcd.setCursor(0,0);
      lcd.print("PAROLA WIFI");
      lcd.setCursor(0,1);
      lcd.print("ACCEPTATA  ");
      delay(400);
    }
    char customKey = customKeypad.getKey();
    if(customKey == 'C'){
       typePassword();
    }
    if(customKey == 'B'){
       ServoClose();
    }
    
  }
}
