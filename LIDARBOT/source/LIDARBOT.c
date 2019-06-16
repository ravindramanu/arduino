#include "LIDARBOT.h"

void initbot(){
  // Initialize propulsion motors
  motorinit();
  // Initialize LIDAR sensing
  // UART_init();
  // Use LED on PIN 13 to show fault.
  DDRB |= (1<<DDB5);
  PORTB &= ~(1<<PORTB5);
}

int main(){

  // Initialize motor functions
  initbot();

  while (1) {

  }
}
