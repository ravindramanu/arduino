/*******************************************************************************
* UART_driver.c                                                                *
* -------------                                                                *
*                                                                              *
* Registers needed to be configured from the datasheet:                        *
*                                                                              *
* UCSRnc - USART Control and Status Register C                                 *
*          -> UMSELn bit controls USART mode                                   *
* UCSRnA - USART Control and Status Register A                                 *
*          -> U2Xn bit controls doublespeed                                    *
* UBRRn  - USART Baud Rate Register                                            *
*                                                                              *
* Recommended baud rate settings from the datasheet:                           *
*                                                                              *
* 9600 kbps - U2Xn = 1, UBRRn = 12 with fosc = 1 Mhz. (0.2 % error)            *
* Selection criterion: Minimal error in data receiving using available clock.  *
*                                                                              *
* Our Baud caculation: UBRRn = (fosc/8*BAUD) - 1                               *
*                            = (1000000/8*9600) - 1                            *
*                            ~ 12                                              *
*                                                                              *
* Required settings for the TFmini Sensor:                                     *
*                                                                              *
* Data bits - 8, Stop bits - 1, Parity Check - None                            *
*                                                                              *
* Author: Ravindra, Manu                                                       *
* Date  : 13 April 2019                                                        *
********************************************************************************/
#include "UART_driver.h"

void UART_Initialize(void){
  // Disable USART transmit and receive
  UCSR0B &= ~((1<<TXEN0) | (1<<RXEN0));
  // Clock polarity
  UCSR0C &= ~(1<<UCPOL0);
  // Make sure USART is in asynchronous mode
  UCSR0C &= ~((1<<UMSEL00) | (1<<UMSEL01));
  // Make sure the parity is disabled
  UCSR0C &= ~((1<<UPM00) | (1<<UPM01));
  // Make sure USART is set to double speed for the baud rate needed
  UCSR0A |= (1<<U2X0);
  // Set baud rate = 12 -> 0x000C
  UBRR0L = 0x0C;
  UBRR0H = 0x00;
  // Enable USART transmit and receive
  UCSR0B |= (1<<TXEN0) | (1<<RXEN0);
  // Set 1 bit stop bit
  UCSR0C &= ~(1<<USBS0);
  // Set 8 bit data
  UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
  UCSR0C &= ~(1<<UCSZ02);

  return;
}
