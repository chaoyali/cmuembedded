/** @file event_wait.S
 *
 * @brief time sycall wrapper
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 	<yag1@andrew.cmu.edu>
 */

#include <asm.h>
#include <bits/swi.h>
	.file "event_wait.S"

FUNC(event_wait)

    SWI		EVENT_WAIT

	@check te return number
    CMP		r0, #0

    @ if return nunber > 0, return immediately
    BGE		.L1

    @ if return value < 0, get the absolute value.
    RSB		r0, r0, #0

    @ set errno
    LDR		r1, =errno
    STR		r0, [r1]

    @ set return value to -1
    MOV		r0, #-1

.L1:
    MOV   pc, lr


