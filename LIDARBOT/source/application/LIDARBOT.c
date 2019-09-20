#include "LIDARBOT.h"

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
}

/* USART BUFFER PROCESSING TASK */
ISR(TIMER0_COMPA_vect){ /* 320 us SOFTWARE INTERRUPT TASK RATE */
  /* Here we wait until there are atleast 50 bytes or close to 3 frames are in
   * the buffer to start reading. This method helps not reading the buffer
   * when there is no data and also keeps the interrupt time short.
   */
  if (buffercount>50) {
    readLIDAR();
  }
}

/* PROPULSION/NAVIGATION TASK */
ISR(TIMER1_COMPA_vect){ /* ~ 500 ms SOFTWARE INTERRUPT TASK RATE */
  /* We will have this ISR drive and steer for us. */
  navigate();
}
