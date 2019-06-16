#include "cpufunctions.h"

void pwmtimer2setup(void){
  /* From the datasheet:
   *
   * PWM_FREQUENCY = CPU_CLOCK_FREQUENCY/(N*256)
   *
   * where N is the clock pre-scalar to give us our required PWM frequency
   * The datasheet gives us select choices for this prescalar out of which the
   * value of N=8 would the most favourable for us (also standard). If we set
   * it to 8 we get:
   *
   * PWM_FREQUENCY = (1000000)/(8*256) ~ 490 HZ. (~ 2.04 ms)
   *
   * Every tick of the CPU clock increments the value of the timer 2. When the
   * value of the timer matches OC2RA and OC2RB, the OC2A and OC2B pins are
   * switched. This means every 255 ticks is one timer period for our motor.
   * If we make this TOP value limited to 250, then our time period will be
   * exactly 2 ms and our frequency will be exactly 500 HZ.
   * Timer 2 can be setup as a "Fast PWM" timer. Set compare output mode to
   * clear OCR2A on compare match.
   */

  // Set compare output mode to clear OCR2A and OCR2B on compare match
  TCCR2A |=  (1<<COM2A1) | (1<<COM2B1);

  // Waveform Generation Mode: WGM02 = 0, WGM01 = 1, WGM00 = 1
  TCCR2A |= (1<<WGM22) | (1<<WGM21) | (1<<WGM20);

  // We will initialize PWM Duty Cycle to zero
  OCR2A = 0;
  OCR2B = 0;

  // Timer pre-scalar for ~490 Hz is 8. We want this to be the last step since
  // this will actually start the timer counter.
  TCCR2B |=  (1<<CS21);
}

void UART_Initialize(void){
  /* Registers needed to be configured from the datasheet:
   *
   * UCSRnc - USART Control and Status Register C
   *          -> UMSELn bit controls USART mode
   * UCSRnA - USART Control and Status Register A
   *          -> U2Xn bit controls doublespeed
   * UBRRn  - USART Baud Rate Register
   *
   * Recommended baud rate settings from the datasheet:
   *
   * 9600 kbps - U2Xn = 1, UBRRn = 12 with fosc = 1 Mhz. (0.2 % error)
   * Selection criterion: Minimal error in data receiving using available clock.
   *
   * Our Baud caculation: UBRRn = (fosc/8*BAUD) - 1
   *                            = (1000000/8*9600) - 1
   *                            ~ 12
   *
   * Required settings for the TFmini Sensor:
   *
   * Data bits - 8, Stop bits - 1, Parity Check - None
   */
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

// Datasheet Implementation
unsigned char UART_Receive(void){
  // We will busy wait in this instance for simplicity
  while (!(UCSR0A & (1<<RXC0)));
  // Collect data from UDR
  return UDR0;
}

void UART_Transmit(unsigned char data){
  // We will busy wait in this instance for simplicity
  while (!(UCSR0A & (1<<UDRE0)));
  // Write data to UDR
  UDR0 = data;
}
