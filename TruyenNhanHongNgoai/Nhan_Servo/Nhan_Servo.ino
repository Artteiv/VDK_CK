#include <IRremote.h>
#include <Servo.h>
#define PIN_RECV 2
// #define PIN_RECV 2
Servo myservo;
IRrecv receiver(PIN_RECV);
decode_results output;
int servo_pin = 9;
void setup() {
  myservo.attach(servo_pin);
  Serial.begin(9600);
  IrReceiver.begin(PIN_RECV, ENABLE_LED_FEEDBACK);
}

void loop() {
  //  Serial.println("Received something...");
  //  Serial.println();
  if (IrReceiver.decode()) {
    uint32_t val = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(val);
    if (val == 1) {
      Serial.println("decode");
      myservo.write(90);
      delay(1000);
      myservo.write(0);
      delay(1000);
    } else if (val == 2) {
      Serial.println("decode2");
      myservo.write(45);
      delay(1000);
      myservo.write(0);
      delay(1000);
    }

  }
    IrReceiver.resume();
}
