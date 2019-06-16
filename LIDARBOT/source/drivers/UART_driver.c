#include "UART_driver.h"

// tfmini special command matrix defines
#define ENTERCONFIG 0
#define STANDARDDATAFORMAT 1
#define OUTPUTREPRATE_10MS 2
#define DISTANCEUNITS_CM 3
#define DISTANCEMODE_LONG 4
#define BAUDRATE_9600 5
#define RESET 6

// tfmini head byte
#define HEAD 0x59

// We only care about the useful info in the dataframe
typedef struct LIDARMODULESTRUCT {
  unsigned char distLow;
  unsigned char distHigh;
  unsigned char strengthLow;
  unsigned char strengthHigh;
  unsigned char mode;
  unsigned char spare;
  unsigned char checksum;
} LIDAR;

LIDAR tfmini;

typedef struct LIDARDATASTRUCT {
  unsigned int distance;
  unsigned char dataIsGood;
} LIDAR_DATA;

LIDAR_DATA objectProximity;


/* The tfmini has an 8 byte command and 9 byte output as described on the
 * datasheet. Lets build a command matrix. We will make a 2D array of commands
 * we need borrowed from the datasheet.
 */

unsigned char command[7][9] = {
  // Configuration mode entry command
  {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x01, 0x02},
  // Output data format command
  {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x01, 0x06},
  // Output rep rate (10 ms)
  {0x42, 0x57, 0x02, 0x00, 0x0A, 0x00, 0x00, 0x07},
  // Unit of distance (cm)
  {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x01, 0x0A},
  // Distance mode: Long Distance
  {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x07, 0x11},
  // Set baud rate (9600)
  {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x00, 0x08},
  // RESET. DONE MESSED UP.
  {0x42, 0x57, 0x02, 0x00, 0xFF, 0xFF, 0xFF, 0xFF}
};

int validateChecksum(){
  if (tfmini.checksum == (tfmini.distLow + tfmini.distHigh + tfmini.strengthLow + tfmini.strengthHigh + tfmini.mode + tfmini.spare)) {
    return 1;
  }
  else
  {
    return 0;
  }
}


void readLIDAR(){
  // Check head and store data
  if (UART_Receive() == HEAD) {
    if (UART_Receive() == HEAD) {
      tfmini.distLow = UART_Receive();
      tfmini.distHigh = UART_Receive();
      tfmini.strengthLow = UART_Receive();
      tfmini.strengthHigh = UART_Receive();
      tfmini.mode = UART_Receive();
      tfmini.spare = UART_Receive();
      tfmini.checksum = UART_Receive();
      if (validateChecksum()) {
        unsigned int distance = (tfmini.distHigh<<8) + tfmini.distLow;
        unsigned int strength = (tfmini.strengthHigh<<8) + tfmini.strengthLow;
        // Data is good, update distance if not FF FF and strong
        if ((distance == 65535) || strength < 20 || strength > 2000) {
          objectProximity.dataIsGood = 0;
          return;
        }
        objectProximity.distance = distance;
        objectProximity.dataIsGood = 1;
        return;
      }
    }
  }
  return;
}

void UART_init(void){
  // Set up cpu for UART
  UART_Initialize();

  // We shall configure the LIDAR for operation
  for (int i = 0; i < 8; i++) { // Set Baud Rate
    UART_Transmit(command[BAUDRATE_9600][i]);
  }
  return;
}
