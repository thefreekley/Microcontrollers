#define BAUD 14400
#define MODE 2
#define TRANSMIT HIGH
#define RECEIVE LOW
#define ID 87
#define DHT_PIN 3

char symbol;
int i = 0;
char* bufferPtr;



#include "CRC8.h"
#include "CRC.h"


#include <iarduino_DHT.h>   
iarduino_DHT sensor(DHT_PIN);



CRC8 crc;

void setup(void) {

  Serial.begin(BAUD);
  pinMode(MODE, OUTPUT); 
  digitalWrite(MODE, RECEIVE);
    

    
}

char message[21];

void loop() {
 sensor.read();
 
 delay(1000);
  i = 0;
  symbol = 0;
  if (Serial.available()) {
    bufferPtr = (char*)malloc(50 * sizeof(char));
    delay(5);
    while (Serial.available()) {
      symbol = Serial.read();
      if (symbol == ';') {
        bufferPtr[i++] = '\0';
        i = 0;
        int c;
        sscanf(bufferPtr, "%d", &c);
        if (c != ID) {
          while (Serial.available()){Serial.read();}
          free(bufferPtr);
          return;
        }
      } else {
        bufferPtr[i++] = symbol;
      }
    }
    if (i > 0){
      bufferPtr[i++] = '\0';
    }
  }
  if (i > 0) {
    if (!strcmp(bufferPtr, "Give")) {
      
//      String resultData = "Hum-" + String(sensor.hum) + " Tem-" + String(sensor.tem) + " C";
//      resultData.toCharArray(message,21);

      static const char messageHum[] = " Hum-";
      static const char messageTem[] = " Tem-";
      
      char message2[5];
      
      
      sendMessage(1, messageHum, sizeof(messageHum));

      dtostrf(sensor.hum,4,1,message2);
      sendMessage(1, message2, sizeof(message2));
      
      sendMessage(1, messageTem, sizeof(messageTem));

      dtostrf(sensor.tem,4,1,message2);
      sendMessage(1, message2, sizeof(message2));
      
     
    }
  }
  free(bufferPtr);
  //delay(200);
}



void sendMessage(int recevierId, char str[], long sizeOf) {
  char buffer[20];
  digitalWrite(MODE, TRANSMIT);

  
  long crc = crc8((uint8_t *)str,sizeOf-1,0x7); 
  
  delay(60); 
  sprintf(buffer, "%d;%d:%lx!%s*", recevierId, ID, crc, str);
  Serial.print(buffer); // Без спотворень
  delay(60);
  digitalWrite(MODE, RECEIVE);
}




     
