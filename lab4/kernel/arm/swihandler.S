	@author: Chaoya Li <chaoyal@andrew.cmu.edu>
	@This is a swi handler. I can parse the swi number
	@and call the C_swi_handler to dispatch the handler
#include <arm/psr.h>
	.file	"swihandler.S"
	.text

	.global swi_handler

swi_handler:

	STMFD	sp!, {r0-r12, lr}
	MOV		r1, sp

    @ recover r8
	LDR   	r2, =global_data
	LDR   	r8, [r2]

	@ enable IRQ
	MRS   	r2, cpsr
	AND   	r2, r2, #0xFFFFFF7F
	MSR   	cpsr, r2

	@ determine the SWI number called
	LDR		r0, [lr, #-4]
	BIC		r0, r0, #0xff000000

    @ store user's sp & lr, or you will get "deadbeef"
    STMFD sp, {sp, lr}^
    SUB   sp, sp, #8

	@ store spsr, to the top of the stack (14*4)
	MRS   r2, spsr
	STR   r2, [sp, #56]

	@go c handler to dispatch appropriate syscall
	BL		C_SWI_handler

	@ disable interrupt
	MRS r2, cpsr
	ORR r2, #PSR_IRQ
	MSR cpsr, r2

	@ restore spsr
 	LDR   r2, [sp, #56]
	MSR   spsr, r2

	@ restore sp & lr
	LDMFD sp, {sp, lr}^
	add   sp, sp, #8

	@retore non-banked user space registers
	LDMFD	sp!, {r0-r12, lr}
	
	@ return
	MOVS	pc, lr		

