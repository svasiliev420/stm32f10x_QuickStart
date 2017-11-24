#---------------------------------------------------------------------------
#	
# Attention points when programming the F103 as well as the F100
# https://sites.google.com/site/mcu314/stm32-experiences/usb-and-freertos
#
#-MMD : Like -MD except mention only user header files, not system header files
#---------------------------------------------------------------------------
#VERSION			= Debug
VERSION		= Release
TOOL_PATH		= arm-none-eabi
SHELL			= bash

INC_PATH		= C:\Developer\ARM\gcc-arm-none-eabi\arm-none-eabi\include
LIB_PATH		= C:\Developer\ARM\gcc-arm-none-eabi\arm-none-eabi\include\lib

OOCD_VERSION			= 
OOCD_INTERFACE_CONFIG	= interface/ftdi_jtag.cfg
OOCD_TARGET_CONFIG		= target/stm32f1x.cfg

PRJ_NAME		= magic

SRC				= startup_stm32f10x_md.s
SRC				+= core_cm3.c
SRC				+= system_stm32f10x.c
SRC				+= misc.c
SRC				+= stm32f10x_rcc.c
SRC				+= stm32f10x_gpio.c
SRC				+= stm32f10x_usart.c
SRC				+= stm32f10x_tim.c
SRC				+= stm32f10x_it.c
SRC				+= main.c

INCLUDE			= $(INC_DIR)
INCLUDE			+= $(INC_PATH)

DEFINE			= STM32F10X_MD_VL
DEFINE			+= USE_STDPERIPH_DRIVER
DEFINE			+= F_CPU=8000000


OPTIMIZE	 	= 2
DEBUG			= 3
LD_SCRIPT		= stm32_flash.ld

#----------------------------------------------------------------------------
#	Секция параметров сборки
#----------------------------------------------------------------------------
CROSS_COMPILE	= arm-none-eabi-
CC				= $(CROSS_COMPILE)gcc
AS				= $(CROSS_COMPILE)gcc
LD				= $(CROSS_COMPILE)gcc
CPPC			= $(CROSS_COMPILE)g++
PROG			= openocd
SIZE			= $(CROSS_COMPILE)size
OBJCOPY			= $(CROSS_COMPILE)objcopy
OBJDUMP			= $(CROSS_COMPILE)objdump

CCFLAGS			= -Wall -mcpu=cortex-m3 -mthumb
CCFLAGS			+= $(addprefix -D,$(DEFINE)) $(addprefix -I,$(INCLUDE))
CCFLAGS			+= -ffunction-sections -fdata-sections

ifeq ($(VERSION),Debug)
CCFLAGS			+=  -g$(DEBUG) -O0
endif

ifeq ($(VERSION),Release)
CCFLAGS			+= -O$(OPTIMIZE)
endif

ASFLAGS			= -Wall -mcpu=cortex-m3 -mthumb
ASFLAGS			+= -Wa,-adhlns=$(<:.s=.lst)
ASFLAGS			+= $(addprefix -D,$(DEFINE)) $(addprefix -I,$(INCLUDE))

ifeq ($(VERSION),Debug)
ASFLAGS			+= -g$(DEBUG) -O0 -DUSE_FULL_ASSERT
endif

LDFLAGS			= -nostartfiles -T$(LD_SCRIPT)
LDFLAGS			+= -Wl,-Map,$(IMAGE).map,--cref -Wl,--gc-sections

CPPCFLAGS		= $(CCFLAGS)
CPPCFLAGS		+= -x c++

PROGFLAGS		= -d0
#PROGFLAGS		+= -f $(OOCD_INTERFACE_CONFIG) -f $(OOCD_TARGET_CONFIG)
PROGFLAGS		+= -c init -c targets
PROGFLAGS		+= -c "halt" -c "flash probe 0"
PROGFLAGS		+= -c "flash write_image erase $(IMAGE).elf 0x00000000 elf"
PROGFLAGS		+= -c "reset run" -c shutdown

SRC_DIR			= src
INC_DIR			= inc

OBJ_DIR			= obj
OUT_DIR			= bin

IMAGE			= $(OUT_DIR)/$(PRJ_NAME)

#----------------------------------------------------------------------------
#	Секция компиляции
#----------------------------------------------------------------------------

SOURCES		= $(wildcard $(addprefix $(SRC_DIR)/,$(SRC)))

ifneq ($(filter %.c,$(notdir $(SOURCES))),)
OBJECTS		+= $(addprefix $(OBJ_DIR)/,$(patsubst %.c, %.o,$(filter %.c,$(notdir $(SOURCES)))))
endif

ifneq ($(filter %.cpp,$(notdir $(SOURCES))),)
OBJECTS		+= $(addprefix $(OBJ_DIR)/,$(patsubst %.cpp, %.o,$(filter %.cpp,$(notdir $(SOURCES)))))
endif

ifneq ($(filter %.s,$(notdir $(SOURCES))),)
OBJECTS		+= $(addprefix $(OBJ_DIR)/,$(patsubst %.s, %.o,$(filter %.s,$(notdir $(SOURCES)))))
endif

ifneq ($(filter %.S,$(notdir $(SOURCES))),)
OBJECTS		+= $(addprefix $(OBJ_DIR)/,$(patsubst %.S, %.o,$(filter %.S,$(notdir $(SOURCES)))))
endif

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	@echo $<
	@echo "--"
	@echo "--"
	$(CC) $(CCFLAGS) -MMD -c $< -o $@

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	@echo $<
	@echo "123------------------------------------------------------"
	$(CPPC) $(CPPCFLAGS) -MMD -c $< -o $@

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.s $(SRC_DIR)/%.S
	@echo $<
	@echo ">>>"
	@echo ">>>"
	$(AS) $(ASFLAGS) -c $< -o $@

#---------------------------------------------------------------------------
#	Секция сборки
#---------------------------------------------------------------------------
#-- $(PROG) $(PROGFLAGS) 
Program: elf
	@echo ">>"
	@echo ">> COMPILE COMPLETE"
	@echo ">> "
	$(SIZE) $(IMAGE).elf

$(VERSION): bin hex size
	@echo "--------------------- COMPLETE -----------------------"
	


bin:$(IMAGE).bin

hex:$(IMAGE).hex

lst:$(IMAGE).lst

elf:$(IMAGE).elf

size:$(IMAGE).elf
	@echo $@
	@echo "--abc----------------------------------------------------"
	$(SIZE) $(IMAGE).elf

$(IMAGE).bin:$(IMAGE).elf
	@echo $@
	@echo "------------------------------------------------------"
	$(OBJCOPY) -O binary $< $@

$(IMAGE).hex:$(IMAGE).elf
	@echo $@
	@echo "------------------------------------------------------"
	$(OBJCOPY) -O ihex $< $@

$(IMAGE).lst:$(IMAGE).elf
	@echo $@
	@echo "------------------------------------------------------"
	$(OBJDUMP) -D $<  > $@

$(IMAGE).elf:$(OBJECTS)
	@echo $@
	@echo "@@"
	@echo "@@"
	$(LD) $(CCFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(patsubst %.o, %.d,$(OBJECTS))
	rm -f $(IMAGE).hex $(IMAGE).bin $(IMAGE).elf $(IMAGE).map
	@echo "--------------------- COMPLETE -----------------------"

.PHONY:  Program Debug Release cleanDebug cleanRelease

include $(wildcard $(OUT_DIR)/*.d)
