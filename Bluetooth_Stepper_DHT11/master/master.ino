// MASTER
// AT+ROLE=1
// AT+CMODE=1
// AT+UART=9600,0,0
#include <SoftwareSerial.h>
#include "DHT.h"
#define DHTPin 4
#define DHTType DHT11
//Cổng D10 = TX = cam, D11 = RX = đỏ, 5V = xanh lá, GND = vàng, nhớ chuyển Both NL & CR
SoftwareSerial BTSerial(10, 11);

DHT HT(DHTPin, DHTType);
float humidity;
float temperatureC;
float temperatureF;

void setup() {
  // put your setup code here, to run once:
  HT.begin();
  Serial.begin(9600);
  BTSerial.begin(9600);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(200);
  humidity = HT.readHumidity();
  temperatureC = HT.readTemperature();
  temperatureF = HT.readTemperature(true);
  Serial.print("Humidity : ");
  Serial.print(humidity);
  Serial.print(", Temperature : ");
  Serial.print(temperatureC);
  Serial.print("oC ~");
  Serial.print(temperatureF);
  Serial.println("oF");

  String valueStringC = String(temperatureC);
  const char* valueC = valueStringC.c_str();

  BTSerial.write(valueC);
  BTSerial.print(",");
  String valueStringhumidity = String(humidity);
  const char* valuehumidity = valueStringhumidity.c_str();
  
  BTSerial.write(valuehumidity);
  delay(2000);
}
