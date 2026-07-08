# Mini ADAS (Advanced Driver Assistance Systems) 🚗🛡️

Mini ADAS là một hệ thống hỗ trợ lái xe nâng cao phiên bản thu nhỏ, được xây dựng trên nền tảng vi điều khiển cấu trúc ARM Cortex-M4 (**STM32F401RET6**). Dự án này tích hợp các cảm biến đo khoảng cách và phát hiện vật cản nhằm mục đích xử lý dữ liệu thời gian thực, cảnh báo va chạm và thực hiện việc điều khiển xe tự hành tránh chướng ngại vật một cách thông minh (Sensor Fusion & Object Tracking).

---

## 📌 Các Tính Năng Chính
- **Phát hiện & Theo dõi vật cản (Object Tracking):** Định vị và giám sát vị trí các chướng ngại vật xung quanh xe.
- **Hợp nhất dữ liệu cảm biến (Sensor Fusion):** Kết hợp thông tin từ nhiều cảm biến khoảng cách để tăng độ chính xác và giảm nhiễu.
- **Hệ thống cảnh báo va chạm sớm:** Đưa ra tín hiệu cảnh báo chủ động khi khoảng cách tới vật cản vượt ngưỡng an toàn.
- **Thuật toán tránh vật cản tự động:** Điều khiển hệ thống bánh lái / động cơ phản hồi tức thời để phòng ngừa va chạm.

---

## 🛠️ Kiến Trúc Phần Cứng (Hardware)
- **Bộ vi điều khiển trung tâm:** STM32F401RET6 (Nucleo-F401RE hoặc bo mạch tùy chỉnh).
- **Cảm biến khoảng cách:** Cảm biến siêu âm (HC-SR04) / Cảm biến hồng ngoại (IR) / Cảm biến ToF (Time-of-Flight).
- **Hệ thống truyền động:** Động cơ DC kèm Encoder, IC điều khiển động cơ (L298N / Driver chuyên dụng).

---

## 💻 Cấu Trúc Thư Mục Dự Án
```text
Mini-ADAS/
├── Core/
│   ├── Inc/             # Các file header cấu hình hệ thống và ngoại vi
│   └── Src/             # Mã nguồn chính xử lý thuật toán (main.c, stm32f4xx_it.c,...)
├── Drivers/             # Thư viện CMSIS và HAL Driver chuẩn từ ST
├── .settings/           # Cấu hình IDE cấu trúc dự án
├── testproject.ioc      # File cấu hình phần cứng chân cấu hình trên STM32CubeMX
├── STM32F401RETX_FLASH.ld  # Bộ liên kết Flash (Linker Script)
└── README.md            # Tài liệu hướng dẫn dự án
