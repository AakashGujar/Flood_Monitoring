//Library
#include "ThingSpeak.h"
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>

//Defining LED, Buzzer and Ultrasonic sensor------------------------------------------------

const int trigPin = D0;
const int echoPin = D1;
const int redled = D2; //Red LED is D4
const int grnled = D3; //Green LED is D2
const int BUZZER = D4;  //Buzzer

//Credentials---------------------------------------------------------------------------------

unsigned long ch_no = 1883335;  //Thingspeak's Channel ID
const char * write_api = "JTSIIO9WVFTYJDZ8";  //Thingspeak write API
char auth[] = "mwa0000027801576"; //Author of ThingSpeak
char ssid[] = "Bexst";
char pass[] = "yoangelo";

//variables defined for timing purpose--------------------------------------------------------

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000;

//To connect node mcu to the internet---------------------------------------------------------

WiFiClient  client;
long duration;
int distance;

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redled, OUTPUT);
  pinMode(grnled, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);   //connect karnee thingspeak ko
  startMillis = millis();  //initial start time
}

void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  digitalWrite(redled, LOW);
  digitalWrite(grnled, LOW);
  digitalWrite(BUZZER, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.println(distance);
  
  if (distance <= 4)
  {
    digitalWrite(D3, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(BUZZER,HIGH);
    delay(500);
    Serial.print("1 Distance: ");
    Serial.println(distance);
  }
  else
  {
    digitalWrite(D2, HIGH);
    digitalWrite(D3, LOW);
    digitalWrite(BUZZER, LOW);
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
