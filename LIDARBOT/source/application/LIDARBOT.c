#include "LIDARBOT.h"

unsigned char navigating;

void navigate(void){
  // BasicBot: See if there is anything ahead and mindlessly turn right
  if ((obstacle.valid == VALID) && (obstacle.proximity < 45) && (botmoving == 1)) {
    /* An obstacle lies ahead. Turn right and hope the bot is cleared it. */
    stop();
  }
  else if ((obstacle.valid == INVALID) && (botmoving == 1)) {
    /* The bot is blind. Poor reflectivity or other such reasons mentioned in the datasheet has
     * caused the lidar data to go bad whilst moving. Stop.
     */
    stop();
  }
  else if ((obstacle.valid == INVALID) && (botmoving == 0)) {
    /* The bot is blind. Poor reflectivity or other such reasons mentioned in the datasheet has
     * caused the lidar data to go bad. Turn right and see if the data becomes better.
     */
    steer(RIGHT);
  }
  else if ((obstacle.valid == VALID) && (obstacle.proximity < 45) && (botmoving == 0)) {
    /* Data is good but obstacle is in colision distance. Steer clear. */
    steer(RIGHT);
  }
  else if ((obstacle.valid == VALID) && (obstacle.proximity > 45) && (botmoving == 0)){
    /* No obstacles ahead. Coast away.*/
    drive(FORWARD, COAST);
  }
}

void initbot(){
  /* Initialize all robot functions */
  clocksetup();  // Master clock setup
  usartinit();   // Communication setup
  lidarinit();   // LIDAR read setup
  motorinit();   // Propulsion setup
}

int main(){
  // Initialize BOT
  initbot();
  // A small delay to give the bot owner time to step away
  _delay_ms(2000);

  // Enable global interrupts - Bot Key ON
  sei();

  while (1) {
    /* Keep Running */
  }
}

/******************************************************************************/
/*                     INTERRUPT SERVICE ROUTINES                             */
/******************************************************************************/
/* USART RECEIVE TASK */
ISR(USART_RX_vect){ /* Asynchronous Hardware Interrupt */

  // Set recieve flag to one
  USART_is_receiving = 1;

  /* We want our interrupt service routine to be quick as possible. Which is why
   * all this routine will do is write to the ring buffer and get out. However,
   * we need our cpu to be quicker so that elements from that buffer are emptied
   * faster than this ISR can populate the buffer. This was one of the prime
   * reasons why the chosen clock was 4 Mhz.
   */

  if((rxBufferWrite() != BUFFER_OK)){
    readLIDAR();
    rxBufferWrite();
  }

  // Set it off when done
  USART_is_receiving = 0;
}

/* USART BUFFER PROCESSING TASK */
ISR(TIMER0_COMPA_vect){ /* 320 us SOFTWARE INTERRUPT TASK RATE */
  // Set reading flag to on
  reading_under_progress = 1;

  /* We want our ISR to do nothing in the middle of a receive since this ISR
   * interrupts our receive USART ISR because of higher prioirty. Unfortunately
   * the Arduino does not allow NVIC or prioritizable interrupts since it is
   * receive is in progress flag. We wil also check if we are readng lidar data
   * low spec board using the ATMega328p, so we have to work with what we have.
   * This means the use of flags and byte count can be one of the ways to tell
   * that the USART is in the middle of a receive operation and also when the
   * buffer is ready for a read. Here we wait until there are atleast 50 bytes
   * or close to 3 frames in the buffer to start reading. This method helps not
   * readin the buffer when there is no data and alos keeps the interrupt time
   * short.
   */

   if ((USART_is_receiving != 1) && (buffercount>50)) {
     readLIDAR();
   }

   // Set the flag off
   reading_under_progress = 0;
}

/* PROPULSION/NAVIGATION TASK */
ISR(TIMER1_COMPA_vect){ /* ~ 500 ms SOFTWARE INTERRUPT TASK RATE */
  /* We will have this ISR drive and steer for us. This should be the least
   * important ISR even though it is meant for controlling the robot because
   * the cpu is a lot faster the the vehicle and we can barely see a change. We
   * will need to give it said low prioirty by using the "dont do anything when
   * receive is in progress flag. We wil also check if we are readng lidar data.
   */
   if ((reading_under_progress != 1) && (USART_is_receiving != 1)) {
     navigate();
   }
}
