#define BLYNK_PRINT Serial
#define DHTPIN 2        // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11

#include "DHT.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "hyXqjf0QJblVjFiSKLeQ5X0bzOxXwb4F";
char ssid[] = "Duong  Duc";
char pass[] = "13051999";
DHT dht(DHTPIN, DHTTYPE);
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
float saiso;
int u;
uint16_t r, g, b, c, lux;
float lux_sp=500;

BlynkTimer timer;
WidgetBridge bridge1(V1);

BLYNK_CONNECTED() {
  bridge1.setAuthToken("qJ0mfD7YdgVtKYL5Y317cC7UUKKQyaff"); 
}

BLYNK_WRITE(V2) {
  lux_sp = param.asInt();
  if(lux_sp==0) lux_sp=500;
  u=lux_sp-100;
  analogWrite(D5,u);
  Serial.println(lux_sp);  
}

void sendUptime() { 
  dosang();
  Blynk.virtualWrite(V0,lux);
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  Serial.println("Humidity and temperature\n\n");
  Serial.print("Current humidity = ");
  Serial.print(h);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(t); 
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V5, h);
  bridge1.virtualWrite(V6, t);
  bridge1.virtualWrite(V5, h);   
}

void setup() {
  dosang();
  u=lux_sp-100;
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.en-26.com", 9600); 
  Serial.println("cant connect");
  timer.setInterval(2000L, sendUptime);
  dht.begin();
}

void loop() {
  Blynk.run();
  timer.run();
  dosang();
}

void dieukhien() {
  dosang();
  while(saiso>0.03) {   
    dosang();
    u=u+0.1*(lux_sp-lux);
    analogWrite(D5,u);
    Serial.print(" u:"); Serial.print(u);
    Serial.print("  Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
    Serial.print("  saiso: "); Serial.print(saiso, DEC); Serial.print(" - ");
    Serial.println(" ");
}  
}

void dosang() {
  tcs.getRawData(&r, &g, &b, &c);
  lux = tcs.calculateLux(r, g, b); 
  saiso=(abs(lux_sp-lux))/lux_sp;
}