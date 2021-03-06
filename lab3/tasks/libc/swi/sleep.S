/** @file sleep.S
 *
 * @brief sleep sycall wrapper
 *
 * @author Chaoya Li <>
 *		   Ya Gao <yag1@andrew.cmu.edu>
 * @date 2015-11-01
 */

#include <asm.h>
#include <./bits/swi.h>

	.file "sleep.S"

FUNC(sleep)
	@ Add code here.  Put your names at the top.

sleep:
	STMFD	sp!, {r1-r12, lr}	@ store user s' gp registers and lr
	SWI		SLEEP_SWI
	LDMFD	sp!, {r1-r12, lr}	@ unstack user's registers
	BX		lr

