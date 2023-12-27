#include <Scheduler.h>
#include <Task.h>
#include <LeanTask.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
// Thiết lập thông số cho WiFi
const char* ssid = "123456789";
const char* password = "1234@56789";

// Khởi tạo đối tượng máy chủ web
ESP8266WebServer server(80);
/*
Thiết bị gắn:
  - Động cơ DC: enable - D5, input1-D6, input2-D7  
  - Cảm biến DHT11: Gắn dây số D vào chân D4, nguồn vào 3v3 (đã test xong)
*/
#define DHTPIN 2
#define DHTTYPE DHT11


DHT dht(DHTPIN, DHTTYPE);

// Khai báo các chân điều khiển động cơ DC
const int enableDC = 14;  // Chân enable động cơ DC - D5
const int input1DC = 12;  // Chân input1 động cơ DC - D6
const int input2DC = 13;  // Chân input2 động cơ DC - D7

// Hàm điều khiển động cơ DC theo hướng thuận
int speed;
int type = 0;
class BlinkTask : public Task {
  void setup() {
    curspeed = 0;
  }
  int curspeed = 0;
  int v1,v2;
  void tov(int speed){
    if (speed>=0){
      v2 = 0;
      v1 = speed;
    }
    else{
      v1 = 0;
      v2 = -speed;
    }
  }
  void apply_speed(){
    tov(curspeed);
    if (v1==0){
      digitalWrite(input1DC,LOW);
    }
    else{
      analogWrite(input1DC,v1);
    }
    if (v2==0){
      digitalWrite(input2DC,LOW);
    }
    else{
      analogWrite(input2DC,v2);
    }
    
  }
  //nhanh dần : 0~1000 ->0 ~1000
  // chậm dần : 1000 ~ 0 -> ...
  // Thuận chiều:  
  // hàm gọi sẽ gán giá trị speed 
  void thuan_chieu_nhanh_dan(){
    if (curspeed<0) curspeed=0;
    curspeed+=100;
    if (curspeed>1000){
      curspeed = 0;
    }
    apply_speed();
  }
  void thuan_chieu_cham_dan(){
    if (curspeed>1000) curspeed = 1000;
    curspeed -= 100;
    if (curspeed<0) curspeed = 1000;
    apply_speed();
  }
  void nguoc_chieu_nhanh_dan(){
    if (curspeed>0) curspeed = 0;
    if (curspeed< -1000) curspeed = 0;
    curspeed -=100;
    apply_speed();
  }
  void nguoc_chieu_cham_dan(){
    if (curspeed>0) curspeed = -1000;
    if (curspeed<-1000) curspeed = -1000;
    curspeed +=100;
    apply_speed();
  }
  void thuan_chieu_quay_deu(){
    curspeed = 1000;
    apply_speed();
  }
  void nguoc_chieu_quay_deu(){
    curspeed = -1000;
    apply_speed();
  } 
  void loop() {
    switch (type)
    {
      case 1: // thuan_chieu_nhanh_dan
        thuan_chieu_nhanh_dan();
      break;
      case 2: // thuan_chieu_cham_dan
        thuan_chieu_cham_dan();
      break;
      case 3: // nguoc_chieu_nhanh_dan
        nguoc_chieu_nhanh_dan();
      break;
      case 4: // nguoc_chieu_cham_dan
        nguoc_chieu_cham_dan();
      break;
      case 5: // thuan_chieu_quay_deu
        thuan_chieu_quay_deu();
      break;
      case 6: // nguoc_chieu_quay_deu
        nguoc_chieu_quay_deu();
      break;
    default:
      break;
    }
    Serial.print(v1);
    Serial.print("-");
    Serial.println(v2);
    delay(1000);
  }
} DCTask;


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

void handleDC() {
  // Xử lý yêu cầu điều khiển động cơ DC
  if (server.hasArg("direction")) {
    type = server.arg("direction").toInt();
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
  // pinMode(enableDC, OUTPUT);
  pinMode(input1DC, OUTPUT);
  pinMode(input2DC, OUTPUT);
  // Thiết lập chân động cơ DC và thử nghiệm
  stopDC();  // Dừng động cơ khi khởi động
  dht.begin();
  // Thêm đường dẫn xử lý driver
  server.on("/dht/temp", handleTemp);
  server.on("/dht/hum", handleHum);
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