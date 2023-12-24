#include <Scheduler.h>
#include <Task.h>
#include <LeanTask.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
// #include <DHT.h>
#include <Servo.h>

/*
Thiết bị gắn:
  - Động cơ servo: Gắn dây vàng(gốc)-xanh(của nhóm) là chân data vào D2 // đã kiểm tra, //oke 
  - Cảm biến ánh sáng: Gắn data A0 vào chân A0(esp), chân nguồn vào 3v3 // Đã kiểm tra, oke
*/

// Thiết lập thông số cho WiFi
const char* ssid = "123456789";
const char* password = "1234@56789";

// Khởi tạo đối tượng máy chủ web
ESP8266WebServer server(80);

// 
#define SERVO_PIN 4
#define LIGHT 0

// DHT dht(DHTPIN, DHTTYPE);
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


void handleLight() {
  int value = analogRead(LIGHT);
  Serial.println(value);
  if (isnan(value)) {
    server.send(500, "text/plain", "Lỗi khi đọc độ ẩm từ cảm biến");
  } else {
    server.send(200, "text/plain", String(value));
  }
}

void handleServo() {
  if (server.hasArg("angle")) {
    servo_angle = server.arg("angle").toInt()*2;
    server.send(200, "text/plain", "Servo đã được điều khiển");
  } else {
    server.send(400, "text/plain", "Thiếu tham số cho Servo");
  }
}


void setup() {
  Serial.begin(9600);

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  Serial.println("Kết nối đến WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Đã kết nối thành công!");

  server.on("/light",HTTP_GET, handleLight);
  server.on("/servo", HTTP_GET, handleServo);

  /**
   * Khởi động máy chủ
   */ 
  server.enableCORS(true);
  server.begin();
  Serial.println("Server started");
  Serial.print("IP Address of network: "); // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  /*
  Thiết bị gắn phụ:
    - Động cơ servo, 
    - Cảm biến ánh sáng (không khai báo vì mặc định chân tương tự là input)
  */
  servo.attach(SERVO_PIN);
  Scheduler.start(&servoSpin);
  Scheduler.begin();

}

void loop() {
  server.handleClient(); // Xử lý các yêu cầu từ client
}
