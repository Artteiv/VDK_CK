// SLAVE
// AT+ROLE=0
// AT+CMODE=1
// AT+UART=9600,0,0
#include <SoftwareSerial.h>
int ena = 10;
int in1 = 9;
int in2 = 8;
int speed = 100;

SoftwareSerial BTSerial(5, 6);

void setup() {
  // put your setup code here, to run once:
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (BTSerial.available()) {
    String c = BTSerial.readString();
    Serial.println(c);
    int value = c.toInt();
    if (value > 1000){}
    else if (value > 500){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      for (int i = 0; i <= 255; i += 50) {
        analogWrite(ena, i);
        delay(300);
      }
      analogWrite(ena, 0);
    } else {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      for (int i = 255; i >= 10; i -= 50) {
        analogWrite(ena, i);
        delay(300);
      }
      analogWrite(ena, 255);
    }
    // delay(5);
  }

  // while (Serial.available()) {
  //   char c = Serial.read();
  //   BTSerial.write(c);
  //   delay(5);
  // }
}
