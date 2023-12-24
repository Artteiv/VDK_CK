#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
// #include <Schedule.h>
// #include <Task.h>
// #include <LeanTask.h>
#include <DHT.h>
#include <Servo.h>
#include <Stepper.h>

// Thiết lập thông số cho WiFi
const char* ssid = "123456789";
const char* password = "1234@56789";

// Khởi tạo đối tượng máy chủ web
ESP8266WebServer server(80);

// Thiết lập cổng cho DHT và Servo

#define DHTPIN 2
#define DHTTYPE DHT11
const int STEPS = 2048;
Stepper myStepper = Stepper(STEPS, 8, 10, 9, 11);
int angle = 90;
int degreeToSteps(int degree, int STEPS = 2048){
  if (degree == 0) return 0;
  return STEPS / (360/degree);
}

DHT dht(DHTPIN, DHTTYPE);

void handleTemp() {
  // Đọc nhiệt độ từ cảm biến DHT
  float temperature = dht.readTemperature();
  Serial.println(temperature);
  if (isnan(temperature)) {
    server.send(500, "text/plain", "Lỗi khi đọc nhiệt độ từ cảm biến");
  } else {
    server.send(200, "text/plain", String(temperature));
  }
}

void handleHum() {
  // Đọc độ ẩm từ cảm biến DHT
  float humidity = dht.readHumidity();
  Serial.println(humidity);
  if (isnan(humidity)) {
    server.send(500, "text/plain", "Lỗi khi đọc độ ẩm từ cảm biến");
  } else {
    server.send(200, "text/plain", String(humidity));
  }
}

void handleStepper() {
  // Xử lý vận hành Servo ở đây, dựa trên tham số được truyền (eg)
  if (server.hasArg("angle")) {
    angle = server.arg("angle").toInt();
    int degreeC = degreeToSteps(angle);
  if (1) {
    myStepper.step(degreeC);
    Serial.print("Stepped 90\n");
  }
  else {
    myStepper.step(-degreeC);
    Serial.print("Stepped -90\n");
  }
  delay(200);
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
  // Định nghĩa các endpoint và gán hàm xử lý tương ứng
  // server.on("/led=on", handleLEDOn);
  // server.on("/led=off", handleLEDOff);
  server.on("/dht/temp", handleTemp);
  server.on("/dht/hum", handleHum);
  server.on("/stepper", HTTP_GET, handleServo);

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
  dht.begin();
  servo.attach(SERVO_PIN);
  pinMode(LED,OUTPUT);
}

void loop() {
  server.handleClient(); // Xử lý các yêu cầu từ client
}