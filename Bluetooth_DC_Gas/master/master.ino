// MASTER
// AT+ROLE=1
// AT+CMODE=1
// AT+UART=9600,0,0
#include <SoftwareSerial.h>
int val = 0;
int pin = 0;
//Cổng D10 = TX = cam, D11 = RX = đỏ, 5V = xanh lá, GND = vàng, nhớ chuyển Both NL & CR

SoftwareSerial BTSerial(10, 11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // val = analogRead(pin);
  // Serial.println(val);
  // char new_val = char(val);
  String valueString = String(analogRead(pin));
  const char* value = valueString.c_str();
  Serial.println(value);
  BTSerial.write(value);
  delay(2000);
}
