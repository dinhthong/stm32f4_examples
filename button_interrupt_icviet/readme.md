Để sử dụng ngắt cho button. Ta cấu hình như sau.
Cấu hình GPIO ( dễ ), với chế độ input để làm chân ngắt.
Tra trong bảng __Vectors của startup file.
Để biết tên để mà ta khai báo NVIC_InitStructure.NVIC_IRQchannel thì thêm chữ n phía sau của ngắt tương ứng. Khi đó thì tên function mà nó sẽ thực hiện khi gặp ngắt thì thêm Handler() vào, nên khai báo hàm này ở file stm32f4xx_it.c

# Source: https://icviet.vn/bai-2-ngat-ngoai-voi-stm32f4
Trong hàm main() tôi cho bóng LED D1 liên tục chớp tắt, 
khi xảy ra một sự kiện ngắt VD như bạn nhấn nút S1 thì LED D2 sẽ thay đổi trạng thái hiện tại của nó. 
Nút nhấn S1 tôi chọn mode falling nên thời điểm bạn nhấn xuống bóng LED D1 sẽ ngay lập tức thay đổi trạng thái, 
còn nút S2 S3 tôi chọn mode rising nên khi bạn nhấn xuống và nhả tay lên LED D3 D4 mới thay đổi trạng thái. 
Trong quá trình nhấn bạn sẽ cảm thấy nhấn không như lý thuyết cho lắm vì lúc ăn lúc không, 
điều đó do nút nhấn là tiếp điểm cơ khí khi bạn nhấn hay nhả sẽ gây nhiễu tiếp điểm, 
để tránh tình trạng này bạn có thể dùng một giải thuật gì đó để giải quyết vấn đề nhiễu này.