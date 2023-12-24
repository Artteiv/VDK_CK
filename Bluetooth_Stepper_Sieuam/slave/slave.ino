// SLAVE
// AT+ROLE=0
// AT+CMODE=1
// AT+UART=9600,0,0
#include <SoftwareSerial.h>
#include <Stepper.h>
const int STEPS = 2048;
Stepper myStepper = Stepper(STEPS, 8, 10, 9, 11);

SoftwareSerial BTSerial(5, 6);

int degreeToSteps(int degree, int STEPS = 2048){
  if (degree == 0) return 0;
  return STEPS / (360/degree);
}

void setup() {
  // put your setup code here, to run once:
  myStepper.setSpeed(13);
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (BTSerial.available()) {
    String c = BTSerial.readString();
    Serial.println(c);
    int value = c.toInt();
    int degreeC = degreeToSteps(90);
    if (value > 5.0 && value < 10.0) {
      myStepper.step(degreeC);
      Serial.print("Stepped 90\n");
    }
    else {
      myStepper.step(-degreeC);
      Serial.print("Stepped -90\n");
    }
  }
}
