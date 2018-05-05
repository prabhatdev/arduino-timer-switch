 #include <Keypad.h>
 #include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
int i=0,flag=0,set=0,count=0,button=0,clr=0,sum=0,input,backl=0;
char k[10],key;
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'.','0','#','D'}
};
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
double start=0,last=0,timers=0,timerm=0,settime=14400;
int relay=11,buzzer=12;
void setup() {
  // put your setup code here, to run once:
lcd.begin (16,2); //  <<----- My LCD was 16x2
// Switch on the backlight
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
lcd.setBacklight(HIGH);
lcd.home ();
lcd.print("TIME     SET");
Serial.begin(9600);
pinMode(relay,OUTPUT);
pinMode(LED_BUILTIN, OUTPUT);
pinMode(buzzer,OUTPUT);
 //lcd.setBacklight(LOW);

}
void beep()
{
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
}
void loop() 
{
  char key = keypad.getKey();
    if(key!=NO_KEY)
    { beep();
      if(key=='A')
        set++;
      if(key=='C')
        button++;
      clr=0;
      if(key=='*')
        backl++;
      while(button%2==1)
      { if(clr==0)
        {
          lcd.clear();
          clr++;
        }
          lcd.home();
          lcd.print("SWITCH ON");
          digitalWrite(relay, HIGH);
          key= keypad.getKey();
          if(key=='C')
          {
            button++;
            lcd.home();
            lcd.print("SWITCH OFF");
            delay(500);
          }
      }
      if(button%2==0)
      {
      digitalWrite(relay, LOW);
      }
      if(set%2==1)
        digitalWrite(LED_BUILTIN, HIGH); 
      else
        digitalWrite(LED_BUILTIN, LOW);
      clr=0;
      sum=0;
      while(set%2==1)
        {
         if(clr==0)
         {
          lcd.clear();
          clr++;
         }
          lcd.setCursor (0,0);
          lcd.print("Enter time:"); 
          //Serial.println(set);
          key = keypad.getKey();   
          if(key!=NO_KEY)
            beep();
          if(key=='A')
            {
              set++;
            }
          if(key=='B')
            {
              count++;
              lcd.setCursor (0,1);
              lcd.print(count);
              lcd.print("hours"); 
              flag=1;
            }
          if(set%2==1)
            digitalWrite(LED_BUILTIN, HIGH); 
          else
            digitalWrite(LED_BUILTIN, LOW);
            
          if(key!=NO_KEY)
            {  
              if(key=='D'&&set%2==1)
                {
                  k[i]='\0';
                  i=0;
                  if(flag==1)
                  {
                    settime=count;
                    settime=settime*3600;
                    count=0;
                    flag=0;
                  }
                  else
                  {
                  settime=atof(k);
                  
                  settime=settime*60;
                  }
                  
                  last=millis();
                  set++;
                }
          else if(set%2==1&&key!='A'&&key!='B'&&key!='C'&&key!='#')
            {
              k[i++]=key;
              input=k[i-1]-48;
              sum=sum*pow(10,i-1)+input;
              lcd.setCursor(0,1);
              lcd.print(sum);
              lcd.print(" minutes");
              }
            }
        } 
    }
    
  start=millis()-last;
  timers=start/1000;
  
  lcd.setCursor(0,0);
  lcd.print("TIME     SET");
  lcd.setCursor (0,1);
  lcd.print(timers,0);
  lcd.print("      ");
  lcd.print(settime,0);
  
  if(timers<settime)
    {
   
    digitalWrite(relay,HIGH);
    //delay(1000);
    }
  
   if(timers>=settime)
    {
    digitalWrite(relay,LOW);
    Serial.println("0ff");
   // delay(1000);
    }
}
