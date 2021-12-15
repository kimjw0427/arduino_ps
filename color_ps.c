#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);


int bt[] = {950,760,560,420,310,190};


int LED_array[6][3] = {
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0}
};

 

int bt_array[] = {0,0,0,0,0,0};


int reg_array[3][3] = {
  {6,7,8},
  {3,4,5},
  {9,10,11}
};


int ps[3] = {0,0,0};
 

int answer[3] = {56,22,35};


int V_signal = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.init();

  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Door is Closed");
  lcd.setCursor(0,1);
  lcd.print("Password Wrong!");

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

}
 

void loop() {

  int Vbt = analogRead(A0);

  Serial.println(Vbt);

  if(Vbt<100){
    for(int i = 0; i < 6; i++){
      bt_array[i] = 0;
    }
  }


  for(int i = 0; i < 6; i++){
    if(bt[i]-60 < Vbt and Vbt < bt[i]+60){
      if(bt_array[i] == 0){
        //Serial.println(i);
        int b_bt = 0;
        
        b_bt = (LED_array[i][2])*4;
        b_bt = b_bt + (LED_array[i][1])*2;
        b_bt = b_bt + (LED_array[i][0])*1;
        b_bt++;

        if(b_bt == 8){
          b_bt = 0;
        }

        if(int(b_bt/(4))==1){
          b_bt = b_bt - 4;
          LED_array[i][2] = 1;
        } else{
          LED_array[i][2] = 0;
        }
        if(int(b_bt/(2))==1){
          b_bt = b_bt - 2;
          LED_array[i][1] = 1;
        } else{
          LED_array[i][1] = 0;
        }
        if(int(b_bt/(1))==1){
          LED_array[i][0] = 1;
        } else{
          LED_array[i][0] = 0;
        }
        
        bt_array[i] = 1;
        tone(2,262,200);
     
      }
    }
  }

  for(int i=0;i<3;i++){
    int d = 0;
    for(int ii=0;ii<6;ii++){
      d = d + int((LED_array[ii][i])*(pow(2,ii)));
    }
    digitalWrite(reg_array[i][1], LOW);
    for(int ii=2;ii<6;ii++){
      d = d + int((LED_array[ii][i])*1);
    }
    ps[i] = d;
    shiftOut(reg_array[i][2],reg_array[i][0],MSBFIRST,d);
    digitalWrite(reg_array[i][1],HIGH);
  }
    
  delay(15);

  Serial.print(ps[0]);
  Serial.print(',');
  Serial.print(ps[1]);
  Serial.print(',');
  Serial.println(ps[2]);

  if(ps[0]==answer[0]&&ps[1]==answer[1]&&ps[2]==answer[2]){
    if(V_signal == 0){
      tone(2,262,100);
      delay(100);
      tone(2,294,100);
      delay(100);
      tone(2,330,100);
      delay(100);
      tone(2,349,100);
      delay(100);
      tone(2,392,100);
      delay(100);
      tone(2,440,100);
      delay(100);
      tone(2,494,100);
      delay(100);
      tone(2,523,100); 
    }
    ds_open();
    V_signal = 1;
    Serial.println("open");
  } else {
    if(V_signal == 1){
      //ds_close();
    }
    ds_close();
    V_signal = 0;
    Serial.println("close");
  }
}

void ds_open(){
  //lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Door is Opened");
  lcd.setCursor(0,1);
  lcd.print("Password Correct!");
}

void ds_close(){
  //lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Door is Closed");
  lcd.setCursor(0,1);
  lcd.print("Password Wrong! ");
}
