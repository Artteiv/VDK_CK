#include <Scheduler.h>
#include <Task.h>
#include <LeanTask.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
// Thiết lập thông số cho WiFi
const char* ssid = "123456789";
const char* password = "1234@56789";

// Khởi tạo đối tượng máy chủ web
ESP8266WebServer server(80);
/*
Thiết bị gắn:
  - Động cơ servo: Gắn dây xanh (của nhóm)- vàng(gốc) là chân data vào D2 
  - Cảm biến joystck: Gắn data A0 vào chân A0(esp), chân nguồn vào 3v3 
*/
#define x 0
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

void handleJoystick() {
  int val = analogRead(gasPin);
  Serial.println(val);
  if (isnan(val)) {
    server.send(500, "text/plain", "Lỗi khi đọc từ cảm biến");
  } else {
    server.send(200, "text/plain", String(val));
  }
}

void handleServo() {
  // Xử lý vận hành Servo ở đây, dựa trên tham số được truyền (eg)
  if (server.hasArg("angle")) {
    servo_angle = server.arg("angle").toInt()*2;
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
  
  // Thêm đường dẫn xử lý driver
  server.on("/joystick", handleJoystick);
  server.on("/servo", HTTP_GET, handleServo);

  // Khởi động máy chủ
  server.enableCORS(true);
  server.begin();
  Serial.println("Server started");
  Serial.print("IP Address of network: ");
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  // Lập lịch cho nhiệm vụ của DCTask
  // dht.begin();
  servo.attach(SERVO_PIN);
  Scheduler.start(&servoSpin);
  Scheduler.begin();
}

void loop() {
  server.handleClient();  // Xử lý các yêu cầu từ client
  // Thêm bất kỳ công việc loop nào khác nếu cần
}