#include <IRremote.h>
#include <Servo.h>
// #define PIN_RECV 2
Servo myservo;
IRrecv receiver(2);
decode_results output;
int servo_pin = 9;
void setup() {
  myservo.attach(servo_pin);
  Serial.begin(9600);
  // IrReceiver.begin(2);
}

void loop() {
  //  Serial.println("Received something...");
  //  Serial.println();
  //  Serial.println(IrReceiver.decode());
  uint32_t val = IrReceiver.decodedIRData.decodedRawData;
  Serial.println(val);
  if (val == 1) {
    Serial.println("decode");
    myservo.write(90);
    delay(1000);
    myservo.write(0);
    delay(1000);
  } else if (val == 0) {
    Serial.println("decode2");
    myservo.write(45);
    delay(1000);
    myservo.write(0);
    delay(1000);
  }
  IrReceiver.resume();
}
