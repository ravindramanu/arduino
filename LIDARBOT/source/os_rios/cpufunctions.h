#ifndef CPUFUNCTIONS_H_
#define CPUFUNCTIONS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

/* CPU defines UART communication*/
#define SELECTEDCLOCK 4000000
#define SELECTEDBAUD 9600
#define TUNEDMICROADJUSTMENT 0
#define CALCULATEDUBBR ((SELECTEDCLOCK/16/SELECTEDBAUD)-1)
#define EMULATED_UARTDELAY_MICROSEC (1000000/SELECTEDBAUD) - TUNEDMICROADJUSTMENT

/* Public Funstions */
void clocksetup(void);
void pwmtimer2setup(void);
void hardwareserialsetup(void);
void softwareserialsetup(void);
void LIDARreadtimer0setup(void);
void faultledsetup(void);

#endif
