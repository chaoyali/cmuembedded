/**
 * return system time: sys_time
 *
 * Authors: Chaoya Li <chaoyal@andrew.cmu.edu>
 *			Ya Gao	  <yag1@andrew.cmu.edu>
 * Date:    11/8/2015
 */

extern unsigned long sys_time;

unsigned long time(void) {
    return sys_time;
}

