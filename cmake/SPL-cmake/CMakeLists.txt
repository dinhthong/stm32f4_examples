project(led_blinking C ASM)
cmake_minimum_required(VERSION 3.8)


set(CMAKE_ASM_COMPILER arm-none-eabi-as)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
#set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_AR arm-none-eabi-ar)

set(COMMON_FLAGS "-DSTM32F40_41xxx -DHSE_VALUE=8000000 -DSTM32F407VG -DUSE_STDPERIPH_DRIVER                                      \
    -mcpu=cortex-m4 -specs=nosys.specs -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

#set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m4")

set(HEX_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.bin)

set(USER_INCLUDE users)
#set(USER_TOOL_INCLUDE users/tool)
set(CMSIS_DEVICE_INCLUDE driver/CMSIS/Device/ST/STM32F4xx/Include)
set(CMSIS_INCLUDE driver/CMSIS/Include)
set(STD_INCLUDE driver/STM32F4xx_StdPeriph_Driver/inc)

file(GLOB_RECURSE USER_INCLUDE_F ${USER_INCLUDE}/*.h)
#file(GLOB_RECURSE USER_TOOL_INCLUDE_F ${USER_INCLUDE}/tool/*.h ${USER_INCLUDE}/tool/*.hpp)
file(GLOB_RECURSE CMSIS_DEVICE_INCLUDE_F ${CMSIS_DEVICE_INCLUDE}/*.h)
file(GLOB_RECURSE CMSIS_INCLUDE_F ${CMSIS_INCLUDE}/*.h)
file(GLOB_RECURSE STD_INCLUDE_F ${STD_INCLUDE}/*.h)

file(GLOB_RECURSE USER_SOURCES users/*.c)
#file(GLOB_RECURSE USER_TOOL_SOURCES users/tool/*.c users/tool/*.cpp)
file(GLOB_RECURSE STD_SOURCES driver/STM32F4xx_StdPeriph_Driver/src/*.c)
file(GLOB_RECURSE CMSIS_SYSTEM driver/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c)
file(GLOB_RECURSE CMSIS_STARTUP driver/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc_ride7/startup_stm32f40xx.s)

#add_definitions(-DSTM32F40_41xxx)

set(SOURCE_FILES ${USER_INCLUDE_F} ${CMSIS_DEVICE_INCLUDE_F} ${CMSIS_INCLUDE_F} 
    ${STD_INCLUDE_F} ${USER_SOURCES} ${STD_SOURCES} ${CMSIS_STARTUP} ${CMSIS_SYSTEM} STM32F407_FLASH.ld)

include_directories(${USER_INCLUDE} ${CMSIS_DEVICE_INCLUDE} ${CMSIS_INCLUDE} ${STD_INCLUDE}
        /usr/lib/arm-none-eabi/include)

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections -T${CMAKE_SOURCE_DIR}/STM32F407_FLASH.ld -Wl,-Map=${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.map")

add_executable(${PROJECT_NAME}.elf ${SOURCE_FILES})
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.map")


add_custom_command(TARGET ${PROJECT_NAME}.elf PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/build
   COMMENT "Creating build directory")
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
   COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
   COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${HEX_FILE}\nBuilding ${BIN_FILE}")

#add_custom_target(flash
#    stm32flash -w ${BIN_FILE} /dev/ttyUSB0)
#add_custom_target(chmod
#    sudo chmod 777 /dev/ttyUSB0)

