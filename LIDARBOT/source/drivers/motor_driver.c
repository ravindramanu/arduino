#include "motor_driver.h"

void stop(){

  botmoving=0;

  /* STOP both motorbanks by setting in0, in1, in2 and in3 to zero. */
  // RIGHT BANK: STOP
  PORTB &= ~(1<<PORTB2);
  PORTB &= ~(1<<PORTB0);

  // LEFT BANK: STOP
  PORTD &= ~(1<<PORTD4);
  PORTD &= ~(1<<PORTD2);
}

void setspeed(float setspeed){
  // Set speed.
  OCR2A = round((setspeed/100.00)*250.00);
  OCR2B = round((setspeed/100.00)*250.00);
}

void motorinit(void){
  // Propulsion timer 1 needs to be setup
  propulsiontimer1setup();

  // Configure motorpinout direction for arduino
  DDRB |= (1<<DDB3) | (1<<DDB2) | (1<<DDB0);
  DDRD |= (1<<DDD4) | (1<<DDD3) | (1<<DDD2);

  // FREEZE ALL MOTOR FUNCTIONS
  stop();

  // Setup cpu Timer 2 on the arduino to be used for our PWM signal generation
  pwmtimer2setup();
}

void steer(unsigned char direction){

  if (direction == RIGHT) {
    // LEFT BANK: Forward
    PORTD &= ~(1<<PORTD4);
    PORTD |=  (1<<PORTD2);

    // RIGHT BANK: Reverse
    PORTB &= ~(1<<PORTB2);
    PORTB |=  (1<<PORTB0);

  }
  else if (direction == LEFT) {
    // LEFT BANK: Reverse
    PORTD |=  (1<<PORTD4);
    PORTD &= ~(1<<PORTD2);

    // RIGHT BANK: Forward
    PORTB |=  (1<<PORTB2);
    PORTB &= ~(1<<PORTB0);
  }
  else {
    // Something is not right. Stop the vehicle.
    stop();
  }
  setspeed(SLOW);
}

void drive(unsigned char direction, double speed) {

  if(direction == FORWARD){
    /* Set the wheels to spin forward */
    // RIGHT BANK: Forward
    PORTB |=  (1<<PORTB2);
    PORTB &= ~(1<<PORTB0);

    // LEFT BANK: Forward
    PORTD &= ~(1<<PORTD4);
    PORTD |=  (1<<PORTD2);
  }
  else if (direction == REVERSE) {
    /* Set the wheels to spin reverse */
    // RIGHT BANK: Reverse
    PORTB &= ~(1<<PORTB2);
    PORTB |=  (1<<PORTB0);

    // LEFT BANK: Reverse
    PORTD |=  (1<<PORTD4);
    PORTD &= ~(1<<PORTD2);
  }
  else {
    // Something is not right. Stop the vehicle.
    stop();
  }
  setspeed(speed);

  botmoving = 1;
}
