//Library-------------------------------------------------------------------------------------
#include "ThingSpeak.h"
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>



//Defining LED, Buzzer aur ye Ultrasonic sensor------------------------------------------------

const int trigPin = D0;
const int echoPin = D1;
const int grnled = D2; //Green LED is D2
const int redled = D3; //Red LED is D3
const int BUZZER = D5;  //Buzzer
long duration;
int distance;

//Credentials---------------------------------------------------------------------------------

unsigned long ch_no = 1883335;  //Thingspeak's Channel ID
const char * write_api = "JTSIIO9WVFTYJDZ8";  //Thingspeak write API
char auth[] = "mwa0000027801576"; //Author of ThingSpeak
char ssid[] = "Gujar Home";
char pass[] = "msaagggg";

//variables defined for timing purpose--------------------------------------------------------

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000;

//To connect node mcu to the internet---------------------------------------------------------

WiFiClient  client;

//---------------------------------------------------------------------------------------------
void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redled, OUTPUT);
  pinMode(grnled, OUTPUT);
  pinMode(BUZZER , OUTPUT);
  
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);   //connecting thingspeak
  startMillis = millis();  //initial start time
}
//-----------------------------------------------------------------------------------------------
void loop()
{
  digitalWrite(redled, LOW);
  digitalWrite(grnled, LOW);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  
  if (distance1 <= 4)
  {
    digitalWrite(D3, HIGH);             //Red led High
    digitalWrite(D2, LOW);              //Green Led Low
    digitalWrite(BUZZER, HIGH);         //Buzzer low
    delay(500);
    Serial.print("1 Distance: ");
    Serial.println(distance);
  }
  else
  {
    digitalWrite(D2, HIGH);             //Green Led Low
    digitalWrite(D3, LOW);              //Red led Low
    digitalWrite(BUZZER, LOW);          //Buzzer low
    delay(500);
    Serial.print("2 Distance: ");
    Serial.println(distance);
  }
  currentMillis = millis();
  if (currentMillis - startMillis >= period)
  {
    ThingSpeak.setField(1, distance);
    ThingSpeak.writeFields(ch_no, write_api);
    startMillis = currentMillis;
  }
}
