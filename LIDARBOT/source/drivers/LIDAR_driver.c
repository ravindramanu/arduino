/*******************************************************************************
*                              LIDAR_DRIVER.C                                  *
********************************************************************************
* Functions for LIDAR driver.                                                  *
*                                                                              *
* Author: Ravindra, Manu Srivatsa                                              *
* Date  : 21 Aug 2019                                                          *
*                                                                              *
********************************************************************************/
#include "LIDAR_driver.h"

/* Calculation variables used locally by the functions below */
unsigned char transferByte, sum, position;
unsigned int  signalstrength, distance;

void lidarinit(void){
  /* Setup timer 0 as trigger for lidar data reading */
  LIDARreadtimer0setup();
  /* Setup the built-in LED 13 as a way to indicate bad data reads */
  faultledsetup();
}

void parse(void){
  /* Let us sum the first eight bytes for checksum vericication */
  sum = (tfmini.headbyte1    + tfmini.headbyte2   +
         tfmini.distanceHigh + tfmini.distanceLow +
         tfmini.strengthHigh + tfmini.strengthLow +
         tfmini.mode         + tfmini.spare);

  /* We also need the signal strength */
  signalstrength = tfmini.strengthHigh + tfmini.strengthLow;

  /* tfmini datasheet says the distance is ineffective below 32 cm */
  distance = (tfmini.distanceHigh) + (tfmini.distanceLow);

  /* Checksum and signal srength */
  if((tfmini.checksum == sum)  &&
     (signalstrength  >   20)  &&
     (signalstrength  < 2000)  &&
     (tfmini.mode     = 0x02)  &&
     (distance        >   32)  &&
     (distance        < 1200)) {

    obstacle.proximity = distance;
    obstacle.valid = VALID;
    PORTB &= ~(1<<PORTB5);
  }
  else {
    obstacle.valid = INVALID;
    PORTB |= (1<<PORTB5);
  }
}

void readLIDAR(void){
  // Read a byte and parse it
  if (rxBufferRead(&transferByte) != BUFFER_OK) {
    return;
  }

  switch(position){
    /* Check if the first byte is HEAD */
    case 0: {
      if(transferByte == HEAD){
        tfmini.headbyte1=transferByte;
        position+=1;
      } else {
        position=0;
      }
      break;
    }

    /* Check if the second byte is also HEAD */
    case 1: {
      if(transferByte == HEAD){
        tfmini.headbyte2=transferByte;
        position+=1;
      } else {
        position=0;
      }
      break;
    }

    /* Gather the next 6 bytes*/
    case 2: {
      tfmini.distanceLow=transferByte;
      position+=1;
      break;
    }
    case 3: {
      tfmini.distanceHigh=transferByte;
      position+=1;
      break;
    }
    case 4: {
      tfmini.strengthLow=transferByte;
      position+=1;

      break;
    }
    case 5: {
      tfmini.strengthHigh=transferByte;
      position+=1;

      break;
    }
    case 6: {
      tfmini.mode=transferByte;
      position+=1;
      break;
    }
    case 7: {
      tfmini.spare=transferByte;
      position+=1;

      break;
    }

    /* Once we get the checksum we can immediately parse the distance */
    case 8: {
      tfmini.checksum=transferByte;
      parse();

      /* Reset the position to first byte and repeat */
      position=0;

      /* Reset the checksum to invalidate parsed frame */
      tfmini.checksum = 0;

      break;
    }
  }
}

/******************************************************************************/
/*                          INTERRUPT SERVICE ROUTINES                        */
/******************************************************************************/

/* TIMER 0 COMPARE MATCH ROUTINE TO READ FROM BUFFER */
ISR(TIMER0_COMPA_vect){
  /* We want our ISR to do nothing in the middle of a receive since this ISR
   * interrupts our receive USART ISR because of higher prioirty. Unfortunately
   * the Arduino does not allow NVIC or prioritizable interrupts since it is a
   * low spec board using the ATMega328p, so we have to work with what we have.
   * This means the use of flags and byte count can be one of the ways to tell
   * that the USART is in the middle of a receive operation and also when the
   * buffer is ready for a read. Here we wait until there are atleast 50 bytes
   * or close to 5 frames in the buffer to start reading. This method helps not
   * readin the buffer when there is no data and alos keeps the interrupt time
   * short.
   */
   if ((USART_is_receiving != 1) && (buffercount>50)) {
     readLIDAR();
  }
}
