// SLAVE ver.2
// sketch de inspiratie de la http://www.roroid.ro/comunicarea-i2c/
// sketch adaptat pentru led multicolor de niq_ro de la http://nicuflorica.blogspot.ro/
// si http://arduinotehniq.blogspot.com/

// Arduino - LoRa rx -> ESP8266 -> Thingspeak
// https://electronics-project-hub.com/send-data-to-thingspeak-using-esp8266/


#include <SoftwareSerial.h>
#include <stdlib.h>

#include <SPI.h>
#include <LoRa.h>

SoftwareSerial ser(8, 7); // RX, TX

char comanda[10]; //vectorul in care va fi receptionata comanda pe I2c
char *i;
String p="";

int semn;
int temp;
int vibr;
int vibr1;
int apa;
float te;

const int NUMBER_OF_FIELDS = 3; // how many comma separated fields we expect
int fieldIndex = 0;            // the current field being received
int values[NUMBER_OF_FIELDS];   // array holding values for all the fields

unsigned long tptrimitere;
unsigned long tpreconectare = 30000;

void setup()
{
 Serial.begin(115200);
  while (!Serial);

// enable software serial
  ser.begin(115200);
  
  Serial.println("Receptor LoRa");

  if (!LoRa.begin(433E6)) {
    Serial.println("Pornire LoRa esuata!");
    while (1);
  }

}

void loop()
{
 // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Pachete receptionate '");

byte index = 0;
    // read packet
    while (LoRa.available()) {
//      Serial.print((char)LoRa.read());
 // if( Serial.available())
 // {
    char ch = LoRa.read();
    if(ch >= '0' && ch <= '9') // is this an ascii digit between 0 and 9?
    {
      // yes, accumulate the value
      values[fieldIndex] = (values[fieldIndex] * 10) + (ch - '0'); 
    }
    else if (ch == ',')  // comma is our separator, so move on to the next field
    {
      if(fieldIndex < NUMBER_OF_FIELDS-1)
        fieldIndex++;   // increment field index
    }
    else
    {
      // any character not a digit or comma ends the acquisition of fields
      // in this example it's the newline character sent by the Serial Monitor
      Serial.print( fieldIndex +1);            
      Serial.println(" fields received:");
    //  for(int i=0; i <= fieldIndex; i++)
    //  {
        temp = values[0];
        vibr = values[1];
        apa = values[2];
        Serial.print("temperatura = ");
        Serial.print(temp);
        Serial.print(" grade Celsius, vibratii = ");
        Serial.print(vibr);
        Serial.print(", nivelul apei = ");
        Serial.print(apa);
        Serial.println(" # ");
        values[0]=0;
        values[1]=0;
        values[2]=0;

      fieldIndex = 0;  // ready to start over
    }
  }

semn = vibr/10;
vibr1 = vibr%10;
if (semn == 1)
{
  te = -temp/10.;
}
else
{
  te = temp/10.;
}

Serial.println(" ----------------------- ");
Serial.print("temperatura = ");
Serial.print(te);
Serial.print(" grade Celsius, vibratii = ");
Serial.print(vibr1);
Serial.print(", nivelul apei = ");
Serial.print(apa);
Serial.println("% ----------------------- ");
 
int a0 = temp/100; 
int a12 = temp%100;
int a1 = a12/10;
int a2 = a12%10;
int a3 = vibr/10;
int a4 = vibr%10;
int a5 = apa/100;
int a67 = apa%100;
int a6 = a67/10;
int a7 = a67%10;

Serial.print("--------------");
Serial.print(a0);
Serial.print(a1);
Serial.print(a2);
Serial.print(a3);
Serial.print(a4);
Serial.print(a5);
Serial.print(a6);
Serial.print(a7);
Serial.print("--------------");

//------Sending Data to ESP8266--------//
    ser.print('*'); // Starting char
    ser.print(a0); 
    ser.print(a1); 
    ser.print(a2); 
    ser.print(a3); 
    ser.print(a4); 
    ser.print(a5); 
    ser.print(a6); 
    ser.print(a7); 
    ser.println('#'); // Ending char
    //------------------------------------//

 }


} // end mai loop
