/*******************************************************************************

First credit to where its due and legalities. This is where this rtos was
borrowed from originally and modified as per my understanding of it. The
site explains an RTOS scheduler in a simple way. Really useful for beginners
like me. A perfect step into creating a custom os or into industry standards
like Free RTOS.

Web links:
http://www.cs.ucr.edu/~vahid/rios/
http://www.cs.ucr.edu/~vahid/rios/rios_avr.htm

Copyright (c) 2012 UC Regents. All rights reserved.

    Developed by: Frank Vahid, Bailey Miller, and Tony Givargis
    University of California, Riverside; University of California, Irvine
    <http://www.riosscheduler.org>http://www.riosscheduler.org

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal with the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

        * Redistributions of source code must retain the above
          copyright notice, this list of conditions and the following
          disclaimers.
        * Redistributions in binary form must reproduce the above
          copyright notice, this list of conditions and the following
          disclaimers in the documentation and/or other materials
          provided with the distribution.
        * Neither the names of any of the developers or universities nor
	  the names of its contributors may be used to endorse or
	  promote products derived from this Software without
	  specific prior written permission.


    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE CONTRIBUTORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS WITH THE SOFTWARE.

(http://opensource.org/licenses/NCSA)

********************************************************************************/
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

// Stnadard includes
#include <math.h>

// Avr specific includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


/* Tick principle
 * --------------
 *
 * We start off by adding our headers from the avr libraries that we need.
 * We will also need to establish the cpu frequency however this is done at
 * at compile a time using the make file. But for documentation purpose here,
 * F_CPU = 1000000 Hz. Embedded ciruits use thier crystals to tell time in
 * terms of thier "ticks" or more specifically one time period of the crystals
 * natural vibration. In this case we have a "clock" thats runs off of the
 * on-board crystal that vibrates naturally at a specifc frequency when voltage
 * is applied to it. F_CPU or Fosc is just that oscillation frequency. So then
 * if F = 1Mhz, by principles of waves time period T = 1 us.Therefore the
 * the smallest unit of time on the board is 1 micro second. Therefore, we can
 * say a tick is a micro second and 1000 ticks make a milli second, 1000000
 * ticks make 1 second and so on.
 *
 */

// Defines for the scheduler. [0 - highest priority, 255 - lowest priority]
#define TOTALTASKS 2             // Total tasks in the scheduler other than idle
#define TASKRATE_PROPULSION 50   // Task rate for propulsion
#define TASKRATE_UART 100        // Task rate for UART reading
#define IDLE_TASK 255            // Task prioirity for an idle task


/* Task data that will change
unsigned char currenttaskindex = 0;    // Index of the highest priority task
unsigned char runningtasks[3] = {255}; // Array of running task priority

// We know that a Real Time OS has tasks that run at task rates. These tasks
// keep running at thier specified task rate, one at a time (on one core).
// However, certain situations allow certain tasks to preempt other
// ongoing tasks. A real time os that allows such preemption is called a
// pre-emptive RTOS and its task have thier own specified prioirity.
// This RTOS is an example of a pre-emptive RTOS. Below is a structure for that
// task.

typedef struct taskstructure {
  unsigned char running;      // A flag to indicate if the task is running
  unsigned long taskRate;     // Rate of each task
  unsigned long elaspsedTime; // Time elapsed since the last time the task ran
  void (*taskfuntion)(void);   // Actual task function pointer
} task;

// We need two task functions:
// 1. Propulsion task
// 2. LIDAR task
task tasklist[TOTALTASKS];
*/
// NOTE: Hard real time system task have to end within a certain time or they
// can cause system failure which can be catastrpohic. Therefore typically the
// "watchdog" timer is used to keep track of a tasks. If the timer expires, the
// timer value resets and the entire board restarts. Bad things happen typically
// when watchdog resets. On my little schedulare I am using Timer 1 as the
// dedicated watchdog

#endif
