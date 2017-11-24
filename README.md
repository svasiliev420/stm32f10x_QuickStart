
http://support.raisonance.com/node/431888
Migrating from GCC-ARM 4.6.x to 4.8.3) ?

1. Update all libraries (CMSIS, peripheral libraries, etc.). This is the cleaner way, and best for the long term, 
	but if you were using a very old version of the libraries, that might require a lot of work in the short term to 
	adapt your code that calls the libraries.
2. Edit core_cm3.c and comment out the definitions of functions __STREXH and __STREXB. These functions, 
	which hold the problem, are very rarely used and it is highly likely that your application doesn't need them. 
	If you do use them, then you should update the libraries (option 1 above).


*
* EABI 
* https://ru.wikipedia.org/wiki/Двоичный_интерфейс_приложений
*
* MinGW
* https://ru.wikipedia.org/wiki/MinGW
*	
* http://gnuwin32.sourceforge.net/packages/make.htm

Язык управления линкером LD
https://www.opennet.ru/docs/RUS/gnu_ld/gnuld-3.html

*
* https://community.particle.io/t/local-development-and-gdb-debugging-with-netbeans-a-step-by-step-guide/7829/2
*

* Environment Edit
* $ sysdm.cpl
	
	>> GNU ARM Embedded Toolchain:	https://launchpad.net/gcc-arm-embedded << ENV
		
Setup Debug + OpenOCD STM32F100
	doc	 : http://microsin.net/programming/ARM/openocd-manual-part1.html
	unix : http://gnutoolchains.com/arm-eabi/openocd/
	win  : https://gnu-mcu-eclipse.github.io/openocd/install/
	http://false.ekta.is/2016/01/using-netbeans-for-stm32-development-with-openocd/
	
	source [find interface/stlink-v1.cfg]
	source [find target/stm32f1x_stlink.cfg]
	
	$ openocd.exe -f openocd.cfg
	$ openocd 	  -f openocd.cfg -c "program usb-gadget0-stm32l1-generic.elf verify reset exit"
	
	$ st-util -p 4242
*	
* ARM 
*	Specific Compiler Options	:-mcpu=cortex-m3 -mthumb -mno-thumb-interwork -mfpu=vfp -msoft-float -mfix-cortex-m3-ldrd
*	Для вывода printf			: -specs=nosys.specs -specs=nano.specs -specs=rdimon.specs -lc -lrdimon
	Для корректного вывода float: LDFLAGS += -u _printf_float
	
*
* ARM Compiling
*	http://blog.podkalicki.com/how-to-compile-and-burn-the-code-to-stm32-chip-on-linux-ubuntu/

	$ arm-none-eabi-size -tA firmware.elf
	$ arm-none-eabi-objcopy -O binary firmware.elf firmware.bin
	$ st-flash write firmware.elf 0x8000000
	
	
	STM32F10X_MD_VL
	DSTM32F10X_MD
*	
* RESTORE USBSTOR
*
	Copy file from _restore/USBSTOR to C:\Windows\inf. 
	
	STM32F10X_MD_VL -DSTM32F10X_MD_VL
	
*
* https://habrahabr.ru/post/191058/
*
.text — скомпилированный машинный код;
.data — глобальные и статические переменные;
.rodata — аналог .data для неизменяемых данных;
.bss — глобальные и статические переменные, которые при старте содержат нулевое значение.
.comment — информация о версии компилятора;
.ARM.attributes — ARM-специфичные атрибуты файла	
