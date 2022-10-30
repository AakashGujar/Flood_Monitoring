#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <Adafruit_MQTT_FONA.h>


#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "Gujar Home"
#define WLAN_PASS       "msaagggg"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "aakash24"
#define AIO_KEY         "aio_Ycgy22QnjUa7ZCTEfEfPsvr7EJAt" 
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish fms = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/fms");

//pins
const int trigPin = D0; 
const int echoPin = D1; 
const int LED1 = D2;  //Red
const int LED2 = D3;  //Yellow
const int LED3 = D4;  //Green
const int BUZZER = D5;
long duration;
int distance;

void setup() {
pinMode(LED1 , OUTPUT);
pinMode(LED2 , OUTPUT);
pinMode(LED3 , OUTPUT);
pinMode(BUZZER , OUTPUT);
pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT); 
Serial.begin(9600);

Serial.println("Connecting to WiFi...");
  WiFi.begin(WLAN_SSID,WLAN_PASS);
  while(WiFi.status()!= WL_CONNECTED){
    Serial.print(".");
    delay(500);
    }
  Serial.println("WiFi Connected !");
}


void loop() {  

digitalWrite(LED1, LOW);
digitalWrite(LED2, LOW);
digitalWrite(LED3, LOW);

digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
distance= duration*0.034/2;

MQTT_Connect();
if (!fms.publish(distance))
  {
    Serial.println("Water Level Not Sent");
  }
  else
  {
    Serial.println("Water Level Sent");
  }
  
delay(500);


if(distance >= 0 && distance <= 5) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    Serial.print("1 Distance: ");
    Serial.println(distance);
  }

  else if(distance > 5 && distance <=35) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    digitalWrite(BUZZER, LOW);
    delay(500);
    Serial.print("2 Distance: ");
    Serial.println(distance);
  }
else if (distance >= 75) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(BUZZER, LOW);
    delay(500);
    Serial.print("4 Distance: ");
    Serial.println(distance);

  }

  else if (distance > 35 && distance <=75 ) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(BUZZER, LOW);
    delay(500);
    Serial.print("3 Distance: ");
    Serial.println(distance);
  }
}

void MQTT_Connect(){
     if(mqtt.connected()){
          return;
       }
     Serial.println("Connecting to MQTT...");
     int retries = 3,status;
     while((status = mqtt.connect()) != 0){
          Serial.println(mqtt.connectErrorString(status));
          Serial.println("Retrying after 5 secs....");
          delay(500);
          retries--;
          if(retries == 0){
              while(1);   // reset the nodemcu
            }
      }
      Serial.println("MQTT Connected");
  }
