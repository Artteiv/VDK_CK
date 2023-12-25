// MASTER
// AT+ROLE=1
// AT+CMODE=1
// AT+UART=9600,0,0
#include <SoftwareSerial.h>
int lightPin = 0;
//Cổng D10 = TX = cam, D11 = RX = đỏ, 5V = xanh lá, GND = vàng, nhớ chuyển Both NL & CR
SoftwareSerial BTSerial(10, 11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  // Đọc giá trị chân tương tự A0 
  // Cắm cảm biến ánh sáng chân A0
  int val = analogRead(lightPin);
  String valueString = String(val);
  const char* value = valueString.c_str();
  
  // In ra màn hình bên master giá trị của chân tương tự A0
  Serial.println(value);
  // Gửi giá trị qua slave
  BTSerial.write(value);
  delay(2000);
}
