# Brief
- Keil C project utilizes ITM trace feature. To use the `#include <stdlib.h>` and retargeted printf. I modified retarget file, add definition for `_ttywrch(int ch)` to solve __use_no_semihosting was requested, but _ttywrch was referenced. The solution was found on https://programmersought.com/article/91751264627/
- This project is a clone from `19-swd_itm_trace`.
- Using Keil C IDE. Demonstration of ITM trace feature of ST-Link. It's simply just like USART via printf. Project configuration is the most important thing
# How
To run with ITM trace you have to go Debug session and 
View -> Serial Window -> Debug (printf) viewer
Then click Run.
