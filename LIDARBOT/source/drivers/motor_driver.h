#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LIDAR_driver.h"
#include "../os_rios/cpufunctions.h"

// Available directions
#define LEFT    0
#define RIGHT   1
#define FORWARD 2
#define REVERSE 3

// Duty Cycle definitions
#define SLOW    30.0
#define COAST   50.0
#define FAST   100.0

// Steer magnitude
#define QUICK  0
#define NORMAL 1
#define LONG   2

unsigned char botmoving;

void motorinit(void);
void stop(void);
void steer(unsigned char direction);
void drive(unsigned char direction, double speed);

#endif
