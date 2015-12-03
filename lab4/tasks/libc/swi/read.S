/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 	<yag1@andrew.cmu.edu>
 */

#include <asm.h>
#include <bits/swi.h>

	.file "read.S"

FUNC(read)

	swi	READ_SWI
	cmp	r0, #0	@ check return value
	bge	.L0	

	rsb	r0, r0, #0	@ get the negative of return value
	ldr	r1, =errno	@ load the address of errno
	str	r0, [r1]
	mov	r0, #-1	@ return -1 on error

.L0:
	mov pc, lr
