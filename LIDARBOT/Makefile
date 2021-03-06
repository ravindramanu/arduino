# Reference: https://www.instructables.com/id/How-to-get-started-with-Eclipse-and-AVR/

# Naming convention for the entire project
sourcename=LIDARBOT
subsourcefile1=UART_driver

# Board specific macros
hardwarepartnumber=atmega328p
frequency_selected=4000000UL
# baudrate=9600 # use PC default

################################################################################
# AVRDUDE specific macros                                                      #
# -----------------------                                                      #
# programmer type - AVRDUDE need to know what "programmer" to pick from its    #
# libraries for programming a specific board. In our case its arduino.         #
#                                                                              #
# serialdeviceport - IMPORTANT: Needs to be edited in each system. The arduino #
# shows up as ttyACMx. In my case this was ttyACM0.                            #
#                                                                              #
# avrdudecompilerflags - set of compile flags that we need to mention for our  #
# board. DF_CPU helps figure out badu rate for transmission of data. Factory   #
# shipped boards have fuses programmed to 8 MHz/ a factor of 8 = 1 MHz.        #
#                                                                              #
# memoryTypes - A list of memoryTypes present on the board. i.e. fuses that    #
# can be read so that we can back up the board and restore it at a later point #
#                                                                              #
# calibration -
# eeprom -
# flash -
# fuse -
# hfuse -
# lfuse -
# lock -
# signature -
#                                                                              #
# Below fuses are not a part of ATmega328P:                                    #
# application boot prodsig usersig                                             #
################################################################################
programmertype=arduino
# avrdudelogfile=$(CURDIR)/avrdudelogfile.log - Need to figure out how to save everything to file
avrdudesavedhexfilesfolder=saved_boards/boardsave_$$(date +%Y%m%d_%H%M%S)
serialdeviceport=/dev/ttyACM0
memorytypes=calibration eeprom efuse flash fuse hfuse lfuse lock signature

# Automatically generated files
softwareobject=objects/$(sourcename).o

softwareobjectelf=elf/$(sourcename).elf
softwarepayload=payload/$(sourcename).flash.hex

# Main source code file1
sourcecode=source/application/$(sourcename).c

################################################################################
# Compiler specific macros                                                     #
# ------------------------                                                     #
# The compiler we will use for the project is an avr specific gcc flavour.     #
# The following flags will be used for compiling:                              #
# -g       --> Enable debug information for say gdb                            #
# -Os      --> Optimize for code size (Embedded Application)                   #
# -Wall    --> Enable all warnings                                             #
# -Werror  --> Show warnings as errors                                         #
# -Wextra  --> Enable warning information that is otherwise not displayed      #
################################################################################

complilerused=avr-gcc
gcccompilerflags=-g -Os -Wall -Werror -Wextra
avrgcccompilerflags=-mmcu=$(hardwarepartnumber) -DF_CPU=$(frequency_selected)
avrlibpath=-I$(CURDIR)/libraries/

################################################################################
# PHONY Targets                                                                #
# -------------                                                                #
# List of phony targets. These commands do not build project deliverables.     #
# They are used solely for maintenance purposes. This is some clever makefile  #
# technique that is super useful.                                              #
################################################################################
.PHONY: clean saveboard help editsource install build object flash hex program

################################################################################
# help - command section                                                       #
# --------------------                                                         #
# A borrowed idea from the reference that helps any user understand what the   #
# different commands are for this makefile.                                    #
#                                                                              #
################################################################################
help:
	@echo 'Help documentation for make commands:                                 '
	@echo '-------------------------------------                                 '
	@echo '                                                                      '
	@echo 'help         Show this text.                                          '
	@echo '                                                                      '
	@echo 'saveboard    Read all known memory types from controller and write    '
	@echo '             to file.                                                 '
	@echo '                                                                      '
	@echo '             Available memory types for $(hardwarepartnumber):        '
	@echo '             $(memorytypes)                                           '
	@echo '                                                                      '
	@echo '             Useful option for saving current board state before      '
	@echo '             programming, in case the board is bricked after a        '
	@echo '             software reprogramming session.                          '
	@echo '                                                                      '
	@echo 'clean        Delete automatically created files.                      '
	@echo '                                                                      '
	@echo 'editsource   Edit the main source file with the authors editor.       '
	@echo '                                                                      '
	@echo 'elf          Create a .elf file of the source as $(sourcename).elf.   '
	@echo '                                                                      '
	@echo 'flash        Program $(sourcename).hex to controller flash memory.    '
	@echo '                                                                      '
	@echo 'hex          Create all hex files for flash, eeprom and fuses.        '
	@echo '                                                                      '
	@echo 'object       Create object file as $(sourcename).o                    '
	@echo '                                                                      '
	@echo 'program      Complete programming of the controller.                  '
	@echo '                                                                      '

