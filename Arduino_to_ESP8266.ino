//-------------Electronics-project-hub>com-------------//
// https://electronics-project-hub.com/send-data-to-thingspeak-using-esp8266/

#include <ESP8266WiFi.h>
//--------------Your Data----------------//
String apiKey = "API_keyL"; // <<<<< YOUR API KEY
const char* ssid = "your_SSID"; // <<<<<<<< Your Wi-Fi SSID 
const char* password = "password_SSID"; // <<<<<<<< Your Wi-Fi Pasword
//--------------------------------------//
const char* server = "api.thingspeak.com";
//int data1, data2, data3, data4, data5, ok;
int semn;
int temp;
int vibr;
int apa;
float te;


WiFiClient client;
unsigned char buff[10], i;
String buffer1, buffer2;
void setup()
{
  Serial.begin(115200);
  delay(10);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Se conecteaza la ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectat");
}
void loop()
{
  if (Serial.available() > 0)
  {
    delay(100);
    while (Serial.available() > 0)
    {
      buffer1 = Serial.readString();
      if (buffer1[0] == '*')
      {
        if (buffer1[9] == '#')
        {
          Serial.println(buffer1);
          temp = ((buffer1[1] - 0x30) * 100 + (buffer1[2] - 0x30)*10 + (buffer1[3] - 0x30));
          vibr = ((buffer1[4] - 0x30) * 10 + (buffer1[5] - 0x30));
          apa = ((buffer1[6] - 0x30) * 100 + (buffer1[7] - 0x30)*10 + (buffer1[8] - 0x30));
        }
      }
    }

semn = vibr/10;
vibr = vibr%10;
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
Serial.print(vibr);
Serial.print(", nivelul apei = ");
Serial.print(apa);
Serial.println("% ----------------------- ");  

  
  if (client.connect(server, 80))
  {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(te);
    postStr += "&field2=";
    postStr += String(vibr);
    postStr += "&field3=";
    postStr += String(apa);    
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.println(postStr);
  }
  client.stop();
  Serial.println("Asteapta...");
  // delay(20000);
} 
  }
//-------------Electronics-project-hub>com-------------//
