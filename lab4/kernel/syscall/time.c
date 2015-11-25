/** @file time.c
 *
 * return system time: sys_time
 *
 * Authors: Chaoya Li <chaoyal@andrew.cmu.edu>
 *			Ya Gao	  <yag1@andrew.cmu.edu>
 * Date:    11/8/2015
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <syscall.h>

extern volatile unsigned long sys_time;

unsigned long time_syscall(void)
{
    return sys_time;
}



/** @brief Waits in a tight loop for atleast the given number of milliseconds.
 *
 * @param millis  The number of milliseconds to sleep.
 *
 * 
 */
void sleep_syscall(unsigned long millis  __attribute__((unused)))
{
	unsigned long target_time = sys_time + millis;
    while(sys_time < target_time) {

	}
}
