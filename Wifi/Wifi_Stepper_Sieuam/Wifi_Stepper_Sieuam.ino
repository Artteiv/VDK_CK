#include <Scheduler.h>
#include <Task.h>
#include <LeanTask.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Stepper.h>
//NHỚ KẾT NỐI CÙNG MỘT WIFI, KIỂM TRA ĐỊA CHỈ IP TRÊN FILE INDEX

/*
Thiết bị gắn:
  - Động cơ Stepper: Gắn dây IN1~D2, IN2~D5, IN3~D6, IN4~D7 // Đã kiểm tra, ok
  - Cảm biến siêu âm: Gắn chân trig vào chân D1, chân echo vào chân D0, // Đã kiểm tra, ok
*/

// Thiết lập thông số cho WiFi
const char* ssid = "123456789";
const char* password = "1234@56789";

// Khởi tạo đối tượng máy chủ web
ESP8266WebServer server(80);

#define trig 5//gpio1
#define echo 16//gpio4
const int STEPS = 2048;
Stepper myStepper = Stepper(STEPS,4,12,14,13);
//gpio 4 12 14 13
// d   2  6 5  7
// o   8  10 9 11

int angle = 90;
int degreeToSteps(int degree, int STEPS = 2048){
  if (degree == 0) return 0;
  return STEPS / (360/degree);
}

class BlinkTask : public Task {
 protected:
  void setup() {
  }

  void loop() {
    int degreeC = 0;
    if (angle>0){
      degreeC = degreeToSteps(angle);
    }
    else degreeC = -degreeToSteps(-angle);
    myStepper.step(degreeC);
    delay(1000);
  }

} spin;

void handleSieuam(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  int duration = pulseIn(echo, HIGH);
  int distance = duration*0.034/2.0;
  Serial.print("khoang cach = ");
  Serial.print(distance);
  Serial.print("cm\n");
  server.send(200,"text/plain",String(distance));
}

void handleStepper() {
  // Xử lý vận hành Servo ở đây, dựa trên tham số được truyền (eg)
  Serial.println("Da goi");
  if (server.hasArg("angle")) {
    angle = server.arg("angle").toInt();
    Serial.print("Get info: ");
    Serial.println(angle);
    server.send(200, "text/plain", "Stepper đã được điều khiển");
  } else {
    server.send(400, "text/plain", "Thiếu tham số cho Stepper");
  }
}


void setup() {
  // Khởi động Serial để theo dõi thông tin debug
  Serial.begin(9600);

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  Serial.println("Kết nối đến WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Đã kết nối thành công!");
  server.on("/sieuam", handleSieuam);
  server.on("/stepper", HTTP_GET, handleStepper);

  // Khởi động máy chủ
  server.enableCORS(true);
  server.begin();
  Serial.println("Server started");
  Serial.print("IP Address of network: "); // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  // Bắt đầu cảm biến DHT và Servo
  // dht.begin();
  // pinMode(LINE,INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  myStepper.setSpeed(13);
  // lập lịch
  Scheduler.start(&spin);
  Scheduler.begin();
}

void loop() {
  server.handleClient(); // Xử lý các yêu cầu từ client
}
