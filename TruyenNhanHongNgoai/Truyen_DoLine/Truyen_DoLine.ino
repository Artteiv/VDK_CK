#define PIN_SEND 3
#include <IRremote.h> 
#define LINE 2

int val = 0;


void setup() { 
  Serial.begin(9600);
  IrSender.begin(PIN_SEND); 
} 
  
void loop() { 
  delay(2000);
  val = digitalRead(LINE);
  Serial.println(val);
  if (val == 0) {
    IrSender.sendNECRaw(2); 
    delay(200);
  }
  else{
    IrSender.sendNECRaw(1);
    delay(200);
  }
      
}