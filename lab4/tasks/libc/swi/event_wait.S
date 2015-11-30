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

    @ Call event_wait
    SWI		EVENT_WAIT

    @ Find out the return is negative or not.
    CMP   r0, #0

    @ For positive return value, just return original value.
    BGE   .L1

    @ For negative return value, compute absolute value.
    RSB   r0, r0, #0

    @ Set errno value to the global errno varible.
    LDR   r1, =errno
    STR   r0, [r1]

    @ Set return value to -1.
    MOV   r0, #-1
.L1:
    MOV   pc, lr


