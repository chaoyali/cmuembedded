/** @file sleep.S
 *
 * @brief sleep sycall wrapper
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 	<yag1@andrew.cmu.edu>
 */

#include <asm.h>
#include <./bits/swi.h>

	.file "sleep.S"

FUNC(sleep)

sleep:
	STMFD	sp!, {r1-r12, lr}	@ store user s' gp registers and lr
	SWI		SLEEP_SWI
	LDMFD	sp!, {r1-r12, lr}	@ unstack user's registers
	BX		lr
