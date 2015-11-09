@ author: Chaoya Li <chaoyal@andrew.cmu.edu>
@	  Ya Gao <yag1@andrew.cmu.edu>
@ reference Lec7, ppt 28

	.file	"irqhandler.S"
	.text
	
	.global irq_handler

irq_handler:

	STMFD	sp!, {r0-r12, lr}	@ save context
	MOV		r1, sp	@ r1 now contains pointer to parameters on stack

	@ need to save r8 register?
        @ Recover r8 register

    @ Restore register r8
	LDR  r2, =global_data
	LDR  r8, [r2]


	BL	C_IRQ_handler	@ go to handlers


	LDMFD	sp!, {r0-r12, lr}	@retore non-banked user space registers
	
	SUBS	pc, lr, #4
@	MOVS	pc, lr

