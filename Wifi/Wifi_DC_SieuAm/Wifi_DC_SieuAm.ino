#include <Scheduler.h>
#include <Task.h>
#include <LeanTask.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Thiết lập thông số cho WiFi
const char* ssid = "123456789";
const char* password = "1234@56789";

// Khởi tạo đối tượng máy chủ web
ESP8266WebServer server(80);
/*
Thiết bị gắn:
  - Động cơ DC: enable - D5, input1-D6, input2-D7  
  - Cảm biến siêu âm: Gắn chân trig vào chân D1, chân echo vào chân D0, 
*/
#define trig 5  //D1
#define echo 16  //D0

// Khai báo các chân điều khiển động cơ DC
const int enableDC = 14;  // Chân enable động cơ DC - D5
const int input1DC = 12;  // Chân input1 động cơ DC - D6
const int input2DC = 13;  // Chân input2 động cơ DC - D7

// Biến để lưu tốc độ hiện tại của động cơ DC
int currentSpeed = 0;

// Hằng số đại diện cho tốc độ tối đa và tốc độ tối thiểu
const int MAX_SPEED = 255;
const int MIN_SPEED = 0;

// Hàm điều khiển động cơ DC theo hướng thuận
void runDCForward() {
  digitalWrite(enableDC, HIGH);
  digitalWrite(input1DC, HIGH);
  digitalWrite(input2DC, LOW);
}

// Hàm điều khiển động cơ DC theo hướng nghịch
void runDCBackward() {
  digitalWrite(enableDC, HIGH);
  digitalWrite(input1DC, LOW);
  digitalWrite(input2DC, HIGH);
}

// Hàm dừng động cơ DC
void stopDC() {
  digitalWrite(enableDC, LOW);
  digitalWrite(input1DC, LOW);
  digitalWrite(input2DC, LOW);
}

// Hàm tăng tốc độ động cơ DC
void accelerateDC() {
  currentSpeed += 5; // Điều chỉnh tăng tốc độ theo yêu cầu của bạn
  // Đặt giới hạn tốc độ tối đa
  if (currentSpeed > MAX_SPEED) {
    currentSpeed = MAX_SPEED;
  }

  // Áp dụng tốc độ vào điều khiển động cơ DC
  analogWrite(enableDC, currentSpeed);
}

// Hàm giảm tốc độ động cơ DC
void decelerateDC() {
  currentSpeed -= 5; // Điều chỉnh giảm tốc độ theo yêu cầu của bạn

  // Đặt giới hạn tốc độ tối thiểu
  if (currentSpeed < MIN_SPEED) {
    currentSpeed = MIN_SPEED;
  }

  // Áp dụng tốc độ vào điều khiển động cơ DC
  analogWrite(enableDC, currentSpeed);
}
int direction = 0;
class BlinkTask : public Task {
    void setup() {
  }

  void loop() {
    if (direction == 1) {
      runDCForward();
    } else if (direction == 2) {
      runDCBackward();
    } else if (direction == 3) {
      accelerateDC();
    } else if (direction == 4) {
      decelerateDC();
    } else {
      stopDC();
    }
  }
} DCTask;


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

void handleDC() {
  // Xử lý yêu cầu điều khiển động cơ DC
  if (server.hasArg("direction")) {
    direction = server.arg("direction").toInt();
    server.send(200, "text/plain", "Đã điều khiển động cơ DC");
  } else {
server.send(400, "text/plain", "Thiếu tham số cho động cơ DC");
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

  // Thiết lập các chân điều khiển động cơ DC
  pinMode(enableDC, OUTPUT);
  pinMode(input1DC, OUTPUT);
  pinMode(input2DC, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  // Thiết lập chân động cơ DC và thử nghiệm
  stopDC();  // Dừng động cơ khi khởi động
  
  // Thêm đường dẫn xử lý driver
  server.on("/sieuam", handleSieuam);
  server.on("/dc", HTTP_GET, handleDC);

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
  Scheduler.start(&DCTask);
  Scheduler.begin();
}

void loop() {
  server.handleClient();  // Xử lý các yêu cầu từ client
  // Thêm bất kỳ công việc loop nào khác nếu cần
}