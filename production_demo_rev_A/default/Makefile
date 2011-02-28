###############################################################################
# Makefile for the project db101_demo
###############################################################################

## General Flags
PROJECT = db101_demo
MCU = atmega1281
TARGET = db101_demo.elf
CC = avr-gcc

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -Wno-unused   -DF_CPU=7372800UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=db101_demo.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\power_driver" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\rtc_driver" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\Sound" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\termfont_lib" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\terminal_lib" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\timing_lib" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\uart_driver" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\backlight_driver" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\common" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\fifo_lib" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\forms_lib" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\gfx" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\joystick_driver" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\memblock_lib" -I"D:\users\mthomas\develop\avr\db101_devel\db101_gcc\production_demo_rev_A\..\Picture_lib" 

## Libraries
LIBS = -lm 

## Objects that must be built in order to link
OBJECTS = walkabout.o configuart.o flashpics.o gameoflife.o lcdcontrast.o main.o memory.o slideshow.o smokeydemo.o snake.o sounddemo.o terminal.o clock.o backlight_driver.o fifo_lib.o widgets_lib.o forms_lib.o dialog_lib.o s6b1713_driver.o lcd_lib.o popup_lib.o gfx_lib.o joystick_driver.o memblock_lib.o picture_lib.o power_driver.o rtc_driver.o sound_driver.o song_lib.o termfont_lib.o terminal_lib.o timing_lib.o uart_driver.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) db101_demo.hex db101_demo.eep db101_demo.lss size

## Compile
walkabout.o: ../walkabout.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

configuart.o: ../configuart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

flashpics.o: ../flashpics.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

gameoflife.o: ../gameoflife.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcdcontrast.o: ../lcdcontrast.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

memory.o: ../memory.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

slideshow.o: ../slideshow.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

smokeydemo.o: ../smokeydemo.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

snake.o: ../snake.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sounddemo.o: ../sounddemo.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

terminal.o: ../terminal.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

clock.o: ../clock.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

backlight_driver.o: ../../backlight_driver/backlight_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

fifo_lib.o: ../../fifo_lib/fifo_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

widgets_lib.o: ../../forms_lib/widgets_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

forms_lib.o: ../../forms_lib/forms_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

dialog_lib.o: ../../forms_lib/dialog_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

s6b1713_driver.o: ../../gfx/s6b1713_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_lib.o: ../../gfx/lcd_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

popup_lib.o: ../../gfx/popup_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

gfx_lib.o: ../../gfx/gfx_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

joystick_driver.o: ../../joystick_driver/joystick_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

memblock_lib.o: ../../memblock_lib/memblock_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

picture_lib.o: ../../Picture_lib/picture_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

power_driver.o: ../../power_driver/power_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

rtc_driver.o: ../../rtc_driver/rtc_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sound_driver.o: ../../Sound/sound_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

song_lib.o: ../../Sound/song_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

termfont_lib.o: ../../termfont_lib/termfont_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

terminal_lib.o: ../../terminal_lib/terminal_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timing_lib.o: ../../timing_lib/timing_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart_driver.o: ../../uart_driver/uart_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) db101_demo.elf dep/* db101_demo.hex db101_demo.eep db101_demo.lss db101_demo.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

