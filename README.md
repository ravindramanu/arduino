# Arduino Projects Repository
Author: Ravindra, Manu

This repository consists of personal projects of the author that were made
using a text editor and makefiles for a novice developer board
arduino uno rev3. The uno uses an atmel processor ATMega328P. Apart from
having a a few General Purpose IO, this board also supports PWM and UART.

Peripheral devices used in some projects include a LIDAR sensor called TFMini.
A very cost effective LIDAR sensor specifically made for pet projects. This
sensor can detect objects effectively in a 12 m range. Good enough for a
small LIDAR Robot.

To use any of the projects simply the clone the project or copy just
the makefile into an empty directory. In the directory that you copied the make file, 
you have several commands you that you can use. The first should be "make install" 
(only if you have not cloned the entire project). Other commands are,

make *command*

commands: clean saveboard help editsource install build object flash hex program

To know what each command does you can use "make help"
