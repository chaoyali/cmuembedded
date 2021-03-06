/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:53:16 -0400
 * 
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */

#include <asm.h>
#include <bits/swi.h>
	.file "read.S"

FUNC(read)

read:
	push	{lr}

	swi	READ_SWI
	cmp	r0, #0		@check the return num of read
	bge	.done

	@if error happen
	mvn	r1, r0
	add	r1, r1, #1
	ldr	r2, =errno	@stroe the address of errno
	str	r1, [r2]
	mov	r0, #-1
	
.done:
	pop	{lr}
	bx	lr	

	.global errno
