# Quạt thông minh

**Quạt thông minh** là dự án tham gia cuộc thi **Sáng tạo thanh thiếu niên nhi đồng - Vifotec 2016** do nhóm tác giả: **_Huỳnh Đức Duy, Nguyễn Hà Sinh_** - học sinh lớp **_11A5 trường THPT Chuyên Lê Quý Đôn, Đà Nẵng_** thực hiện

## Giới thiệu

Nước ta có khí hậu khắc nghiệt, thường xuyên thay đổi, gây khó khắn trong dự đoán, phòng tránh, có thể gây ra nhiều bât lợi cho cuộc sống của con người. Cùng với việc mức tu nhập của nhân dân không thực sự cao để để lắp các thiết bị hiện đại như điều hòa,… Hơn nữa các thiết bị hiện đại đó còn có lượng điện tiêu tụ lớn, có thể làm ô nhiễm môi trường. 

Để giải quyết vấn đề trên, ý tưởng Quạt thông minh ra đời. Chiếc quạt vừa tích hợp các ưu điểm của các thiết bị hiện đại, đồng thời tiết kiệm điện và thân thiện với môi trường, thích hợp với các hộ gia đình nhỏ. 

![Quạt thông minh](https://i.imgur.com/6yOKGKI.jpg)


Các hình ảnh liên quan khác của sản phẩm có thể xem thêm ở: [Quạt thông minh's Imgur](https://imgur.com/a/H4m3Dky)

## Mô tả

##### Các chức năng chính

* Tự động bật/tắt quạt – chuyển đổi công suất
* Tự động di chuyển cổ quạt
* Chế độ quạt hơi nước
* Giao diện cho người dùng
* Điều khiển từ xa

## Thành phần

* Bo mạch Arduino Uno R3
* 5 cảm biến chuyển động PIR HR501
* Màn hình Lcd kích thước 20x4 với mạch giao tiếp i2c
* Bộ mạch 4 nút nhấn
* 4 Relay
* Cảm biến nhiệt độ - độ ẩm DHT11
* Động cơ Servo MG996R
* Động cơ DC cùng bộ nguồn (cuộn chạy – cuộn đề) – tụ điện
* Động cơ phun sương (từ động cơ bơm bồ cá)
* Đèn led thu hồng ngoại – Điều khiển từ xa hồng ngoại
* Còi
* Khung quạt
* Biến áp, tụ điện, điện trở, đi-ốt

## Cách thức hoạt động

Quạt sử dụng board Arduino Uno R3 kết hợp với các với các linh kiện, nguyên lý hoạt động như sau

-	Nhận tín hiệu từ cảm biến nhiệt độ - độ ẩm DHT11
-	Nhận tín hiệu từ điều khiển hồng ngoại thông qua đèn Led nhận hồng ngoại
-	Nhận tín hiệu từ 5 cảm biến chuyển động
-	Nhận dữ liệu từ các nút nhấn
-	Điều khiển 4 Relay
-	Điều khiển màn hình LCD qua giao tiếp i2c
-	Điều khiển còi
-	Điều khiển động cơ Servo

![Sơ đồ hoạt động của quạt](http://i.imgur.com/iVyYMfP.png)

## Hướng dẫn cài đặt và sử dụng

#### Hướng dẫn cài đặt

Là một sản phẩm được tạo ra nhằm giúp thuận tiện cho hoạt động hằng ngày của con người, nên quá trình cài đặt của quạt cũng vô cùng đơn giản: chỉ cần cắm nguồn điện, quạt sẽ tự động hoạt động theo các thông tin đã được người dùng cài đặt hoặc cài đặt mặc định.

#### Hướng dẫn sử dụng giao diện người dùng

Người dùng sẽ sử dụng 4 nút nhấn phía dưới màn hình LCD của quạt (Chọn – Lên – Xuống – Quay lại) để điều khiển con trỏ và thay đổi các thông tin cài đặt của quạt.
* Màn hình khởi động: Màn hình chờ của quạt hiển thị trong thời gian chờ bo mạch Galileo khởi động để quạt có thể chạy

![Màn hình khởi động](http://i.imgur.com/fnTVHnK.jpg)


* Trang chủ giao diện: Gồm 4 dòng: 3 dòng đầu, các lựa chọn để vào các phần cài đặt riêng biệt của quạt, dòng dưới cùng hiển thị các thông tin hiện tại: Nhiệt độ, Độ ẩm, Số quạt, Tình trạng phun sương

![Trang chủ giao diện](http://i.imgur.com/R5Eub9W.jpg)


* Trang cài đặt quay quạt: Có 3 cài đặt cho người dùng: Bật/Tắt chức năng tự động, quay với cài đặt bằng tay (với 2 tọa độ đầu và cuối), và tốc độ quay (thời gian trễ giữa các độ quay – tính bằng ms)

![Trang cài đặt quay quạt](http://i.imgur.com/ZjSyqhB.jpg)


* Trang cài đặt công suất: Có 3 mục cho người dùng: Bật/Tắt chức năng tự động, cài đặt nhiệt độ tương ứng với công suất (trỏ về trang <cài đặt nhiệt độ), và cài đặt công suất theo người dùng

![Trang cài đặt công suất](http://i.imgur.com/cyLh3sk.jpg)

* Trang cài đặt thời gian: Phần tiếp theo của cài đặt công suất: Hẹn giờ bật/tắt quạt

![Trang cài đặt thời gian](http://i.imgur.com/ovWQ4Sc.jpg)

* Trang cài đặt nhiệt độ (Trỏ từ trang cài đặt công suất): Với các cài đặt tương ứng với 3 số của quạt

![Trang cài đặt nhiệt độ](http://i.imgur.com/VzUeIhd.jpg)


* Trang cài đặt phun sương: Cũng có 3 cài đặt: Bật/Tắt chức năng tự động, cài đặt độ ẩm (tính bằng % - nếu dưới mức này thì sẽ tự động kích hoạt phun sương), hoặc điều khiển hệ thông phun sương bằng tay

![Trang cài đặt phun sương](http://i.imgur.com/lE5dk51.jpg)


Ở trang chủ, dùng nút **`lên`** / **`xuống`** để chuyển giữa các phần, **`chọn`** để truy cập.
Ở các trang cài đặt, dùng **`lên`** / **`xuống`** để chuyển giữa các cài đặt, sau đó **`chọn`** để truy cập cài đặt sau đó tiếp tục dùng **`lên`** / **`xuống`** để thay đổi giá trị, sau đó **`quay lại`** để trở về.

#### Hướng dẫn sử dụng điều khiển từ xa

![Giao diện điều khiển từ xa](http://i.imgur.com/5eax8ou.png)

Nút | Tác Dụng
--- | ---
CH-|Bật/Tắt tự động quay
CH|Bật/Tắt tự động công suất
CH+|Bật/Tắt tự động phun sương
\|<<|Giảm tốc độ quay
>>\||Tăng tốc dộ quay
>\|\||Bật/Tắt phun sương
-|Giảm số quạt
+|Tăng số quạt
EQ|Bắt đầu lưu vị trí quay bằng tay 
Từ 1 đến 9|Vị trí quay (có tác dụng khi kết hợp với EQ)
0,100+,200+|Không sử dụng

## Kết luận

Trong tương lai nhóm sẽ khắc phục tất cả những mặt tồn tại của sản phẩm trong khả năng của mình, và phát triển thêm các ý tưởng mới nâng cao hiệu suất cho sản phẩm.

# Chú thích thư mục

```board``` : Mã nguồn của bo mạch Arduino Uno R3

```libraries``` : Các thư viện ngoài được sử dụng

```test``` : Các đoạn mã dùng để kiểm tra tình trạng của từng thành phần trong sản phẩm
