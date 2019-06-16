#include "motors.h"

void stop(){
  /* STOP both motorbanks by setting in0, in1, in2 and in3 to zero. */
  // LEFT BANK: STOP
  PORTB &= ~(1<<PORTB1);
  PORTB &= ~(1<<PORTB0);

  // RIGHT BANK: STOP
  PORTD &= ~(1<<PORTD4);
  PORTD &= ~(1<<PORTD2);
}

void forward(){
  /* Set the wheels to spin forward */
  // LEFT BANK: Forward
  PORTB |=  (1<<PORTB1);
  PORTB &= ~(1<<PORTB0);

  // RIGHT BANK: Forward
  PORTD |=  (1<<PORTD4);
  PORTD &= ~(1<<PORTD2);
}

void reverse(){
  /* Set the wheels to spin reverse */
  // LEFT BANK: Reverse
  PORTB &= ~(1<<PORTB1);
  PORTB |=  (1<<PORTB0);

  // RIGHT BANK: Reverse
  PORTD &= ~(1<<PORTD4);
  PORTD |=  (1<<PORTD2);
}

void turn(unsigned char direction){

  // Turn switch case
  switch (direction) {
    case 0: {
      // LEFT BANK: Stop
      PORTB &= ~(1<<PORTB1);
      PORTB &= ~(1<<PORTB0);

      // RIGHT BANK: Forward
      PORTD |=  (1<<PORTD4);
      PORTD &= ~(1<<PORTD2);
      break;
    }
    case 1: {
      // LEFT BANK: Forward
      PORTB |=  (1<<PORTB1);
      PORTB &= ~(1<<PORTB0);

      // RIGHT BANK: Stop
      PORTD &= ~(1<<PORTD2);
      PORTD &= ~(1<<PORTD4);
      break;
    }
    default: {
      stop();
      break;
    }
  }
}

void go(unsigned char speed) {
  // Set dutyCycle to pre determined amounts. BOT movements are limited.
  switch (speed) {
    case 0:{
      OCR2A = round((60.00/100.00)*250.00);
      OCR2B = round((60.00/100.00)*250.00);
      break;
    }
    case 1: {
      OCR2A = round((75.00/100.00)*250.00);
      OCR2B = round((75.00/100.00)*250.00);
      break;
    }
    case 2: {
      OCR2A = round((100.00/100.00)*250.00);
      OCR2B = round((100.00/100.00)*250.00);
      break;
    }
    default: {
      OCR2A = round((0.00/100.00)*250.00);
      OCR2B = round((0.00/100.00)*250.00);
      break;
    }
  }

  return;
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
