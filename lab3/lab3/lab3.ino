#include <LiquidCrystal.h>

#define DDR_KEYPAD  DDRC
#define PORT_KEYPAD PORTC
#define PIN_KEYPAD  PINC
#include "keypad4x4.h"

const int buzzerPin = 21;


const int rs = 5, rw = 6, en = 7,d0 = 22, d1 = 23, d2 = 24,d3 = 25, d4 = 26, d5 = 27, d6 = 28, d7 = 29;
LiquidCrystal lcd(rs, rw, en,d0,d1,d2,d3,d4,d5,d6,d7);

const PROGMEM  char sixty[60][3] = {
  {"00"}, {"01"}, {"02"}, {"03"}, {"04"}, {"05"}, {"06"}, {"07"}, {"08"}, {"09"},
  {"10"}, {"11"}, {"12"}, {"13"}, {"14"}, {"15"}, {"16"}, {"17"}, {"18"}, {"19"},
  {"20"}, {"21"}, {"22"}, {"23"}, {"24"}, {"25"}, {"26"}, {"27"}, {"28"}, {"29"},
  {"30"}, {"31"}, {"32"}, {"33"}, {"34"}, {"35"}, {"36"}, {"37"}, {"38"}, {"39"},
  {"40"}, {"41"}, {"42"}, {"43"}, {"44"}, {"45"}, {"46"}, {"47"}, {"48"}, {"49"},
  {"50"}, {"51"}, {"52"}, {"53"}, {"54"}, {"55"}, {"56"}, {"57"}, {"58"}, {"59"}
};

struct Time
{
  unsigned char second, minute, hour;
};
Time T4 = {0, 0, 0};

Time arrTime[60];
byte countTime = 0;
boolean saveTime = 0;

boolean startCounting = 0;

boolean modeJournal = 0;

void LCD_WriteStrPROGMEM(char *str, int n)  
{
  for (int i = 0; i < n; i++)
    lcd.print( (char)pgm_read_byte( &(str[i]) ) );
}

ISR(TIMER4_COMPA_vect)  
{
  if (startCounting){
    if (++T4.second == 60)
    {
      T4.second = 0;
      if (++T4.minute == 60)
      {
        T4.minute = 0;
         
        
        if (++T4.hour == 24)
          T4.hour = 0;
      }
    }

  }
  if(!modeJournal){
  lcd.setCursor(3, 0);
  LCD_WriteStrPROGMEM(sixty[T4.hour], 2);
  lcd.write(':');
  LCD_WriteStrPROGMEM(sixty[T4.minute], 2);
  lcd.write(':');
  LCD_WriteStrPROGMEM(sixty[T4.second], 2);
  }
}

