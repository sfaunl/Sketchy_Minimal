# Toolchain
TOOLCHAIN_PREFIX = /usr/bin/arm-none-eabi-
CC = $(TOOLCHAIN_PREFIX)gcc
AS = $(TOOLCHAIN_PREFIX)gcc -x assembler-with-cpp
LD = $(TOOLCHAIN_PREFIX)gcc
SZ = $(TOOLCHAIN_PREFIX)size
OD = $(TOOLCHAIN_PREFIX)objdump
OC = $(TOOLCHAIN_PREFIX)objcopy

EXE = main
BUILD_DIR = build

# Compiler Flags
ASM_SOURCES = startup_stm32h743zitx.s
C_SOURCES = system_stm32h7xx.c
C_SOURCES += src/main.c
C_SOURCES += \
lib/scheduler/scheduler.c \
lib/scheduler/list/linkedlist.c
C_SOURCES += \
lib/shal/common/shal_time.c \
lib/shal/common/systick/shal_systick.c \
lib/shal/common/systick/shal_systick_irq.c \
lib/shal/stm32h7/gpio/shal_stm32h7_gpio.c 
CINCLUDES = \
-Ilib/CMSIS/Device/ST/STM32H7xx/Include \
-Ilib/CMSIS/Include \
-Ilib/shal/ \
-Ilib/scheduler/
CDEFS = \
-DSTM32H743xx \
-DSTM32H7
MCU = -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb

CFLAGS += -O0 -g3
CFLAGS += -std=gnu11
CFLAGS += -Wall -Wextra -Wformat -Wformat
CFLAGS += $(MCU) $(CDEFS) $(CINCLUDES)

# Linker Flags
LDFLAGS += $(MCU)
LDFLAGS += -TSTM32H743ZITx.ld
LDFLAGS += --specs=nano.specs --specs=nosys.specs

# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

all: $(BUILD_DIR)/$(EXE).elf

# compile c files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# compile asm files
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

# link obj files into elf executable
$(BUILD_DIR)/$(EXE).elf: $(OBJECTS) | $(BUILD_DIR)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@
	$(SZ) $@

# create build directory
$(BUILD_DIR):
	mkdir $@

clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf

# Openocd Flags
OPENOCD_FLAGS = -f interface/stlink.cfg -f target/stm32h7x.cfg

flash: $(BUILD_DIR)/$(EXE).elf
	openocd $(OPENOCD_FLAGS) -c "program $(BUILD_DIR)/$(EXE).elf reset" -c "shutdown"
