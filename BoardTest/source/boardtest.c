/*******************************************************************************
* Boardtest.c                                                                  *
* -----------                                                                  *
*                                                                              *
* This program will blink an led on the arduino UNO REV 3 board there by       *
* serving as a quick boardtest program.                                        *
*                                                                              *
* The schematics show that the board has the following circuit built-in:       *
*                                                                              *
* PIN13 ------> 2.2k resistor ------> built-in LED ------> Common Ground       *
*                                                                              *
* So we can toggle the built-in LED on and off by switch suppying VCC to PIN13.*
* From the datasheet, PIN13 is driven by ATmega328P pin 5 of port B.           *
* The registers that control pin 5 are DDRB for direction and PORTB for output *
*                                                                              *
* Author: Ravindra, Manu                                                       *
* Date  : 13 April 2019                                                        *
********************************************************************************/

#include <avr/io.h>
#include <util/delay.h>

int main(void) {

   // Set direction first
   DDRB |= (1<<DDB5);

   // Now to toggle the output in a while loop
   while(1){
     PORTB ^= (1<<PORTB5);
     _delay_ms(10000); // 10 board seconds
   }
}
