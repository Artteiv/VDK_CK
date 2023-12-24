#include <Scheduler.h>
#include <Task.h>
#include <LeanTask.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
//NHỚ KẾT NỐI CÙNG MỘT WIFI, KIỂM TRA ĐỊA CHỈ IP TRÊN FILE INDEX

/*
Thiết bị gắn:
  - Động cơ servo: Gắn dây xanh (của nhóm)- vàng(gốc) là chân data vào D2  
  - Cảm biến siêu âm: Gắn chân trig vào chân D1, chân echo vào chân D0, 
*/

// Thiết lập thông số cho WiFi
const char* ssid = "123456789";
const char* password = "1234@56789";

// Khởi tạo đối tượng máy chủ web
ESP8266WebServer server(80);

#define trig 5  //D0
#define echo 16  //D1
#define SERVO_PIN 4

Servo servo;

int servo_angle = 90;
class BlinkTask : public Task {
protected:
  void setup() {
  }

  void loop() {
    servo.write(servo_angle);
    delay(1000);
    servo.write(0);
    delay(1000);
  }

} servoSpin;

void handleSieuam() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  int duration = pulseIn(echo, HIGH);
  int distance = duration * 0.034 / 2.0;
  Serial.print("khoang cach = ");
  Serial.print(distance);
  Serial.print("cm\n");
  server.send(200, "text/plain", String(distance));
}

void handleServo() {
  // Xử lý vận hành Servo ở đây, dựa trên tham số được truyền (eg)
  if (server.hasArg("angle")) {
    servo_angle = server.arg("angle").toInt() * 2;
    server.send(200, "text/plain", "Servo đã được điều khiển");
  } else {
    server.send(400, "text/plain", "Thiếu tham số cho Servo");
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
  server.on("/servo", HTTP_GET, handleServo);

  // Khởi động máy chủ
  server.enableCORS(true);
  server.begin();
  Serial.println("Server started");
  Serial.print("IP Address of network: ");  // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  // Bắt đầu cảm biến DHT và Servo
  // dht.begin();
  // pinMode(LINE,INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  servo.attach(SERVO_PIN);
  Scheduler.start(&servoSpin);
  Scheduler.begin();
}

void loop() {
  server.handleClient();  // Xử lý các yêu cầu từ client
}
