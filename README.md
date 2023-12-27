# VDK_CK

## Một số lưu ý khi cắm

### Cảm biến

### Động cơ

### Truyền nhận
#### Bluetooth: 
Master dây cầu vồng
    - RX 11,
    - TX 10 
Slave dây Đức
    - RX 6,
    - TX 5
- Nếu có nhu cầu sửa code:
#### Hồng ngoại:
#### Truyền: // Nên để 3.3v (nguồn 3.3)
- Tìm kiếm dấu trừ trên thiết bị
- Dấu trừ luôn là dây âm
- Tiếp theo dây âm là dây dương
- Cuối cùng là tín hiệu 
#### Nhận:
- Tương tự như truyền
#### Module

#### Những lưu ý
- Tháo nguồn động cơ servo khi nạp code, tránh bị cắn ngược hỏng driver arduino
- Với module wifi, kết nối wifi nhớ đổi địa chỉ
- Với module bluetooth, tháo dây nguồn 2 thiết bị trước khi nạp, nếu kết nối không gửi được nhớ check lại chân rxtx
- Động cơ DC hai chân IN1 IN2 phải kết nối với chân xung hoặc chân tương tự (mặc kệ lý thuyết)