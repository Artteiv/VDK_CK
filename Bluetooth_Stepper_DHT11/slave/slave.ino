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
    String c = BTSerial.readStringUntil('\n');
    Serial.println(c);
    int value = c.toInt();
    int commaIndex = c.indexOf(',');
    if (commaIndex != -1) {
      String tempCStr = c.substring(0, commaIndex);
      String tempFStr = c.substring(commaIndex + 1);

      float tempC = tempCStr.toFloat();
      float tempF = tempFStr.toFloat();

        // Bây giờ bạn có thể sử dụng tempC và tempF trong logic của mình
      int degreetrue = degreeToSteps(90);
      int degreefalse = degreeToSteps(45);

      if (tempC > 25 && tempF > 75 ) {
        myStepper.step(degreetrue);
        Serial.print("Stepped 90\n");
      }
      else {
        myStepper.step(-degreefalse);
        Serial.print("Stepped -45\n");
      }
    }
  }
}
