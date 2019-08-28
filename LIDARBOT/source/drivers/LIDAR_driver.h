/*******************************************************************************
*                              LIDAR_DRIVER.H                                  *
********************************************************************************
* The LIDAR driver program defines subroutines for reading data from the       *
* TFMINI LIDAR sensor. It has three main roles:                                *
*                                                                              *
* 1. Extraction of data from the USART receive software buffer                 *
* 2. Organising this data into a TFmini data frame as per specificaion         *
* 3. Parsing distance of the obstacle from a the robot and updating the        *
*    the globally used obstacle distance.                                      *
*                                                                              *
* The program accomplishes the extraction of data from the USART buffer by     *
* using a timed interrupt. Timer 0 is configured to interrupt the CPU every    *
* 672 us. This is because a byte is written into the buffer every 1040 ms      *
* since the baud rate is 9600. However the buffer is large enough to hold      *
* a 100 plus data frames. So the "task rate" for reading does not have to be   *
* too fast allowing the cpu some time to do other things but still has to be   *
* fast enough that he buffer does not get full real quick. Different values    *
* of this "task rate" were tried and 672 us seemed to work the best.           *
*                                                                              *
* The program organises the frame by syncing with the HEAD bytes of value 0x59 *
* from the datasheet and parsing the rest of the frame into distance, mode and *
* strength. When the checksum is finally collected the whole frame is verified *
* against it. The proximity of the obstacle and the validity of the data is    *
* updated in the end.                                                          *
*                                                                              *
* Author: Ravindra, Manu Srivatsa                                              *
* Date  : 21 Aug 2019                                                          *
*                                                                              *
********************************************************************************/
#ifndef LIDAR_DRIVER_H_
#define LIDAR_DRIVER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../os_rios/cpufunctions.h"
#include "../os_rios/UART.h"

// Some defines for the LIDAR driver
#define HEAD 0x59
#define VALID 0x20
#define INVALID 0xFF

/* This is how a frame is layed out in the datasheet */
typedef struct LIDARMODULESTRUCT {
  unsigned char headbyte1;
  unsigned char headbyte2;
  unsigned char distanceLow;
  unsigned char distanceHigh;
  unsigned char strengthLow;
  unsigned char strengthHigh;
  unsigned char mode;
  unsigned char spare;
  unsigned char checksum;
} LIDAR;

/* Make a TFMINI sensor construct */
LIDAR volatile tfmini;

/* Make an obstacle contruct, one that our LIDARBOT will use */
typedef struct OBJECTPROXIMITYSTRUCT{
  unsigned int proximity;
  unsigned char valid;
} OBJECT;

OBJECT volatile obstacle;

/* Public Funstions */
void lidarinit(void);
void readLIDAR(void);

#endif
