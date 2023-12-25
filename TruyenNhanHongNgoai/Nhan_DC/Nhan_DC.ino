#include <IRremote.h>
#define PIN_RECV 2

IRrecv receiver(PIN_RECV);
decode_results output;

// Kết nối động cơ A
int enableA = 3;
int input1 = 5;
int input2 = 4;

void setup() {
  pinMode(enableA, OUTPUT);
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);
  Serial.begin(9600);
  IrReceiver.begin(PIN_RECV, ENABLE_LED_FEEDBACK);
}

void loop() {
  //  Serial.println("Received something...");
  //  Serial.println();
  //  Serial.println(IrReceiver.decode());
  if (IrReceiver.decode()) {
    uint32_t val = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(val);
    if (val == 1) {
      Serial.println("decode");
      analogWrite(enableA, 255);
      // Động cơ chạy thuận 5s
      digitalWrite(input1, HIGH);
      digitalWrite(input2, LOW);
      delay(1000);
    } else if (val == 0) {
      Serial.println("decoode2");
      analogWrite(enableA, 255);
      digitalWrite(input1, LOW);
      digitalWrite(input2, HIGH);
      delay(1000);
    }
    IrReceiver.resume();
  }
}
