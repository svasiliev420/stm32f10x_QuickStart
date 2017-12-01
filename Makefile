#---------------------------------------------------------------------------
#	
#
#
#
#
#---------------------------------------------------------------------------
PRJ_NAME		= magic
VERSION			= Release
SHELL			= bash
TOOL_PATH		= arm-none-eabi

SRC_DIR			= Src
INC_DIR			= Inc
OBJ_DIR			= obj
OUT_DIR			= bin

##
## GCC ARM
##
INC_PATH	= C:\Developer\ARM\gcc-arm-none-eabi\arm-none-eabi\include
LIB_PATH	= C:\Developer\ARM\gcc-arm-none-eabi\arm-none-eabi\lib

INC_PATH	+= C:/Developer/SDK/STM32/CMSIS/Include
INC_PATH	+= C:/Developer/SDK/STM32/CMSIS/Device/ST/STM32F1xx/Include
INC_PATH	+= C:/Developer/SDK/STM32/STM32F1xx_HAL_Driver/Inc



DEFINE			= STM32F100xB
DEFINE			+= HSE_VALUE=8000000U
DEFINE			+= USE_FULL_LL_DRIVER
DEFINE			+= F_CPU=8000000

OPTIMIZE	 	= 0
DEBUG			= 3
LD_SCRIPT		= linker_flash.ld

#----------------------------------------------------------------------------
#	Compiler Options
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


INCLUDE			= $(INC_DIR)
INCLUDE			+= $(INC_PATH)
IMAGE			= $(OUT_DIR)/$(PRJ_NAME)

#----------------------------------------------------------------------------
#	
#----------------------------------------------------------------------------

vpath %.c .
vpath %.s .
vpath %.S .
vpath %.c Src
vpath %.c /C/Developer/SDK/STM32/STM32F1xx_HAL_Driver/Src 

SOURCES 	= $(wildcard Src/*.c)
SOURCES		+= $(wildcard /C/Developer/SDK/STM32/STM32F1xx_HAL_Driver/Src/*.c)
SOURCES		+= startup_stm32f10x_md_vl.s

ifneq ($(filter %.c,$(notdir $(SOURCES))),)
	OBJECTS		+= $(addprefix $(OBJ_DIR)/,$(patsubst %.c, %.o,$(filter %.c,$(notdir $(SOURCES)))))
endif

#ifneq ($(filter %.cpp,$(notdir $(SOURCES))),)
#OBJECTS		+= $(addprefix $(OBJ_DIR)/,$(patsubst %.cpp, %.o,$(filter %.cpp,$(notdir $(SOURCES)))))
#endif

ifneq ($(filter %.s,$(notdir $(SOURCES))),)
OBJECTS		+= $(addprefix $(OBJ_DIR)/,$(patsubst %.s, %.o,$(filter %.s,$(notdir $(SOURCES)))))
endif

ifneq ($(filter %.S,$(notdir $(SOURCES))),)
OBJECTS		+= $(addprefix $(OBJ_DIR)/,$(patsubst %.S, %.o,$(filter %.S,$(notdir $(SOURCES)))))
endif


$(OBJ_DIR)/%.o:%.c
	@echo "--"
	@echo "--"$<
	@echo "--" 
	@echo "--" 
	$(CC) $(CCFLAGS) -MMD -c $< -o $@ 

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	@echo $<
	@echo "123------------------------------------------------------"
	$(CPPC) $(CPPCFLAGS) -MMD -c $< -o $@

$(OBJ_DIR)/%.o:%.s
	@echo $<
	@echo ">>>"
	@echo ">>>" $<
	@echo ">>>"
	$(AS) $(ASFLAGS) -c $< -o $@

#---------------------------------------------------------------------------
#	Ñåêöèÿ ñáîðêè
#---------------------------------------------------------------------------
#-- $(PROG) $(PROGFLAGS) 
Program: elf
	@echo ">>>"
	@echo ">>>Build Complete"
	@echo ">>>"
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
