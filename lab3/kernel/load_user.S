	@author: Chaoya Li <chaoyal@andrew.cmu.edu>
	@		 Ya Gao	<yag1@andrew.cmu.edu>
	@This file is used to set the user mode, stack, branch; irq stack
	@to user program.
	.file	"load_user.S"
	.text

	.global load_user

#include <bits/swi.h>
#include <config.h>
load_user:

	@save all the register in svc mode
	STMFD	sp!, {r0-r12, lr}
	LDR		r2, =ADDR_SP_SAVE
	STR		sp, [r2]

	@ Switch to IRQ mode, set its stack
	MRS   r2, cpsr
	AND   r2, r2, #0xFFFFFFF0
	ORR   r2, r2, #0x2
	MSR   cpsr, r2
	MOV   sp, #IRQ_STACK

	@swich to user mode with FIQs masked.
	MRS	r2, cpsr
	BIC	r2, #0xff
	ORR	r2, #0x50
	MSR	cpsr, r2

	@set user mode stack	
	MOV	sp, #USER_STACK

	@push NULL
	MOV	r3, #0
	STMFD	sp!, {r3}
	
	@argc from uboot, act as a counter
	MOV	r3, r0, LSL #2
L1:
	CMP	r3, #0
	BEQ	L2
	@load argv to stack
	SUB	r3, r3, #4
	LDR	r4, [r1, r3]
	STMFD	sp!, {r4}
	B	L1
L2:
	STMFD	sp!, {r0}

	@jump to 0xa0000000
	MOV	r3, #USER_BEGIN
	BLX	r3

	@exit
	SWI	EXIT_SWI