# Reference: https://electronics.stackexchange.com/questions/66145/avr-how-to-program-an-avr-chip-in-linux

################################################################################
# clean - command section                                                      #
# -----------------------                                                      #
# Basically clean up things lying around in the folder. Run this command or    #
# phony target to reset the folder items to a good state when you have failed  #
# compilations, want to to reset everything to a good working order or just    #
# hate half completed files in your folder.                                    #
#                                                                              #
################################################################################
clean:
	@echo 'Cleaning house...                                                     '
	@rm -f *.hex *.o *.lst *.elf *.eeprom
	@rm -f objects/*.o assembly/*.lst elf/*.elf payload/*.hex
	@rm -f -r temp

################################################################################
# saveboard - command section                                                  #
# ---------------------------                                                  #
# For every memory type, this command will read the controller and save the    #
# memory on to a hex file. All hex files are saved in a folder.                #
################################################################################
saveboard:
# Totally not an elegant solution. But this will do the job.
	@mkdir -p temp
	@for memorytype in $(memorytypes) ; do \
		avrdude -p $(hardwarepartnumber) -c$(programmertype) -P$(serialdeviceport) -v -U $$memorytype:r:./$(hardwarepartnumber).$$memorytype.hex:i; \
	done
	@mv *.hex temp
	@mv temp $(avrdudesavedhexfilesfolder)

editsource:
# I am using atom for my editor
	@atom .

# Payload object needs drivers, os and main source file
object: cpufunctions.o USART.o motor_driver.o LIDAR_driver.o
	@echo '                                                                      '
	$(complilerused) $(gcccompilerflags)  $(avrgcccompilerflags) -Wa,-ahlmns=$(sourcename).lst -c -o $(softwareobject) $(sourcecode) $(avrlibpath)
	@mv $(sourcename).lst assembly/

cpufunctions.o:
		@echo '                                                                      '
		$(complilerused) $(gcccompilerflags)  $(avrgcccompilerflags) -Wa,-ahlmns=cpufunctions.lst -c -o objects/cpufunctions.o source/os/cpufunctions.c $(avrlibpath)
		@mv cpufunctions.lst assembly/

USART.o:
		@echo '                                                                      '
		$(complilerused) $(gcccompilerflags)  $(avrgcccompilerflags) -Wa,-ahlmns=USART.lst -c -o objects/USART.o source/os/USART.c $(avrlibpath)
		@mv USART.lst assembly/

motor_driver.o:
	@echo '                                                                      '
	$(complilerused) $(gcccompilerflags)  $(avrgcccompilerflags) -Wa,-ahlmns=motor_driver.lst -c -o objects/motor_driver.o source/drivers/motor_driver.c $(avrlibpath)
	@mv motor_driver.lst assembly/

LIDAR_driver.o:
	@echo '                                                                      '
	$(complilerused) $(gcccompilerflags)  $(avrgcccompilerflags) -Wa,-ahlmns=LIDAR_driver.lst -c -o objects/LIDAR_driver.o source/drivers/LIDAR_driver.c $(avrlibpath)
	@mv LIDAR_driver.lst assembly/

elf: object
	@echo '                                                                      '
	$(complilerused) $(gcccompilerflags) $(avrgcccompilerflags) -o $(softwareobjectelf) $(softwareobject) objects/cpufunctions.o objects/USART.o objects/motor_driver.o objects/LIDAR_driver.o
	@chmod a-x $(softwareobjectelf) 2>&1

hex: elf
	@echo '                                                                      '
	avr-objcopy -j .text -j .data -O ihex $(softwareobjectelf) $(softwarepayload)
	@echo '                                                                      '

dumpelf: elf
	@echo '                                                                      '
	avr-objdump -s -h $(softwareobjectelf)

build: clean hex

flash:
	@echo '                                                                      '
	avrdude -p$(hardwarepartnumber) -c$(programmertype) -P$(serialdeviceport) -v -U flash:w:$(softwarepayload)

program: build flash

install:
	@echo '                                                                      '
	@echo 'Creating source and object folders if not present.                    '
	@mkdir -p source objects libraries assembly saved_boards elf payload EEPROM
	@echo 'Please ensure that libraries are copied into the libraries folder.    '
	@echo '                                                                      '
