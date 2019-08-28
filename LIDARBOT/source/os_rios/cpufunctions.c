#include "cpufunctions.h"

void clocksetup(void){
  /* CPU Clock setup
   * ---------------
   *
   * The clock source for the board is the on-board 16Mhz crystal that is
   * shipped with the Arduino Uno board. The system clock will be used for
   * PWM motor control and UART. So a clock pre-scalar has to be chosen such
   * that motor control and UART communication can occur smoothly. A 8 MHzclock
   * clock should yeild a minimal error UART communication and a motor PWM
   * that would work at 500 Hz. Therefore to get 4 Mhz, the clock needs to be
   * divided by a pre-scalar of 4. Hence we can set CLKPS1 a.k.a CKDIV4.
   */

  // CLKPCE lock needs to be written first to actually change the pre-scalar
  CLKPR = (1<<CLKPCE);
  CLKPR = (1<<CLKPS1);
}

void pwmtimer2setup(void){
  /* TIMER2 setup for PWM Motor Control
   * ----------------------------------
   *
   * From the datasheet:
   *
   * PWM_FREQUENCY = CPU_CLOCK_FREQUENCY/(N*256)
   *
   * where N is the clock pre-scalar to give us our required PWM frequency
   * The datasheet gives us select choices for this prescalar out of which the
   * value of N=32 would the most favourable for us (also standard). If we set
   * it to 32 we get:
   *
   * PWM_FREQUENCY = (4000000)/(32*256) ~ 488 HZ. (~ 2.04 ms)
   *
   * This frequncy should help the motor operate smoothly throughout the Duty
   * Cycle range controlled by OCR2A and OCR2B for the two motor banks.
   *
   */

  /* Set compare output mode to clear OCR2A and OCR2B on compare match */
  TCCR2A |=  (1<<COM2A1) | (1<<COM2B1);

  /* Waveform Generation Mode: WGM02 = 1, WGM01 = 1, WGM00 = 1 */
  TCCR2A |= (1<<WGM22) | (1<<WGM21) | (1<<WGM20);

  /* We will initialize PWM Duty Cycle to zero */
  OCR2A = 0;
  OCR2B = 0;

  // Timer pre-scalar for ~488 Hz is 32. We want this to be the last step since
  // this will actually start the timer counter.
  TCCR2B |=  (1<<CS21) | (1<<CS20);
}

void LIDARreadtimer0setup(void){
  /* TIMER0 setup for USART receive buffer read
   * ------------------------------------------
   * If we set up the timer in Clear Timer on Compare or simply CTC mode, the
   * frequency at which the timer will be cleared is described in the datasheet
   * as follows:
   *
   * OUTPUT FREQUENCY = CPU_CLOCK_FREQUENCY/(2*N*(1+OCR0A))
   *
   * where N is the clock pre-scalar to give us our required output frequency
   * The datasheet gives us select choices for this prescalar out of which the
   * value of N=64 would the most favourable for us (also standard). If we set
   * it to 64 and the OCR0A value to 20 we get:
   *
   * PWM_FREQUENCY = (4000000)/(2*64*(20+1)) ~ 1488 HZ. (672 us)
   *
   * That gives us a "task rate" of 670 us.
   */

  /* Set compare output mode to clear OCR2A and OCR2B on compare match */
  TCCR0A |=  (1<<COM0A1);

  /* Waveform Generation Mode: WGM02 = 0, WGM01 = 1, WGM00 = 0 -> CTC */
  TCCR0A |= (1<<WGM01);

  /* We will initialize OCR0A to  the value we have chosen */
  OCR0A = 20;

  /* Let us choose the timer pre-scalar of 64 (will start the timer) */
  TCCR0B |= (1<<CS01) | (1<<CS00);

  /* Finally lets enable the Timer 0 compare interrupt */
  TIMSK0 |= (1<<OCIE0A);
}

void hardwareserialsetup(void){
  /* Hardware UART Control Registers setup
   * -------------------------------------
   *
   * Registers needed to be configured from the datasheet:
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
   * Required settings for the TFmini LIDAR Sensor:
   *
   * Data bits - 8, Stop bits - 1, Parity Check - None
   */

  // Set 8 bit data
  UCSR0C = UCSR0C | ((1<<UCSZ01) | (1<<UCSZ00));

  // Set baud rate High and Low byte in UBBR
  UBRR0H = (CALCULATEDUBBR>>8);
  UBRR0L =  CALCULATEDUBBR;

  // Enable USART transmit and receive
  UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);
}

void softwareserialsetup(void){
  /* Software Serial Communication UASRT setup
   * -----------------------------------------
   *
   * The Arduino UNO has only one hardware UART. We will need another one for
   * debugging. Therefore we will need to emulate it and build a quick drver
   * that will allow us to transmit data while the RX pin is receiving. We can
   * use PORTB4 PIN 12 as software serial pin. Lets set that to high (IDLE).
   * Here is how a UART data frame with data 0xAA, a start, a stop bit and
   * no parity would look like for the Arduino:
   *
   *    IDLE | start bit |          8 data bits          |  stop bit | IDLE
   *    -------------------------------------------------------------------
   *
   *      1                1       1       1       1           1        1
   *    -----|           |---|   |---|   |---|   |---|   |-----------|-----
   *         |           |   |   |   |   |   |   |   |   |
   *         |           |   |   |   |   |   |   |   |   |
   *         |-----------|   |---|   |---|   |---|   |---|
   *               0           0       0       0       0
   *
   * The BUAD rate we want is 9600. That means we have 9600 bits per second.
   * That means 1 bit is transferred every (1 second/ 9600 bits) ~104 us.
   * We are clocked at 4000000 Hz. Each "tick" is 0.25 us. So this should be not
   * be a big deal to do with a regular pin if we just turn it ON and OFF to
   * simulate serial data. We will be using PORTB4 to do this which means
   * PIN 12 on the Arduino UNO board and a TTL USB driver on an external bread
   * board connected to a laptop to see the output.
   */

  /* Set the direction of PORTB4 to OUTPUT and hold it HIGH (IDLE) */
  DDRB  |= (1<<DDB4);
  PORTB |= (1<<PORTB4);
}

void faultledsetup(void){
  /* let us set up the built-in LED to light up when there is a fault */
  DDRB |= (1<<DDB5);     // Make the pin direction out
  PORTB &= ~(1<<PORTB5); // Turn it off so that it can be turned on when faulted
}
