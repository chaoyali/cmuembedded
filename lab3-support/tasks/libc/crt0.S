/** @file crt0.S
 *
 * @brief Execution startup routine
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:51:29 -0400
 */

#include <asm.h>
#include <bits/swi.h>
	.file "crt0.S"

FUNC(_start)
	ldr	r0, [sp]	@argc
	mov	r1, sp		@address of sp 
	add	r1, r1, #4	@*argv
	mov	lr, pc		@return address
	b	main		@call main
	swi	EXIT_SWI	@exit with the return value of main
