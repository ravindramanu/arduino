#include "USART.h"

void usartinit(void){
  // Initialize the hardware and software USART
  hardwareserialsetup();
  softwareserialsetup();
}

void softwareserialTransmit(unsigned char byte){
  /* Reference: https://www.avrfreaks.net/forum/software-serial-uart?page=all */
  /* The software serial transmit function is for debug only.
   * This function will send out any data that is passed into it on to a
   * software emulated UART on pin 12 of the Arduino.
   */
  unsigned char bitmask=0x01;

  // Send out start bit by turning PORTB4 OFF and wait ~104 ms (since baud is 9600)
  PORTB &= ~(1<<PORTB4);
  _delay_us(EMULATED_UARTDELAY_MICROSEC);

  // Toggle PORTB4 according to data bits in the reverse order LSB --> MSB
  while (bitmask) {
    if (byte & bitmask) {
      PORTB |= (1<<PORTB4);
      _delay_us(EMULATED_UARTDELAY_MICROSEC);
    }
    else {
      PORTB &= ~(1<<PORTB4);
      _delay_us(EMULATED_UARTDELAY_MICROSEC);
    }
    bitmask=bitmask<<1;
  }

  // Send out stop bit by turning PORTB4 ON and wait ~104 ms (since baud is 9600).
  // The line can stay ON since that would be IDLE.
  PORTB |= (1<<PORTB4);
  _delay_us(EMULATED_UARTDELAY_MICROSEC);
}

enum bufferStatus rxBufferRead(unsigned char * byte){
  // Check first before reading the buffer
  if ((rxBuffer.readindex) == (rxBuffer.writeindex)) {
    /* This would mean the buffer is empty. */
    softwareserialTransmit(0xAA);
    return BUFFER_EMPTY;
  }
  // Read the variable from buffer
  *byte=rxBuffer.data[rxBuffer.readindex];
  buffercount = buffercount - 1;

  /* Here is another hack to reset the value of index when we hit BUFFERSIZE
  * Using modulus operator gives us the remainder of a divison. Our BUFFERSIZE
  * is the maximum value of the size, so for every value that is less than
  * BUFFERSIZE the value fo the index from the below expression is itself.
  * When we reach BUFFERSIZE, the value will be zero.
  */
  rxBuffer.readindex = ((rxBuffer.readindex)+1) % BUFFERSIZE;
  return BUFFER_OK;
}


enum bufferStatus rxBufferWrite(){
  // That modulus thing again, although  we will write it out for readablity
  unsigned char insertindex = ((rxBuffer.writeindex)+1) % BUFFERSIZE;

  // Check if the next element if someting that needs to be read out. If it is
  // the buffer is definitely full.
  if (insertindex == (rxBuffer.readindex)) {
    /* This would mean the buffer is full. But what shoudl we do if the buffer
     * is full. Why just return. UDR0 itself is hardware buffered. So are very
     * less likely to lose any data. But in case we reach that point we just
     * wait until something is read.
     */
    softwareserialTransmit(0xFF);
    return BUFFER_FULL;
  }
  // Read the variable from UDR0 into the buffer
  rxBuffer.data[rxBuffer.writeindex] = UDR0;
  buffercount = (buffercount+1) % BUFFERSIZE;

  // Increment the index
  rxBuffer.writeindex = insertindex;
  return BUFFER_OK;
}
