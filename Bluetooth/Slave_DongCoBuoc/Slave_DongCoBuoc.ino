// Connect the HC-05 and communicate using the serial monitor
// When first powered on. you need to manually enter AT mode
// The default baud rate for AT mode is 38400
#include <SoftwareSerial.h>
#include "Stepper.h"

SoftwareSerial BTSerial(10, 11); // RX | TX
// Connect the HC-05 TX to Arduino pin 10
// Connect the HC-05 RX to Arduino pin 11

const int stepsPerRevolution = 2048;
Stepper myStepper = Stepper(stepsPerRevolution, 5, 7, 6, 8);
int degreeToSteps(int degree, int STEPS = 2048){
  if (degree == 0) return 0;
  return STEPS / (360/degree);
}
void setup() {
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  Serial.begin(9600);
  Serial.println("Arduino is ready:");
  Serial.println("Remember to select Both BL & CR in the serial monitor");
  BTSerial.begin(9600); // HC-05 default speed in AT command
  pinMode(7, OUTPUT);
  myStepper.setSpeed(15);
}

void loop() {
  if (BTSerial.available())
  {
    char c = BTSerial.read();
    Serial.println(c);
    int degreetrue = degreeToSteps(90);
    if (c== '1')
    {
      myStepper.step(degreetrue);
//      delay(500);
    }
    else {
      myStepper.step(-degreetrue);
//      delay(500);
    }
  }
  if (Serial.available())
  {
    BTSerial.write(Serial.read());
  }
}
