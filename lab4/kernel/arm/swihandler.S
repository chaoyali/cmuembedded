	@author: Chaoya Li <chaoyal@andrew.cmu.edu>
	@This is a swi handler. I can parse the swi number
	@and call the C_swi_handler to dispatch the handler
	.file	"swihandler.S"
	.text

	.global swi_handler

swi_handler:

	STMFD	sp!, {r0-r12, lr}
	MOV		r1, sp

    @ Recover r8 register
	LDR   	r2, =global_data
	LDR   	r8, [r2]

	@ Enable IRQ
	MRS   	r2, cpsr
	AND   	r2, r2, #0xFFFFFF7F
	MSR   	cpsr, r2

	@determine the SWI number called
	LDR		r0, [lr, #-4]
	BIC		r0, r0, #0xff000000

        @ Store stack and link register
        STMFD sp, {sp, lr}^
        ADD   sp, sp, #-8

        @ Store spsr to a specific position????????????????????
        MRS   r2, spsr
        STR   r2, [sp, #56]

	@go c handler to dispatch appropriate syscall
	BL		C_SWI_handler

        @ Restore spsr
        LDR   r2, [sp, #56]
        MSR   spsr, r2

        @ Restore stack and link register????????????
        LDMFD sp, {sp, lr}^
        add   sp, sp, #8

	@retore non-banked user space registers
	LDMFD	sp!, {r0-r12, lr}
	
	@ return
	MOVS	pc, lr		

