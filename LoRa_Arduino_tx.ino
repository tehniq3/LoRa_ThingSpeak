// MASTER ver2
// sketch de inspiratie de la http://www.roroid.ro/comunicarea-i2c/
// sketch adaptat pentru led multicolor de niq_ro de la http://nicuflorica.blogspot.ro/
// si http://arduinotehniq.blogspot.com/
//se incarca biblioteca pentru comunicarea pe I2C


// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

#include <SPI.h>
#include <LoRa.h>

#define pinds 3
#define pinapa A0
#define pinvibr 4



String ghe1;

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS pinds
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

float te; // temperature
byte semn;
int te1;  // int = 10 x temperature
byte tez, teu, tes;
int terest; 

byte vibr;

int apa1;
byte apa;
int aparest;
byte apas, apaz, apau;

void setup()
{
  Serial.begin(115200);
  while (!Serial);

// Start up the library
  sensors.begin();

pinMode(pinvibr, INPUT);
pinMode(pinapa, INPUT);

  Serial.println("Emitator LoRa");

  if (!LoRa.begin(433E6)) {
    Serial.println("Pornire LoRa esuata!");
    while (1);
  }
}

void loop()
{
 // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Solicita temperatura...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("GATA");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.print("Temperatura pentru senzorul 1 (index 0) este: ");
  te = sensors.getTempCByIndex(0);
  Serial.println(te);  

/*
te = -23.4 degree Celsius
te = 34.4  semn = 1
te1 = 10*23.4 = 234
tez = 2
teu = 3
tes = 4
*/

if (te < 0)
{
  te = -te;
  semn = 1;
}
else
  semn = 0; 
  
te1 = 10*te;  // multiply for be integar
/*
tez = te1/100;
terest = te1 - 100*tez;
teu = terest/10;
tes = terest - 10*teu;
*/

// vibration
vibr = digitalRead(pinvibr);

// waterlevel
apa1 = analogRead(pinapa);
apa = map(apa1, 0, 1023, 0, 100);
/*
apas = apa/100;
aparest = apa - 100*apas;
apaz = aparest / 10;
apau = aparest % 10;
*/

// send data; x12341098;
  Serial.println("Trimite date.. ");
  //Serial.print('H'); // unique header to identify start of message
  //Serial.print(",");
  Serial.print(te1,DEC);
  Serial.print(",");
  Serial.print(10*semn+vibr,DEC);
  Serial.print(",");
  Serial.print(apa,DEC);
  Serial.print(",");  // note that a comma is sent after the last field
  Serial.println();  // send a cr/lf
  delay(100);
  
LoRa.beginPacket();

  //LoRa.print('H'); // unique header to identify start of message
  //LoRa.print(",");
  LoRa.print(te1,DEC);
  LoRa.print(",");
  LoRa.print(10*semn + vibr,DEC);
  LoRa.print(",");
  LoRa.print(apa,DEC);
  LoRa.print(",");  // note that a comma is sent after the last field
  LoRa.print(" ");  // send a cr/lf
  delay(100);
  
LoRa.endPacket();
 
  
delay(3000);

}  // end main looop
