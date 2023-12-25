// Connect the HC-05 and communicate using the serial monitor
// When first powered on. you need to manually enter AT mode
// The default baud rate for AT mode is 38400
#include <SoftwareSerial.h>
int count0 = 0;
int count1 = 0;
SoftwareSerial BTSerial(10, 11); // RX | TX
// Connect the HC-05 TX to Arduino pin 10
// Connect the HC-05 RX to Arduino pin 11

int GasPin = 0;
int val;

void setup() {
  Serial.begin(9600);
  pinMode(GasPin, INPUT);
  Serial.println("Arduino is ready:");
  Serial.println("Remember to select Both BL & CR in the serial monitor");
  BTSerial.begin(9600); // HC-05 default speed in AT command
}

void loop() {
  val = analogRead(GasPin);
  Serial.println(val, DEC);
  if (val > 550) {    
      Serial.println("YES");
      BTSerial.write("1");
      delay(2000);
    }
    else{
      
  //    reverse_derection();
      Serial.println("NO");
      BTSerial.write("0");
      delay(2000);
    } 
  if (BTSerial.available())
  {
    Serial.write(BTSerial.read());
  }
  if (Serial.available())
  {
    BTSerial.write(Serial.read());
  }
}
