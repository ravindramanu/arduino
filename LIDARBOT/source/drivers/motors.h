#ifndef MOTORS_H_
#define MOTORS_H_

#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../os_rios/cpufunctions.h"

// Available directions
#define LEFT 0
#define RIGHT 1

// Available speeds
#define SLOW 0
#define MEDIUM 1
#define FAST 2

void motorinit(void);
void stop(void);
void turn(unsigned char direction);
void forward(void);
void reverse(void);
void go(unsigned char speed);

#endif
