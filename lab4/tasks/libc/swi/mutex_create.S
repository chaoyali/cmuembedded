/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @ Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @ Ya Gao 	<yag1@andrew.cmu.edu>
 */

#include <asm.h>
#include <bits/swi.h>

	.file "mutex_create.S"

FUNC(mutex_create)

mutex_create:
	push	{lr}
	swi	MUTEX_CREATE
	cmp	r0, #0	@ check the return value of mutex_create
	bge	.done	@ if the value is non-negative, no error

	@ if there exists error, set errno to the negative of return value
	rsb	r0,	r0,	#0
	LDR	r1,	=errno
	STR	r0,	[r1]
	MOV	r0,	#-1

.done:
	pop	{lr}
	bx	lr

	.global	errno

