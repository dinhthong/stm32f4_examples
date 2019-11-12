# Brief
- Original github repo: https://github.com/yuya-oc/stm32f4-discovery-cmake
- the libopencm3 is pre-buit thus the compile process is very fast. 
# stm32f4-discovery-cmake
STM32F4 Discovery samples with CMake

## Prerequisite
* [CMake](https://cmake.org/) 3.0 or later
* [GCC ARM Embedded](https://launchpad.net/gcc-arm-embedded) 4.9 or later
# Thong's comments
Toolchain-arm-none-eabi.cmake: explictly specify where the toolchain definion file is.
If you skip writing a toolchain file then CMake will default to your system compiler and things will start going wrong for you
## Build
```
$ git clone https://github.com/yuya-oc/stm32f4-discovery-cmake.git
$ cd stm32f4-discovery-cmake
$ git submodule update --init
$ cmake -DCMAKE_TOOLCHAIN_FILE=Toolchain-arm-none-eabi.cmake .
$ make libopencm3
$ make
```
