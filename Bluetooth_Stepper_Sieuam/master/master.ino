// MASTER
// AT+ROLE=1
// AT+CMODE=1
// AT+UART=9600,0,0
#include <SoftwareSerial.h>
#define LINE 2
#define triPin 5
#define echoPin 6
int duration;
int distance;
SoftwareSerial BTSerial(10, 11);

void setup() {
  // put your setup code here, to run once:
  pinMode(LINE, INPUT);
  pinMode(triPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(triPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2.0;
  Serial.print("khoang cach = ");
  Serial.print(distance);
  Serial.print("cm\n");

  String valueString = String(distance);
  const char* value = valueString.c_str();
  Serial.println(value);
  BTSerial.write(value);
  delay(2000);
}
