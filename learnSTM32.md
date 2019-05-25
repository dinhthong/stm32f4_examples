# learn STM32
Dedicated to guide beginners in progress of learning STM32 discovery board.
STM32F407VG
# IDE for programming with STM32.
There are a lot of IDEs but the most suitable option for beginners would be 
The Keil uVision IDE
http://www2.keil.com/mdk5/uvision/

After downloading and installing the IDE, you can find video tutorials to properly set up the IDE for working with your board. I recommend strictly follow the tutorial to make your first program works. And then understand the why and IDE options later.
# Libraries and examples.

For programmer who wants to build application fast. ST provides a lot of good libraries along with examples. You won't need to spending time find examples on the Internet. 
1. STM Standard Peripheral Library from ST with examples
Main page for downloading from ST.
http://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32-standard-peripheral-libraries/stsw-stm32065.html
You can get documents and the library folder, code example with the link above !
2. STM HAL from ST with examples
This comes with CubeMX
http://www.st.com/en/embedded-software/stm32cubef4.html
3. STM32Snippets ( Only for L0 and F0 series )
http://www.st.com/en/embedded-software/stm32snippets.html?querycriteria=productId=LN1898
4. CMSIS.
5. Cube LL API.
# Documentations.
1. From STM32 http://www.st.com/en/microcontrollers/stm32f407vg.html

You can find everything to deeply understand and work with everything which your board provides. 
Reference manual explains all the peripherals as well as theirs register, you will also find memory mapping, electrical schematics.
2. From ARM.
# Books
# Video tutorials
http://www.st.com/content/st_com/en/support/learning/stm32-education.html
Others:
Embedded Systems with ARM Cortex-M Microcontrollers in Assembly Language and C
https://www.youtube.com/channel/UCY0sQ9hpSR6yZobt1qOv6DA

Quantum Leaps, LLC
https://www.youtube.com/channel/UCMGXFEew8I6gzjg3tWen4Gw

Fastbit Emebdded Brain Academy
https://www.youtube.com/channel/UCa1REBV9hyrzGp2mjJCagBg/feed

Patrick Hood-Daniel
https://www.youtube.com/user/anajonesr
# Recommended starting projects.
1. GPIO with LED blinking.
2. System Timer ( SysTick ).
3. PWM.
4. UART.
5. Interupts.
6. I2C.
7. SPI.
8. ADC and DAC.
9. Reset and clock control, clock tree.

And other topics and peripherals provided by STM32 discovery board. Can be found in Reference Manual. 
Along with that you can build your own projects by combining what you've learnt. Or communicate with other things outside the board like LCD, sensors, RTC ( real time clock ),...
# Advanced topics
1. Bare metal programming
2. ARM Cortex Booting Process.
3. Memory map.
4. Linker script and startup code.
5. Debugging.
6. Setup environment for programming and debugging using toolchains and Eclipse IDE.
7. Bootloader for STM32
8. FreeRTOS and porting FreeRTOS
9. GNU and Makefile
10. Manually porting to lower level programs from higher library. Minimize code density and resources.

#Good articles
http://blog.atollic.com/the-embedded-developer-is-dead-long-live-the-embedded-developer 
http://blog.atollic.com/12-great-tools-and-techniques-for-stm32-developers
