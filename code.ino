// C++ code
//
#include <Servo.h>

int buzzer_pin = 3;

Servo servo1;
int buttonPin = 8;
int timerButtonPin = 9;

int last_button_state = 0;      
int current_button_state = 0; 

int last_button_state1 = 0;      
int current_button_state1 = 0; 

int ldrPin = 2;
volatile bool status=false; //perdenin açık/kapalı olma durumu
bool timerDurum = false;  //timer calısma durumu
int butSay = 0;

void setup()
{
  servo1.attach(6);
  pinMode(buttonPin,INPUT);
  pinMode(buzzer_pin,OUTPUT);
  
  servo1.write(0);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(ldrPin),openOrClose,CHANGE);
}

void loop()
{
  if(state_check() && butSay == 0){ //butona 1 kere basılınca
    openOrClose();          //perdenin durumu degisti
    buzz();
    butSay = butSay + 1;
  delay(1000);
  }
  
  if(butSay == 1){          //butonsay 1 olunca interrupt disable edildi
    disable_interrupts();
  }
  
  if(state_check() && butSay == 1){   //butona 2. basımında perdenin durumu degisti
    enable_interrupts();        //interrupt aktif edildi
    butSay = 0;
  }
  
  if(state_check1()){ //timer modunu ayarlıyor
    timerDurum = true;
    openOrClose();
    buzz();
  delay(1000);
    disable_interrupts();  //bu sure boyunca interrupt disable halde
  }
  
  if(timerDurum){ //perde belirli bir sure acık kapalı
    delay(100000);
    timerDurum = false;
    enable_interrupts(); //interrupt tekrar enable ediliyor
  }
  
}

void disable_interrupts(){
    delay(1000);
    cli(); //disable yapan fonksiyon
}

void enable_interrupts(){
    sei(); //enable yapan fonksiyon
    openOrClose();
    buzz();
}


void openOrClose(){
  status = !status; //perdenin durumunu degistirip acıp kapıyor
  if(status){
    servo1.write(180);
    Serial.println("Perde acildi");
  }
  else{
    servo1.write(0);
    Serial.println("Perde kapandi\n");
  }
}

bool state_check(){
   last_button_state = current_button_state;   //butonun onceki hali          
   current_button_state = digitalRead(buttonPin);  //yeni okunan   
   return (current_button_state == HIGH &&        //yeni okunmussa ve eskisinden farklıysa true      
           current_button_state != last_button_state);  
                                                        
}

bool state_check1(){
   last_button_state1 = current_button_state1;            
   current_button_state1 = digitalRead(timerButtonPin);     
   return (current_button_state1 == HIGH &&              
           current_button_state1 != last_button_state1);  
                                                        
}

void buzz(){
    delay(300);
    tone(buzzer_pin,500);
    delay(300);
    noTone(buzzer_pin);
    delay(300);
}
