#---------------------------------------------------------------------------
#	
#
# Attention points when programming the F103 as well as the F100
# https://sites.google.com/site/mcu314/stm32-experiences/usb-and-freertos
#
# -MMD : Like -MD except mention only user header files, not system header files
#
#
#---------------------------------------------------------------------------
PRJ_NAME		= magic
VERSION			= Release


OUT_DIR			= bin
OBJ_DIR			= obj
INC_DIR			= inc
SRC_DIR			= src

##
## GCC ARM
##
INC_PATH	= C:/Developer/ARM/gcc-arm-none-eabi/arm-none-eabi/include
LIB_PATH	= C:/Developer/ARM/gcc-arm-none-eabi/arm-none-eabi/lib

##
## Use Developer
INC_PATH	+= C:/Developer/SDK/STM32/F10x/inc

##
## Use with Keil
##
#INC_PATH	+= C:/Keil_v5/ARM/Pack/ARM/CMSIS/4.5.0/CMSIS/Include
#INC_PATH	+= C:/Keil_v5/ARM/Pack/Keil/STM32F1xx_DFP/2.2.0/Device/Include
#INC_PATH	+= C:/Keil_v5/ARM/Pack/Keil/STM32F1xx_DFP/2.2.0/Device/StdPeriph_Driver/inc
#INC_PATH	+= C:/Keil_v5/ARM/Pack/Keil/STM32F1xx_DFP/2.2.0/RTE_Driver
#INC_PATH 	+= RTE
#INC_PATH 	+= RTE/Device/STM32F100RB/



DEFINE	 = STM32F10X_MD_VL
DEFINE	+= USE_STDPERIPH_DRIVER
DEFINE	+= F_CPU=8000000


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

CCFLAGS			= -Wall -mcpu=cortex-m3 -mthumb -std=c99
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
# Developer
vpath %.c .
vpath %.c src
vpath %.c /c/Developer/SDK/STM32/F10x/src
vpath %.s /c/Developer/SDK/STM32/F10x/src
# Keil
#vpath %.c RTE/Device/STM32F100RB 
#vpath %.s RTE/Device/STM32F100RB 
#vpath %.c /c/Keil_v5/ARM/Boards/ARM/MPS_CM3/Blinky
#vpath %.c /c/Keil_v5/ARM/Pack/Keil/STM32F1xx_DFP/2.2.0/Device/StdPeriph_Driver/src
#vpath %.c /c/Keil_v5/ARM/Pack/Keil/STM32F1xx_DFP/2.2.0/RTE_Driver

SOURCES 	= $(wildcard src/*.c)
SOURCES		+= startup_stm32f10x_md.s
SOURCES		+= system_stm32f10x.c
SOURCES		+= core_cm3.c
SOURCES 	+= misc.c
SOURCES		+= stm32f10x_rcc.c
SOURCES		+= stm32f10x_gpio.c
SOURCES		+= stm32f10x_usart.c
SOURCES		+= stm32f10x_tim.c

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
#	������ ������
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
