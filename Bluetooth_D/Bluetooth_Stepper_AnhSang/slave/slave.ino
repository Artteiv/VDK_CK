// SLAVE
// AT+ROLE=0
// AT+CMODE=1
// AT+UART=9600,0,0
#include <SoftwareSerial.h>
#include <Stepper.h>
const int STEPS = 2048;

// Cắm dây như hình: In1 = 8 = xanh lá , In2 = 9 = xanh biển, In3 = 10 = tím, In4 = 11 =xám
Stepper myStepper = Stepper(STEPS, 8, 10, 9, 11);


// DT5 = TX = nâu, DT6 = RX = đen, VCC = cam, GND = Đỏ
SoftwareSerial BTSerial(5, 6);

// Hàm degree to steps
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
    int degreetrue = degreeToSteps(90);
    int degreefalse = degreeToSteps(45);
    if (value > 700) {
      myStepper.step(degreetrue);
      Serial.print("Stepped 90\n");
    }
    else {
      myStepper.step(-degreefalse);
      Serial.print("Stepped -45\n");
    }
  }
}
