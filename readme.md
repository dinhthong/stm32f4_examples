My STM32 projects codehub.
# Brief
+ STM32F407 Discovery has ARM Cortex-M4 core inside.
+ Most of projects in this repository use `STM Standard Peripheral Library V1.0.2 (aka SPL)`
# Note
## Example template project
### Keil IDE
`template_keilc` is a pre-configured project using Keil uVision IDE.
To run the project. You have to correctly set the Include Paths to the folder contains files used in the project.  
In uVision. Click `'Options for Target'` -> `C/C++` -> `Include Paths`.  
### GNU toolchain
`template_gcc` is sample project for building with GNU toolchain.
## Project examples
### Many project examples already come with libraries from STMicroelectronics, i.e:
+ STM32-Discovery_FW_V1.10/Project
+ STM32F4xx_DSP_StdPeriph_Lib_V1.3.0/Project
+ STM32Cube_FW_F4_V1.16.0
+ ARM CMSIS DSP library and its examples. `STM32F4xx_DSP_StdPeriph_Lib_V1.8.0\Libraries\CMSIS\DSP_Lib\Source`
+ STM32_USB-Host-Device_Lib_V2.2.0/Project
+ STM32_USB-FS-Device_Lib_V4.0.0/Projects
### Other example projects
+ https://github.com/mfauzi/STM32F4/tree/master/STM32F4%20Discovery%20Software%20Examples
## Keywords
+ CMSIS RTOS.
+ ST STM32F4xx Application Notes.
+ ARM Math library - DSP (Digital Signal Processing).
# Todos
+ [] Clean up/refactor the code (for e.g stm32f4xx.h files in all directories)
+ [] Merging all my small ARM-Cortex based MCU projects
+ [] Different IDE (e.g IAR, Eclipe, TrueStudio,...), and different libraries (libopencm3)
+ [] Detail tutorials
+ [] SDIO, FSMC, USB mouse and keyboard, ARM Cortex M4 exception generation and handling, TFT LCD SPI, EEPROM
+ [] ARM DSP and math functions