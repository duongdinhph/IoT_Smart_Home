#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

float humidity=0, temperature=0;
int state_gas=0;
int state_thief=0;
LiquidCrystal_I2C lcd(0x27,16,2);
byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};
char auth[] = "qJ0mfD7YdgVtKYL5Y317cC7UUKKQyaff";
char ssid[] = "Duong  Duc";
char pass[] = "13051999";
BlynkTimer timer;

void sendUptime() {
  Blynk.virtualWrite(V2, state_gas);
  Blynk.virtualWrite(V3, state_thief);
}

BLYNK_WRITE(V5) {
  humidity = param.asFloat();
  Serial.print("do am:");
  Serial.println(humidity);
}

BLYNK_WRITE(V6) {
  temperature = param.asFloat();
  Serial.print("nhiet do:");
  Serial.println(temperature);
}

void setup() {
  Serial.begin(9600);
  pinMode(14, OUTPUT);          //D5 for buzz
  pinMode(12, INPUT);           //D6 for thief
  Blynk.begin(auth, ssid, pass, "blynk.en-26.com", 9600); 
  timer.setInterval(2000L, sendUptime);
  lcd.begin(16,2);
  lcd.init();  
  lcd.backlight();
  lcd.print("Nhiet do: ");
  lcd.setCursor(0,1);
  lcd.print("Do am: ");
  lcd.createChar(1, degree);
  }

void detect_thief() { 
  state_thief=0;
  if(digitalRead(12)==HIGH) { 
    state_thief=255;
    buzz(); 
  }
}

void detect_NVS() { 
  if(digitalRead(13)==HIGH)
  { 
    digitalWrite(15,HIGH);
  }
}

void mq6() { 
  float sensorValue = analogRead(A0);
  float sensorVoltage = sensorValue/1024*5.0;
 
  Serial.print("sensor voltage = ");
  Serial.print(sensorVoltage);
  Serial.println(" V");
  state_gas=0;
  if(sensorVoltage>4.0) { 
    state_gas=255;
    buzz();
    Blynk.notify("Hey, Blynkers! GASS!");
  }
  delay(500);
}

void buzz() { 
  float sinVal;
  int toneVal;
  for(int x=0; x<180; x++){
    // convert degrees to radians then obtain value
    sinVal = (sin(x*(3.1412/180)));
    // generate a frequency from the sin value
    toneVal = 2000+(int(sinVal*1000));
    tone(14, toneVal);        
    delay(2); 
    }
  noTone(14);
}

void showTH() {
  lcd.setCursor(10,0);
  lcd.print(round(temperature));
  lcd.print(" ");
  lcd.write(1);
  lcd.print("C");  
  lcd.setCursor(10,1);
  lcd.print(round(humidity));
  lcd.print(" %");
}

void loop() {
  Blynk.run();
  timer.run();
  showTH();
  mq6(); 
  detect_thief(); 
}