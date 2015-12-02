/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 	<yag1@andrew.cmu.edu>
 */

#include <asm.h>
#include <./bits/swi.h>

	.file "time.S"

FUNC(time)

time:
@	STMFD	sp!, {r1-r12, lr}	@ store user s' gp registers and lr
	SWI		TIME_SWI
@	LDMFD	sp!, {r1-r12, lr}	@ unstack user's registers
	BX		lr
