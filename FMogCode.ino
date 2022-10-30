//Library
#include "ThingSpeak.h"
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>



//Defining LED, Buzzer aur ye Ultrasonic sensor------------------------------------------------

const int trigPin1 = D0;
const int echoPin1 = D1;
const int redled = D4; //Red LED is D4
const int grnled = D2; //Green LED is D2
const int BUZZER = D5;  //Buzzer

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
long duration1;
int distance1;

void setup()
{
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(redled, OUTPUT);
  pinMode(grnled, OUTPUT);
  digitalWrite(redled, LOW);
  digitalWrite(grnled, LOW);
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
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  Serial.println(distance1);
  if (distance1 <= 4)
  {
    digitalWrite(D4, HIGH);
    tone(BUZZER, 300);
    digitalWrite(D2, LOW);
    delay(1500);
    noTone(BUZZER);
  }
  else
  {
    digitalWrite(D2, HIGH);
    digitalWrite(D4, LOW);
  }
  currentMillis = millis();
  if (currentMillis - startMillis >= period)
  {
    ThingSpeak.setField(1, distance1);
    ThingSpeak.writeFields(ch_no, write_api);
    startMillis = currentMillis;
  }
}
