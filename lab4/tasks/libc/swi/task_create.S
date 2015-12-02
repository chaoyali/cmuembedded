/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 	<yag1@andrew.cmu.edu>
 */

#include <asm.h>
#include <./bits/swi.h>

	.file "task_create.S"

FUNC(task_create)

	SWI		CREATE_SWI

	cmp	r0, #0		@check the return num of read
	bge	.done

	@if error happen
	mvn	r1, r0
	add	r1, r1, #1
	ldr	r2, =errno	@stroe the address of errno
	str	r1, [r2]
	mov	r0, #-1
.done:
	MOV		pc, lr

	.global errno

