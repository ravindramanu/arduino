#include "LIDARBOT.h"

int main(){
  //UART_Initialize();
  DDRB  |= (1<<DDB5);
  PORTB &= ~(1<<PORTB5);

    while (1) {
      /* code */
    }
}
