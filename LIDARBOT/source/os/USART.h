#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../os/cpufunctions.h"
#include "../drivers/LIDAR_driver.h"

#define BUFFERSIZE 1024 // Honestly this massive.

/* The webpage shows a way to use enums along with functions thats worth
 * remembering along with a few ther quick hacks. Below is a enum definition.
 * Therefore by definition :
 *
 * BUFFER_OK    = 0
 * BUFFER_EMPTY = 1
 * BUFFER_FULL  = 2
 *
 * We can use these status indicator to work with the buffer
 */
enum bufferStatus {BUFFER_OK, BUFFER_EMPTY, BUFFER_FULL};

/* The internet has a gazillion ways to implement a ring buffer that we need
 * for our UART RX and TX buffer implementation. But this one works fine from:
 * https://hackaday.com/2015/10/29/embed-with-elliot-going-round-with-circular-buffers/
 * It is quick and fast enough for our ISR to work with. In essence it is
 * managed ring buffer data structure. Below is the structure
 */
typedef struct Buffer {
  unsigned char data[BUFFERSIZE];
  unsigned char readindex;
  unsigned char writeindex;
} Buffer;

// Below is an instance of the struct a.k.a object of Buffer struct
volatile Buffer rxBuffer;
volatile unsigned int buffercount;

/* Now we make instances of this enum however instead of just a variale we will
 * make a generic function generic fucntion by passing a buffer pointer into it
 * along with the a byte of info to wrote to the buffer. LEGIT.
 * If the buffer is empty that would mean irrespective of where the indices are
 * with respect to the first element fo the array, they will be equal and this
 * essentially mean there is no data to read. We will indicate that by returning
 * the status as an ENUM variale but lets accept the variable address where we
 * want to store our answer a function parameter. This makes everything
 * complicated and not very easily readable however it is important that we
 * deal with the buffer in  swift manner since it will a contantly uses resource
 */

enum bufferStatus rxBufferRead(unsigned char * byte);
enum bufferStatus rxBufferWrite();
void usartinit(void);
void softwareserialTransmit(unsigned char byte);

#endif
