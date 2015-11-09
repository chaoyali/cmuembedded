/**
 * sleep
 *
 * Authors: Chaoya Li <chaoyal@andrew.cmu.edu>
 *			Ya Gao	<yag1@andrew.cmu.edu>
 * Date:    12:18, 11/01/2013
 */
#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>


#include "./include/bits/swi.h"
#include "./include/bits/errno.h"
extern volatile unsigned long sys_time;

void sleep(unsigned long msceond) {
    unsigned long target_time = sys_time + msceond;
    while(sys_time < target_time) {
		printf("%ld  %ld\n", sys_time, target_time);
	}
}
