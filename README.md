###### Пример Настройки Рабочего Окружения и Компиляции Проекта под MK stm32
###### Без использования IDE

---

> Качаем архив с набором инструментов для компиляции и разворачиваем его в корень диска.
> https://drive.google.com/file/d/19jE1_PhLk4vd9hqvoeid3CnB_HqEEH5D/view?usp=sharing
> Должно получится такая структура
```javascript
C:\Developer 
	\MinGW
	\ARM
	\ST_LINK
```
	
> Открываем редактор Переменных Окружения (удобней это сделать через вызов $sysdm.cpl)
> И добавляем слудующие пути к PATH
```javascript
C:\Developer\MinGW\bin
C:\Developer\ARM\gcc-arm-none-eabi\bin
C:\Developer\MinGW\msys\1.0\bin
C:\Developer\ST_LINK\ST-LINK_U
```

После внесения изменений может потребоваться перезапуск терминалов...
Открываем консоль и проверяем выполнение команд
```javascript
$make
$arm-none-eabi-gcc
```

---

###
### Сборка Проекта
###

> make clean
> make 
> make bin
> make hex

###
###  Ликбез
###
> EABI : https://ru.wikipedia.org/wiki/Двоичный_интерфейс_приложений
> MinGW: https://ru.wikipedia.org/wiki/MinGW
> Make : http://gnuwin32.sourceforge.net/packages/make.htm
---	
> Откуда брать заголовочне файлы и компилятор	
> https://launchpad.net/gcc-arm-embedded
> https://github.com/ARM-software/CMSIS
	
###	
### ARM Compiler Key
###	
> Тонкости компиляции 		: https://habrahabr.ru/post/191058/
> Управления линкером LD	: https://www.opennet.ru/docs/RUS/gnu_ld/gnuld-3.html

Compiler Options			: -mcpu=cortex-m3 -mthumb -mno-thumb-interwork -mfpu=vfp -msoft-float -mfix-cortex-m3-ldrd
Для вывода printf			: -specs=nosys.specs -specs=nano.specs -specs=rdimon.specs -lc -lrdimon
Для корректного вывода float: LDFLAGS += -u _printf_float