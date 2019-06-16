#ifndef CPUFUNCTIONS_H_
#define CPUFUNCTIONS_H_

#include <avr/io.h>

void pwmtimer2setup(void);
void UART_Initialize(void);
unsigned char UART_Receive(void);
void UART_Transmit(unsigned char data);

#endif
