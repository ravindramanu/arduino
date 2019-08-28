#include "motor_driver.h"

void stop(){
  botmoving=0;

  /* STOP both motorbanks by setting in0, in1, in2 and in3 to zero. */
  // LEFT BANK: STOP
  PORTB &= ~(1<<PORTB1);
  PORTB &= ~(1<<PORTB0);

  // RIGHT BANK: STOP
  PORTD &= ~(1<<PORTD4);
  PORTD &= ~(1<<PORTD2);
  _delay_ms(500);
}

void setspeed(float setspeed){
  // Set speed.
  OCR2A = round((setspeed/100.00)*250.00);
  OCR2B = round((setspeed/100.00)*250.00);
}

void motorinit(void){
  // Configure motorpinout direction for arduino
  DDRB |= (1<<DDB3) | (1<<DDB1) | (1<<DDB0);
  DDRD |= (1<<DDD4) | (1<<DDD3) | (1<<DDD2);

  // FREEZE ALL MOTOR FUNCTIONS
  stop();

  // Setup cpu Timer 2 on the arduino to be used for our PWM signal generation
  pwmtimer2setup();
}

void steer(unsigned char direction, unsigned char magnitude){
  botmoving=1;
  if (direction == LEFT) {
    // LEFT BANK: Stop
    PORTB &= ~(1<<PORTB1);
    PORTB &= ~(1<<PORTB0);

    // RIGHT BANK: Forward
    PORTD |=  (1<<PORTD4);
    PORTD &= ~(1<<PORTD2);

  }
  else if (direction == RIGHT) {
    // LEFT BANK: Forward
    PORTB |=  (1<<PORTB1);
    PORTB &= ~(1<<PORTB0);

    // RIGHT BANK: Stop
    PORTD &= ~(1<<PORTD2);
    PORTD &= ~(1<<PORTD4);
  }
  else {
    stop();
  }

  // Set speed SLOW for turn
  setspeed(COAST);

  if (magnitude == QUICK) {
    _delay_ms(250);
  }
  else if(magnitude == NORMAL){
    _delay_ms(500);
  }
  else if(magnitude == LONG){
    _delay_ms(1000);
  }

  stop();
}

void drive(unsigned char direction, double speed) {
  botmoving = 1;
  if(direction == FORWARD){
    /* Set the wheels to spin forward */
    // LEFT BANK: Forward
    PORTB &= ~(1<<PORTB1);
    PORTB |=  (1<<PORTB0);

    // RIGHT BANK: Forward
    PORTD &= ~(1<<PORTD4);
    PORTD |=  (1<<PORTD2);
  }
  else if (direction == REVERSE) {
    /* Set the wheels to spin reverse */
    // LEFT BANK: Reverse
    PORTB |=  (1<<PORTB1);
    PORTB &= ~(1<<PORTB0);

    // RIGHT BANK: Reverse
    PORTD |=  (1<<PORTD4);
    PORTD &= ~(1<<PORTD2);
  }
  else {
    // Something is not right. Stop the vehicle.
    stop();
  }
  setspeed(speed);
}
