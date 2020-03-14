# todo_add
+ simple SPI loopback
+ ADC conversion (ADC injection based on MotCtrl)
+ Hall timer reading, speed and position measurement.
# Preference
+ STM32CubeF4-master.zip (from ST's CubeHAL page).
+ Xuan Thiep tutorials

# note
These notes are re-transcripted from My own notes from PPT file
## UART

mới vào trong hàm main phải có dòng lệnh nói uart nào đó nhận dữ liệu HAL_UART_Receive_IT(&huart3, &get, 1) : cho phép nhận dữ liệu thông qua ngắt chỉ 1 lần. Nên sau mỗi khi nhận thì phải khai báo lại
Từ đó trở đi mới xảy ra các sự kiện như 
```
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==huart3.Instance)
	{
		HAL_UART_Receive_IT(&huart3, &get, 1)
	}
}
```
Để viết nó nhận dữ liệu tiếp
---
Còn nếu dùng chế độ Receive and Transmit bt?
Mình đã làm ok bằng cách cho nó luôn truyền và luôn nhận ở trong hàm main
---
Phím Ctrl + Space: nhận gợi ý …
---
Chuẩn bị làm với DMA. ok làm được khá là dễ.
Chú ý ở đây là làm theo XT thì hình như cái ngắt Receive Cplt ko phải dùng cho DMA. -> tìm hiểu 
static void UART_DMAReceiveCplt(DMA_HandleTypeDef *hdma)
## SPI
Projects Management 
Work with project at its main directory
Using branch feature. Revert feature of Github.
If the project works. Explain how it work and what it does on NOTE. and create new directory for it. For example. 11.1 SPI using Interupt.
Delete this branch on the main project.

Steps for making this project works.Xuan Thiep tutorial

CRC calculation: technique to ensure the data transferring process is perfect

## VCP
chú ý cắm cổng micro USB còn lại của kit, không thôi đừng hỏi tại sao 
nó hiện ra lỗi chấm than, cứ mò đi nạp code rồi debug vài lần là được
Chú ý làm theo hướng dẫn của XT, có thêm vài code vào các hàm được tạo ra.
Nếu tự tọa bằng CubeMX mà không chạy thì lên đọc comment trên youtube của XT thay cái giá trị define gì đó.
