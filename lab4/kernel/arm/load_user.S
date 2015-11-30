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

	@swich to user mode with FIQs masked.
	MRS		r2, cpsr
	BIC		r2, #0xff
	ORR		r2, #0x50
	MSR		cpsr, r2

	@set stack
	MOV   sp, #USR_STACK

    @restore register r8
    LDR  	r2, =global_data
    LDR  	r8, [r2]

	@push NULL
	MOV		r3, #0
	STMFD	sp!, {r3}
	
	@argc from uboot, act as a counter
	MOV		r3, r0, LSL #2
L1:
	CMP		r3, #0
	BEQ		L2
	@load argv to stack
	SUB		r3, r3, #4
	LDR		r4, [r1, r3]
	STMFD	sp!, {r4}
	B		L1
L2:
	STMFD	sp!, {r0}

	@jump to 0xa0000000
	MOV		r3, #LOAD_ADDR
	BLX		r3

