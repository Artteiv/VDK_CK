#include <IRremote.h>
int val = 0;
int LIGHT = 0;

void setup() { 
  Serial.begin(9600);
  IrSender.begin(3); 
} 
  
void loop() { 
  val = analogRead(LIGHT);
  Serial.println(val);
  if (val > 700) {
    IrSender.sendNECRaw(1); 
    delay(500);
  }
  else{
    IrSender.sendNECRaw(2);
    delay(500);
  }
      
}
