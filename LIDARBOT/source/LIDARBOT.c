#include "LIDARBOT.h"

void initbot(){
  /* Initialize all robot functions */
  clocksetup();  // Master clock setup
  usartinit();   // Communication setup
  lidarinit();   // LIDAR read setup
  motorinit();   // Propulsion setup
  sei();         // Enable global interrupts
}

int main(){
  // Initialize BOT
  initbot();
  _delay_ms(2000);

  while (1) {
    if ((obstacle.valid == VALID) && (obstacle.proximity < 45) && (botmoving == 1)) {
      stop();
      steer(RIGHT, QUICK);
    }
    else if (botmoving == 0) {
      drive(FORWARD,SLOW);
    }
  }
}
