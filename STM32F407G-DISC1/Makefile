# Inspired by https://github.com/STM32-base/STM32-base
#         and https://github.com/cpq/bare-metal-programming-guide

help : ## Display this help
	@awk \
		'BEGIN { FS = "[[:space:]]*:.*##" } \
		/^([a-zA-Z_-]+)[[:space:]]*:.*##/ \
		{ printf ("\033[36m%-30s\033[0m %s\n", $$1, $$2) }' \
		$(MAKEFILE_LIST)
.PHONY : help

DEVICE ?= STM32F4

SERIES_FOLDER  = STM32F4
SERIES_CPU     = cortex-m4
SERIES_ARCH    = armv7e-m+fp
MAPPED_DEVICE ?= STM32F407xx

# Default values for project directories
BIN_DIR ?= ./bin
DRV_DIR ?= ./drv
INC_DIR ?= ./inc
OBJ_DIR ?= ./obj
SRC_DIR ?= ./src

# Input files
# SRC ?=
# SRC += $(SRC_DIR)/*.c
SRC := $(wildcard $(SRC_DIR)/*.c)

# Output files
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
BIN_FILE_NAME ?= firmware.bin
ELF_FILE_NAME ?= firmware.elf
OBJ_STARTUP_FILE_NAME ?= startup_$(MAPPED_DEVICE).o
OBJ_SYSTEMINIT_FILE_NAME ?= system_stm32f4xx.o
OBJ_FILE_NAME ?= startup_$(MAPPED_DEVICE).o

BIN_FILE_PATH = $(BIN_DIR)/$(BIN_FILE_NAME)
ELF_FILE_PATH = $(BIN_DIR)/$(ELF_FILE_NAME)
OBJ_STARTUP_FILE_PATH = $(OBJ_DIR)/$(OBJ_STARTUP_FILE_NAME)
OBJ_SYSTEMINIT_FILE_PATH = $(OBJ_DIR)/$(OBJ_SYSTEMINIT_FILE_NAME)
OBJ_FILE_PATH = $(OBJ_DIR)/$(OBJ_FILE_NAME)

TOOLCHAIN_PREFIX ?= arm-none-eabi-
CC      = $(TOOLCHAIN_PREFIX)gcc
LD      = $(TOOLCHAIN_PREFIX)ld
AS      = $(TOOLCHAIN_PREFIX)gcc
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy

# Compilation flags
# Flags - Warning Options
CFLAGS  = -Wall
CFLAGS += -Wextra
# Flags - Debugging Options
CFLAGS += -g
# Flags - Optimization Options
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
# Flags - Preprocessor options
CFLAGS += -D$(MAPPED_DEVICE)
# Specify the name of the target CPU.
CFLAGS += -mcpu=$(SERIES_CPU)
# Generate code for Thumb state.
CFLAGS += -mthumb
CFLAGS += -I$(INC_DIR)

# Linker flags
LD_FLAGS  = -specs=nosys.specs
LD_FLAGS  += -lc -lgcc
# Specify the linker script.
# LD_FLAGS += -T$(DRV_DIR)/STM32F411VETX_FLASH.ld
# LD_FLAGS += -T$(DRV_DIR)/link.ld
LD_FLAGS += -T$(DRV_DIR)/STM32F407VGTX_FLASH.ld
LD_FLAGS += -Wl,--gc-sections
# Create map file
LD_FLAGS += -Wl,-Map=$(ELF_FILE_PATH).map

# Startup file
DEVICE_STARTUP = $(DRV_DIR)/startup_$(MAPPED_DEVICE).s
DEVICE_SYSTEMINIT = $(DRV_DIR)/system_stm32f4xx.c

all : $(BIN_FILE_PATH)

build : $(BIN_FILE_PATH) ## Build the binary file
.PHONY : build

dump : $(ELF_FILE_PATH) ## Show the hex dump of the firmware.elf
	$(TOOLCHAIN_PREFIX)objdump -h $^
.PHONY : dump

clean : ## Clean the build files
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
.PHONY : clean

flash : $(BIN_FILE_PATH) ## Flash the board
	st-flash --reset write $< 0x8000000
.PHONY : flash



$(ELF_FILE_PATH) : $(OBJ) $(OBJ_STARTUP_FILE_PATH) | $(BIN_DIR)
	@echo "\nLinking $@"
	$(CC) $(CFLAGS) $(LD_FLAGS) $^ -o $@

$(BIN_FILE_PATH) : $(ELF_FILE_PATH)
	@echo "\nBuilding $@"
	$(OBJCOPY) -O binary $< $@

$(OBJ_STARTUP_FILE_PATH) : $(DEVICE_STARTUP) | $(OBJ_DIR)
	@echo "\nBuilding $@"
	$(CC) -c $(CFLAGS) $(LD_FLAGS) $< -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "\nBuilding $@"
	$(CC) -c $(CFLAGS) $(LD_FLAGS) $< -o $@





$(BIN_DIR) :
	mkdir $(BIN_DIR)

$(OBJ_DIR) :
	mkdir $(OBJ_DIR)

