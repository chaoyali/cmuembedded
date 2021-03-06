/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Chaoya Li <>
 *	   Ya Gao <yag1@andrew.cmu.edu>			
 * @date 2015-11-01
 */

#include <asm.h>
#include <./bits/swi.h>

	.file "time.S"

FUNC(time)
	@ Add code here.  Put your name at the top.

time:
	STMFD	sp!, {r1-r12, lr}	@ store user s' gp registers and lr
	SWI		TIME_SWI
	LDMFD	sp!, {r1-r12, lr}	@ unstack user's registers
	BX		lr
