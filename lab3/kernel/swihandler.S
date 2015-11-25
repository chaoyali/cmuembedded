	@author: Chaoya Li <chaoyal@andrew.cmu.edu>
	@This is a swi handler. I can parse the swi number
	@and call the C_swi_handler to dispatch the handler
	.file	"swihandler.S"
	.text

	.global swi_handler

swi_handler:



	STMFD	sp!, {r0-r12, lr}
	MOV	r1, sp

        @ Mask IRQ
        MRS   r2, cpsr
        AND   r2, r2, #0xFFFFFF7F
        MSR   cpsr, r2

	@determine the SWI number called
	LDR	r0, [lr, #-4]
	BIC	r0, r0, #0xff000000

	@go c handler to dispatch appropriate syscall
	BL	C_SWI_handler

	@retore non-banked user space registers
	LDMFD	sp!, {r0-r12, lr}
	
	MOVS	pc, lr		@It can copy srsr to cpsr

