#include <SPI.h>
#include "RF24.h"
#include <SoftwareSerial.h>

/* 
Line    Uno    Mega Pin  Modulo
GND            GND         1        NEGRO
3V3            3V3         2        ROJO
CE      7      53          3        BLANCO
CSN     8      48          4        NARANJA
SCK     13     52          5        GRIS
MOSI    11     51          6        AMARILLO
MISO    12     50          7        VIOLETA
*/
  
// COMUNICACION
// serial para debug => Serial.
// serial para comunicacion => swSerial
unsigned int bufferSerie;
SoftwareSerial swSerial(9, 10); // RX, TX
#define BAUDRATE 9600

// nRF24L01 radio en el SPI bus + pines 7 & 8
RF24 radio(7,8); // UNO - Base 
//RF24 radio(53,48); // MEGA - Rover
bool radioNumber = 0;
byte addresses[][6] = {"Base","Rover"};
#define TIMEOUT 20000
bool role = 0;

void setup() {
  Serial.begin(BAUDRATE);
  Serial.println(F("Iniciando..."));
  
  swSerial.begin(BAUDRATE);
  
  radio.begin();

  //Set the PA Level low to prevent power supply related issues since this is a
  //getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  //radio.setPALevel(RF24_PA_LOW);
  
  if (radioNumber) {
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]); }
  else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
  radio.startListening();
}

void loop() {           
  
  /*
  /
  /  envio
  */
  if ( swSerial.available() ) {
      radio.stopListening();                                    // First, stop listening so we can talk.
          
      Serial.println(F("Hay datos en el swSerial. Iniciando reenvio por la radio!"));

      while ( swSerial.available() ) {
        bufferSerie = swSerial.read();
        Serial.print(bufferSerie);
        if (!radio.write(&bufferSerie, sizeof(bufferSerie)) ) {
          Serial.println(F("El envio fallo")); }
      }
      
      Serial.println(F("El envio finalizo."));
  }
        
  /*
  /
  /  recibo
  */
  radio.startListening();                                    // Now, continue listening
    
  unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
  boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
  while ( ! radio.available() ) {                             // While nothing is received
    if (micros() - started_waiting_at > TIMEOUT ){            // If waited longer than 200ms, indicate timeout and exit while loop
        timeout = true;
        break; }      
  }
        
  if ( ! timeout ) {
      if ( radio.available() ) {
        Serial.println(F("Hay datos en la radio. Iniciando reenvio por el swSerial!"));
        
        while ( radio.available() ) {
          radio.read( &bufferSerie, sizeof(bufferSerie) );        
          swSerial.write(bufferSerie);

          Serial.print(F("Obtuve: "));
          Serial.println(bufferSerie);
        }
      }
  }
 
} // Loop

