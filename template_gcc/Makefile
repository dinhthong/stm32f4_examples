# Makefile created by dinhthong

PROJECT_NAME=template
BUILDDIR = build
# from STM32F4xx_DSP_StdPeriph_Lib_V1.8.0\Libraries\CMSIS\Device\ST\STM32F4xx\Include
# system_stm32f4xx.h, stm32f4xx.h, stm32f4xx_conf.h
DEVICE = STM32F4xx
# STM32F4xx_DSP_StdPeriph_Lib_V1.8.0\Libraries\CMSIS\Include
# Core Cortex M4 functions support from CMSIS
CORE = CMSIS
# SPL library
SPL_DIR = STM32F4xx_StdPeriph_Driver
INC_DIR = inc
# USB middleware library if available
USB = inc/usb
SRC_DIR = src
#SOURCES += $(INC_DIR)/src/stm32f4_discovery.c
SOURCES = $(SRC_DIR)/startup_stm32f4xx.S
SOURCES += $(wildcard $(SRC_DIR)/*.c)
SOURCES += $(wildcard $(SPL_DIR)/src/*.c)
#SOURCES += 	$(SPL_DIR)/src/stm32f4xx_gpio.c \
			$(SPL_DIR)/src/stm32f4xx_i2c.c \
			$(SPL_DIR)/src/stm32f4xx_rcc.c \
			$(SPL_DIR)/src/stm32f4xx_spi.c \
			$(SPL_DIR)/src/stm32f4xx_exti.c \
			$(SPL_DIR)/src/stm32f4xx_syscfg.c \
			$(SPL_DIR)/src/misc.c

#SOURCES += stm32f4xx_it.c
OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

INCLUDES += -I$(DEVICE) \
			-I$(CORE) \
			-I$(SPL_DIR)/inc \
			-I$(INC_DIR) \
			-I$(USB)/inc \
			-I.

ELF = $(BUILDDIR)/$(PROJECT_NAME).elf
HEX = $(BUILDDIR)/$(PROJECT_NAME).hex
BIN = $(BUILDDIR)/$(PROJECT_NAME).bin

CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
GDB = arm-none-eabi-gdb

CFLAGS  = -O0 -g -Wall -I.\
   -mcpu=cortex-m4 -mthumb \
   -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
   $(INCLUDES) -DUSE_STDPERIPH_DRIVER
CFLAGS+= -DSTM32F40_41xxx
CFLAGS+= -DHSE_VALUE=8000000
LDSCRIPT = stm32_flash.ld
#LDFLAGS += -T$(LDSCRIPT) -mthumb -mcpu=cortex-m4 -nostdlib
#Support creating .map file after linking
#https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
LDFLAGS += -T$(LDSCRIPT) -mthumb -mcpu=cortex-m4 -nostdlib -Wl,-Map,$(BUILDDIR)/$(PROJECT_NAME).map
.PHONY: clean all debug flash

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(ELF): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)
# compile to object file but do not link
$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.S
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

flash: $(BIN)
	st-flash write $(BIN) 0x8000000

debug: $(ELF)
	$(GDB) -tui $(ELF)

all: $(HEX) $(BIN)

clean:
	rm -rf build
