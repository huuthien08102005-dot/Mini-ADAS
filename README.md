# Thiết Kế và Thực Hiện Hệ Thống Phát Hiện và Cảnh Báo Tiền Va Chạm Cho Xe Tự Lái (Mini ADAS)

[![Embedded C](https://img.shields.io/badge/Language-Embedded_C-blue.svg)](https://en.wikipedia.org/wiki/Embedded_C)
[![MCU](https://img.shields.io/badge/MCU-STM32F401RE-orange.svg)](https://www.st.com/en/microcontrollers-microprocessors/stm32f401re.html)
[![Framework](https://img.shields.io/badge/Framework-CMSIS_Register_Level-green.svg)](https://www.arm.com/technologies/cmsis)
[![IDE](https://img.shields.io/badge/IDE-STM32CubeIDE-blueviolet.svg)](https://www.st.com/en/development-tools/stm32cubeide.html)

Dự án nghiên cứu và phát triển mô hình hỗ trợ cảnh báo va chạm cơ bản (**Mini ADAS - Advanced Driver Assistance System**) tích hợp trên xe tự hành. Hệ thống xử lý dữ liệu cảm biến thời gian thực, đưa ra các cấp độ cảnh báo bằng âm thanh/hình ảnh và tự động can thiệp phanh điện tử nhằm hạn chế tối đa nguy cơ va chạm.

---

## 📌 Các Tính Năng Cốt Lõi
- **Đo khoảng cách thời gian thực:** Quét vật cản phía trước chính xác với dải đo từ $2\text{ cm} - 400\text{ cm}$, sai số $\le \pm 3\text{ mm}$.
- **Thuật toán FSM (Finite State Machine):** Phân loại trạng thái di chuyển thông minh thành 3 vùng: `SAFE` (An toàn), `WARNING` (Chú ý), và `DANGER` (Nguy hiểm).
- **Phanh khẩn cấp tự động (AEB):** Tự động ngắt xung điều khiển động cơ khi khoảng cách dưới ngưỡng nguy kịch ($< 30\text{ cm}$).
- **Cảnh báo đa cấp độ:** Tần suất nhấp nháy LED và tiếng bíp của còi Buzzer thay đổi linh hoạt, dồn dập tỷ lệ nghịch với khoảng cách vật cản.
- **Giám sát thị giác:** Tích hợp Camera OV7670 hỗ trợ thuật toán phát hiện chuyển động cơ bản (Motion Detection) tại các vùng mù.
- **Truyền thông không dây:** Đóng gói dữ liệu định dạng ký tự ASCII gửi qua Bluetooth HC-05 để giám sát trực quan trên Smartphone/PC hành trình.

---

## 🛠️ Kiến Trúc Phần Cứng (Hardware Architecture)

### 1. Thành Phần Linh Kiện
* **Khối xử lý trung tâm:** Board mạch STM32 Nucleo-F401RE (Lõi ARM Cortex-M4, 84 MHz).
* **Khối cảm biến:** Cảm biến siêu âm HC-SR04 & Mô-đun Camera OV7670.
* **Khối chấp hành:** Mạch cầu H L298N điều khiển cặp động cơ DC dòng đỉnh 2A.
* **Khối cảnh báo:** Hệ thống LED chỉ thị (Vàng/Đỏ) & Còi báo động Buzzer chủ động.
* **Khối giao tiếp:** Mô-đun Bluetooth HC-05 (Chuẩn v2.0+EDR).

### 2. Sơ Đồ Kết Nối Chân (Pinout Configuration)
Toàn bộ hệ thống được cấu hình thủ công ở mức thanh ghi thay vì sử dụng thư viện HAL nhằm tối ưu hóa bộ nhớ và triệt tiêu độ trễ phần mềm (`Non-OS Super-loop`):

| Tên ngoại vi | Chức năng chân | Chân STM32 | Chế độ cấu hình (Register Mode) |
| :--- | :--- | :--- | :--- |
| **HC-SR04** | Chân phát xung (Trig)<br>Chân nhận xung (Echo) | `PA0`<br>`PA1` | GPIO Output<br>GPIO Input |
| **Bluetooth HC-05** | Truyền dữ liệu (TX)<br>Nhận dữ liệu (RX) | `PA9`<br>`PA10` | Alternate Function (`USART1`) |
| **UART Debug** | Truyền dữ liệu (TX)<br>Nhận dữ liệu (RX) | `PA2`<br>`PA3` | Alternate Function (`USART2`) |
| **Mạch cầu H L298N**| Điều khiển bánh trái<br>Điều khiển bánh phải | `PB3`, `PB4`<br>`PB5`, `PB6` | GPIO Output<br>GPIO Output |
| **LED & Buzzer** | LED Chú ý (Warning)<br>LED Nguy hiểm (Danger)<br>Còi báo (Buzzer) | `PA4`<br>`PA5`<br>`PA6` | GPIO Output<br>GPIO Output<br>GPIO Output |
| **Camera OV7670** | Chân dữ liệu (D0)<br>Cấu hình SCCB | `PB8`<br>`PB9`, `PB10` | GPIO Input<br>GPIO Output (Giả lập I2C) |

---

## 💻 Cấu Trúc Phần Mềm (Software Architecture)

Mã nguồn được tổ chức theo mô hình kiến trúc phân tầng chức năng (**Functional Layered Architecture**):
- **Tầng ứng dụng (Application Layer):** Điều phối vòng lặp `main.c`, quản lý máy trạng thái FSM.
- **Tầng trình điều khiển (Peripheral Driver Layer):** Gồm các file driver cấu hình thanh ghi trực tiếp (`MODER`, `PUPDR`, `ODR`, `IDR`) thông qua thư viện CMSIS.
- **Tầng cấu hình phần cứng (HAL Layer):** Thiết lập xung nhịp nội HSI 16 MHz và cấu hình hệ thống.

### Ngưỡng Logic Điều Khiển FSM:
1. **Trạng thái SAFE ($d > 100\text{ cm}$):** Động cơ tiến bình thường, tắt toàn bộ LED và Buzzer.
2. **Trạng thái WARNING ($50\text{ cm} \le d \le 100\text{ cm}$):** Bật Warning LED, xe tiếp tục di chuyển, chuẩn bị giảm tốc.
3. **Trạng thái DANGER ($d < 50\text{ cm}$):** Bật Danger LED, còi báo hú dồn dập. Nếu $d < 30\text{ cm}$, ngắt lập tức động cơ để phanh khẩn cấp.

---

## 📈 Đánh Giá Hệ Thống & Hướng Phát Triển

### Ưu điểm
- Chương trình biên dịch gọn nhẹ, thực thi thời gian thực tốc độ cao do can thiệp trực tiếp mức thanh ghi.
- Thuật toán FSM phân vùng cảnh báo rõ ràng, đáp ứng tốt kiểm thử thực nghiệm hành trình.

### Hạn chế hiện tại
- Hệ thống đang dùng cơ chế Polling liên tục và hàm Delay Blocking làm tiêu tốn tài nguyên CPU.
- Đo khoảng cách siêu âm bằng định thời phần mềm (Software Timing) dễ bị nhiễu môi trường.

### Hướng nâng cấp (Roadmap)
- [ ] Chuyển đổi sang sử dụng kỹ thuật **Timer Input Capture** để đo xung Echo chính xác tuyệt đối.
- [ ] Tích hợp bộ băm xung **PWM** để điều khiển giảm tốc mượt mà thay vì ON/OFF động cơ cơ bản.
- [ ] Triển khai hệ điều hành thời gian thực **RTOS** và quản lý các tác vụ bằng cơ chế ngắt (Interrupt).
- [ ] Mở rộng thuật toán xử lý ảnh trên Camera OV7670 và thiết kế màn hình hiển thị LCD/OLED trực tiếp trên xe.

---

## 👥 Thành Viên Thực Hiện
Dự án được thực hiện trong khuôn khổ môn học *Nhập môn Hệ thống nhúng (ELT3240_1)* - Trường Đại học Công nghệ, Đại học Quốc gia Hà Nội:
- **Đặng Trung Nguyên** (MSV: 23021877) - Thiết kế mạch, lắp ráp xe, kiểm tra kiểm thử và viết báo cáo.
- **Trần Hữu Thiện** (MSV: 23021921) - Thiết kế thuật toán, phát triển mã nguồn nhúng trên STM32.
