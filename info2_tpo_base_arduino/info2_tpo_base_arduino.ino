#include <SoftwareSerial.h>

// ESTADOS == MENSAJES
#define ESPERANDO 127
#define ADELANTE 63
#define ATRAS 126
#define IZQUIERDA 31
#define DERECHA 125
#define ALERTA 62

#define THRE 256

SoftwareSerial swSerial(10, 11); // RX, TX
#define BAUDRATE 38400 //9600 x 4

void setup() {
 
  Serial.begin(BAUDRATE);
  swSerial.begin(BAUDRATE);

}

void loop() // run over and over
{
  int x= analogRead(A0);
  int y= analogRead(A1);
  
  if (x > 512+THRE) { swSerial.write(DERECHA); }
  else if (x < 512-THRE) { swSerial.write(IZQUIERDA); }
  else if (y > 512+THRE) { swSerial.write(ADELANTE); }
  else if (y < 512-THRE) { swSerial.write(ATRAS); }
  else { swSerial.write(ESPERANDO); }
  
  //Serial.println("X="); Serial.println(x);  Serial.print('\n');
  //Serial.println("Y="); Serial.println(y);  Serial.print('\n');
  
  if (swSerial.available())
    Serial.write(swSerial.read());
    
  delay(0.1);
}

