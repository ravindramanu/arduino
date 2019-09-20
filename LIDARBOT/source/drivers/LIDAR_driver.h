#ifndef LIDAR_DRIVER_H_
#define LIDAR_DRIVER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../drivers/motor_driver.h"
#include "../os/cpufunctions.h"
#include "../os/USART.h"

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
void navigate(void);

#endif