void setup() {
  noInterrupts();           // disable all interrupts

  // Таймер#1: Скид при співпадінні OCR1A (1sec) + дільник=256
  TCCR4A = 0x00;
  TCCR4B = (1 << WGM12) | (1 << CS12) | (1 << CS10); //CTC mode & Prescaler @ 1024
  TIMSK4 = (1 << OCIE4A); // дозвіл на переривання по співпадінню
  OCR4A = 0x3D08;// compare value = 1 sec (16MHz AVR)
  
  //KeyPad 4x4
  initKeyPad();

  //LCD 16x2
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  interrupts();  // Enable global interrupts

  //Buzzer
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
   static byte scrollJournal = 1;
   static byte numberJournal = 0;

   static byte lastMinute = 0;
   static byte newMinute = 0;
   newMinute = T4.minute;
   if (lastMinute!= newMinute){
      lastMinute = newMinute;
      buzzerSound(buzzerPin,200);
   }

   
    if ( isButtonPressed() ) {
      lcd.setCursor(0, 1);

      
      switch(readKeyFromPad4x4()){
          if(!modeJournal){
          case 'D':
            startCounting = !startCounting;
            saveTime = 0;
          break;
          
          case 'C':
            startCounting = 0;
            T4 = {0, 0, 0};
            saveTime = 0;
          break;

          case 'F':
            countTime = 0;
            lcd.setCursor(13, 0);
            lcd.write("DEL");
            delay(1000);
            lcd.setCursor(13, 0);
            lcd.write("   ");
          break;
          }
          case 'A':
              lcd.clear();
            if(countTime==0){
               lcd.setCursor(4, 0);
               lcd.write("NOTHING");
               buzzerSound(buzzerPin,1000);
               
            }
            else{
            modeJournal = !modeJournal;
            if(modeJournal){
              setJournal(1,countTime);
              numberJournal = checkJournalNumber(0,0);
              lcd.setCursor(13, 0);
              lcd.write("M");
              LCD_WriteStrPROGMEM(sixty[countTime], 2);    
            }
            else {
              scrollJournal = 0;
              lcd.setCursor(13, 0);
              lcd.write("M");
              LCD_WriteStrPROGMEM(sixty[countTime], 2);     
            }
            }    
          break;

          case 'B':
            if (scrollJournal<countTime){
              scrollJournal++;
              setJournal(scrollJournal,countTime);
            }  
          break;
          case '0': numberJournal = checkJournalNumber(numberJournal,0); break;
          case '1': numberJournal = checkJournalNumber(numberJournal,1); break;
          case '2': numberJournal = checkJournalNumber(numberJournal,2); break;
          case '3': numberJournal = checkJournalNumber(numberJournal,3); break;
          case '4': numberJournal = checkJournalNumber(numberJournal,4); break;
          case '5': numberJournal = checkJournalNumber(numberJournal,5); break;
          case '6': numberJournal = checkJournalNumber(numberJournal,6); break;
          case '7': numberJournal = checkJournalNumber(numberJournal,7); break;
          case '8': numberJournal = checkJournalNumber(numberJournal,8); break;
          case '9': numberJournal = checkJournalNumber(numberJournal,9); break;

          case 'E':
            if(!modeJournal){
              arrTime[countTime] = T4;
              if (countTime<=60)countTime++;
              
              lcd.setCursor(13, 0);
              lcd.write("M");
              LCD_WriteStrPROGMEM(sixty[countTime], 2);
            }
            else{
            if (numberJournal<=countTime && numberJournal!=0){
              scrollJournal= numberJournal;
              setJournal(numberJournal,countTime);
              buzzerSound(buzzerPin,50);
              delay(100);
              buzzerSound(buzzerPin,50);
            }
            else{
               buzzerSound(buzzerPin,500);
               numberJournal = checkJournalNumber(numberJournal,0);
               numberJournal = checkJournalNumber(numberJournal,0);
            }
            }
          break;
          
        }
  
      }
      
    }

byte checkJournalNumber(byte number,byte add){
  byte newNumber = 0;
  if (number>0) newNumber = number*10 + add;
  else if (number == 0) newNumber = add;
  if (newNumber>59){
   
    newNumber = 0;
  }
    lcd.setCursor(13, 1);
      lcd.write("F");
      LCD_WriteStrPROGMEM(sixty[newNumber], 2);   
  return newNumber; 
}

void setJournal(byte number, byte border){
  if (number == 1){
    
    setRowJournal(number,1);
    lcd.setCursor(0, 0);
     lcd.write("           ");
  }
  else if (number<=border){
    setRowJournal(number-1,0);
    setRowJournal(number,1);
  }


}

void buzzerSound(byte pin,int timeDelay){
  TIMSK4 &= ~(1 << OCIE1A);
  digitalWrite(pin,HIGH);
  delay(timeDelay);
  digitalWrite(pin,LOW);
  TIMSK4 = (1 << OCIE4A); 

}
void setRowJournal(byte number, byte row){
  lcd.setCursor(0, row);
  LCD_WriteStrPROGMEM(sixty[number], 2);
  lcd.write('-');
  LCD_WriteStrPROGMEM(sixty[arrTime[number-1].hour], 2);
  lcd.write(':');
  LCD_WriteStrPROGMEM(sixty[arrTime[number-1].minute], 2);
  lcd.write(':');
  LCD_WriteStrPROGMEM(sixty[arrTime[number-1].second], 2);
 }
  
  
 
