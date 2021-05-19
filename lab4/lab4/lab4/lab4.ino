
#define NUM_DIGIT 6

#define PORT_CONTROL_SEGMENT PORTA
#define DDR_CONTROL_SEGMENT DDRA

#define PORT_CONTROL_VOLTAGE PORTC
#define DDR_CONTROL_VOLTAGE DDRC

#define PORT_BUTTONS PORTB
#define DDR_BUTTONS DDRB
#define PIN_BUTTONS PINB

#define BUZZER 20


struct Time
{
  unsigned char second, minute, hour;
};
Time T4 = {0, 0, 0};
String timeString = "";

boolean startCounting = 0;

Time arrTime[10];
byte countTime = 0;

boolean journalMode = 0;

void timeToString(Time pattern){
  timeString = pattern.hour>9 ? String(pattern.hour) : '0' + String(pattern.hour) + '.'; 
  timeString += pattern.minute>9 ? String(pattern.minute) : '0' + String(pattern.minute) + '.';
  timeString += pattern.second>9 ? String(pattern.second) : '0' + String(pattern.second); 
}

ISR(TIMER1_COMPA_vect)  
{
  if(!journalMode){
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

  timeToString(T4);
}
}




const byte segmentNumber[10] = {
  0b00111111,
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01101111,
};

void setup() {

   noInterrupts();           

  TCCR1A = 0x00;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
  TIMSK1 = (1 << OCIE1A);
  OCR1A =  0x3D08;


  interrupts();  // Enable global interrupts

  
  DDR_CONTROL_VOLTAGE = 0b11111111;
  PORT_CONTROL_VOLTAGE = 0b00000000;
  
  DDR_CONTROL_SEGMENT = 0b11111111;
  PORT_CONTROL_SEGMENT  = 0b00000000;

  DDR_BUTTONS = 0x0F;
  PORT_BUTTONS = 0xF0;

  Serial.begin(9600);
  pinMode(53,INPUT_PULLUP);
  pinMode(52,INPUT_PULLUP);
  pinMode(51,INPUT_PULLUP);
  pinMode(50,INPUT_PULLUP);

  pinMode(BUZZER,OUTPUT);
}

void loop() {
 
  showSegment(timeString);

  if(T4.second == 59) digitalWrite(BUZZER,HIGH);
  else if (T4.second == 0)digitalWrite(BUZZER,LOW);
   


  static byte postDown = 2;

  static byte journalScroll = 0;
 
  
  static byte pinButtonsOld = PIN_BUTTONS;
  byte pinButtonsNow = PIN_BUTTONS;

  if(pinButtonsNow == 255){
    
    if (pinButtonsOld == 0b11111110){
      if (journalMode)journalMode = 0;
      else {
        if (postDown<2)postDown++;
        else postDown = 0;
    
        if (postDown==0)startCounting = 1;
        else if (postDown==1)startCounting = 0;
        else T4 = {0, 0, 0};
      }    
   }
   else if (pinButtonsOld == 0b11111101 && !journalMode){ 
      arrTime[countTime] = T4;
      if(countTime<10)countTime++;
   }
   else if (pinButtonsOld == 0b11111011 && !journalMode){
      countTime=0;
   }
   else if (pinButtonsOld == 0b11110111){
      
      if (startCounting == 0 && countTime>0)journalMode = 1;
      if (journalMode){
      timeToString(arrTime[journalScroll]);
      
        if(journalScroll<countTime-1)journalScroll++;
        else journalScroll = 0;
      
      }
   }    
 }
 
pinButtonsOld = pinButtonsNow;

}


void showSegment(String value){
 
 String clearValue = value;
 
 byte pointRow[6]={0,0,0,0,0,0};
 
 
 while(true){
  int index = clearValue.indexOf(".");
  if (index == -1) break;
  clearValue.remove(index,1);
  pointRow[index-1] = 0b10000000; 
 }
 int clearValueLength = clearValue.length();
 if (clearValueLength > NUM_DIGIT+1) return;
 

 
 
  PORT_CONTROL_VOLTAGE = 0b01111111;

 for (int i = 0; i<clearValueLength; i++){ 
      PORT_CONTROL_SEGMENT = segmentNumber[clearValue[clearValueLength-i-1] - '0'] | pointRow[clearValueLength-i-1];
      delay(1);
      PORT_CONTROL_VOLTAGE = (PORT_CONTROL_VOLTAGE>>1) ^ 0b10000000;
 } 

 
}
