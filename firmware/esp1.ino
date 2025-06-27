#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "RsSYqMEWcRDKSfmn8Zirkf_WyMkM6rGi";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Duong  Duc";
char pass[] = "13051999";
int ack=0;

BLYNK_WRITE(V1) {
  ack = param.asInt();
  digitalWrite(14,HIGH);
  if(ack==1) digitalWrite(4,HIGH); else digitalWrite(4,LOW);
  delay(1000);
  digitalWrite(14,LOW);
}

void setup() {
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass,"blynk.en-26.com", 9600); 
  pinMode(4, OUTPUT); 
  pinMode(14, OUTPUT);
  digitalWrite(14,LOW);
}

void loop() {
  Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}