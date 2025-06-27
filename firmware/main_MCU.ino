#include <Keypad.h> //thu vien keypad 4x4
#include <LiquidCrystal_I2C.h> //thu vien i2c 1602 lcd
#include <Servo.h> //thu vien motor servo

LiquidCrystal_I2C lcd(0x27, 16, 2); //lcd 1602 chan 0x27 kich thuoc 16x2
Servo servo;

#define DodaiPass 5                     //chieu dai password mo state_cua
char serialData;
bool state_cua = true;                  //trang thai state_cua, 0 là dang mo, 1 la dang dong

int VitriServo = 0;                     //goc quay Servo

boolean buttonState11 = 1; 
char NhapPass[DodaiPass];
char Pass[DodaiPass] = "1234";          //Gia tri Password de mo state_cua
byte NhapPass_thu = 0;
char Phim; 
const byte Hang = 4;                    //so hang state_cua keypad 4x4
const byte Cot = 4;                     //so cot state_cua keypad 4x4
char phim[Hang][Cot] = {
  {'1','2','3','A'}, 

  {'4','5','6','B'},

  {'7','8','9','C'},

  {'*','0','#','D'}
};

byte ChanHang[Hang] = {9, 8, 7, 6};     //Chan ket noi cac hang keypad vs arduino
byte ChanCot[Cot] = {5, 4, 3, 2};       //Chan ket noi cac cot keypad vs arduino
Keypad DocPhim(makeKeymap(phim), ChanHang, ChanCot, Hang, Cot); //lay du lieu tu keypad

//bieu tuong khoa dong
byte Dong[8] = { 
  B01110,
  B10001,
  B10001,
  B11111,
  B11011,
  B11011,
  B11011,
  B11111
};

//bieu tuong khoa mo
byte Mo[8] = {
  B01110,
  B00001,
  B00001,
  B11111,
  B11011,
  B11011,
  B11011,
  B11111
};

void setup() {
  servo.attach(10);                     //chan ket noi servo vs arduino
  Dongcua();                            //Dong servo
  lcd.init();                           //kich hoat lcd
  lcd.backlight();                      //bat sang lcd
  lcd.setCursor(0,0);                   //vi tri nhap chu/so
  lcd.print("KSTN DK K62");             //hien thi dong chu tren lcd
  lcd.setCursor(0,1);
  lcd.print("Welcome our home");
  delay(2000);                          //doi trong 4000ms = 4s
  lcd.clear();                          //xoa hien thi state_cua lcd
  Serial.begin(9600);
  pinMode(11, INPUT);
  pinMode(13, INPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12,0);
}

void loop() {
  if (state_cua == 0)                   //state_cua dang o trang thai mo
  {
      Phim=0;
      while(!(Phim))
      { 
        Phim = DocPhim.getKey();
        buttonState11 = digitalRead(11);
        if((!buttonState11)&&digitalRead(13)) break;
      };
   
    if ((Phim == '#')||(!buttonState11))
      {
        lcd.clear(); 
        Dongcua(); 
        lcd.setCursor(2,0); 
        lcd.print("Dong cua"); 
        lcd.createChar(0, Dong);  
        lcd.setCursor(14,0); 
        lcd.write(0); 
        delay(3000); 
        state_cua = 1;
       }
  }
  else Open(); 
}

void Open() { 
  lcd.setCursor(1,0);
  lcd.print("Chon che do");
  lcd.setCursor(0,1);
  lcd.print("1_Pass  2_Face");
  Phim = 0;

  while(!(Phim)){
    Phim = DocPhim.getKey();
    if(digitalRead(11)&&digitalRead(13)) Phim=3;  
  };

  if (Phim == '1')
  {
    MoPass();
  }
  else if (Phim == '2')
  {
    MoAI();
  }
  else if (Phim == 3)
  {
   MoBlynk();
  }

}

void MoBlynk() {
  Mocua();
  lcd.clear();
  lcd.setCursor(0,1); 
  lcd.print("Nhan # de dong");
  state_cua=0;
}

//xoa du lieu NhapPass
void xoaNhapPass() {                              
  while (NhapPass_thu != 0){
    NhapPass[NhapPass_thu--] = 0; 
  }
  return;
}

void Mocua() {
   //di chuyen dong co Servo tu 90 den 0 de mo state_cua
  for (VitriServo = 90; VitriServo >= 0; VitriServo -= 5) { 
    servo.write(VitriServo); 
    delay(15); 
    digitalWrite(12,1);
  }
}

void Dongcua() {
  //di chuyen dong co Servo tu 0 den 90 de dong state_cua
  for (VitriServo = 0; VitriServo <= 90; VitriServo += 5) { 
    servo.write(VitriServo); 
    delay(15);
    digitalWrite(12,0);
  }
}

void MoPass() { 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Nhap mat khau:");
  NhapPass_thu = 0;
  while(NhapPass_thu < DodaiPass - 1){
    Phim = DocPhim.getKey();
    if (Phim)
    {
      NhapPass[NhapPass_thu] = Phim;
      lcd.setCursor(NhapPass_thu, 1);
      lcd.print("*");
      NhapPass_thu++;
    }
  };
  if (NhapPass_thu == DodaiPass - 1)
  {
 
    if (!strcmp(NhapPass, Pass))
    {
      lcd.clear();
      Mocua();
      lcd.setCursor(2,0); 
      lcd.print("Mo cua");
      lcd.createChar(1, Mo);
      lcd.setCursor(14,0); 
      lcd.write(1);
      lcd.setCursor(0,1); 
      lcd.print("Nhan # de dong");
      state_cua = 0;                                //state_cua duoc mo   
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0,0);                           // sets the cursor on the lcd
      lcd.print("Sai rui :(");                      // prints the text/character
      lcd.setCursor(0,1);
      lcd.print("Nhap lai sau");
      lcd.setCursor(13,1);
      lcd.print("05");
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("04");
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("03");
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("02");
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("01");
      delay(1000);
      lcd.setCursor(13,1);
      lcd.print("00");
      delay(1000);
      lcd.clear();
      state_cua = 1;                                //nhap sai mk nen state_cua o trang thai dong    
    }
    xoaNhapPass();      
  }  
}

void MoAI() { 
  while(Serial.available() <= 0) {};
  if (Serial.available() > 0){
    serialData = Serial.read();
    Serial.print(serialData);
    if (serialData == '1'){
      lcd.clear();
      Mocua();
      lcd.setCursor(2,0); 
      lcd.print("Mo cua");
      lcd.createChar(1, Mo);
      lcd.setCursor(14,0); 
      lcd.write(1);
      lcd.setCursor(0,1);
      lcd.print("Nhan # de dong");
      state_cua = 0;                                //state_cua duoc mo
    }
    else if (serialData == '0')
    {
      lcd.clear();
      lcd.setCursor(0,0);                           // sets the cursor on the lcd
      lcd.print("Ko xac dinh mat");                 // prints the text/character
      lcd.setCursor(0,1);
      lcd.print("Ktra lai sau");
      lcd.setCursor(14,1);
      lcd.print("03");
      delay(1000);
      lcd.setCursor(14,1);
      lcd.print("02");
      delay(1000);
      lcd.setCursor(14,1);
      lcd.print("01");
      delay(1000);
      lcd.setCursor(14,1);
      lcd.print("00");
      delay(1000);
      lcd.clear();
      state_cua = 1;                                //nhap sai mk nen state_cua o trang thai dong
    }
  }
}