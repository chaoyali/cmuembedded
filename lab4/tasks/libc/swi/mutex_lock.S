/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @ Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @ Ya Gao 	<yag1@andrew.cmu.edu>
 */

#include <asm.h>
#include <bits/swi.h>

	.file "mutex_lock.S"

FUNC(mutex_lock)

mutex_lock:
	push	{lr}
	swi	MUTEX_LOCK
	cmp	r0, #0	@ check the return value of mutex_lock
	beq	.done	@ if the value is zero, no error

	@ if there exists error, set errno to the return value
	LDR	r1, =errno
	STR	r0,	[r1]
	MOV	r0,	#-1

.done:
	pop	{lr}
	bx	lr

	.global	errno
