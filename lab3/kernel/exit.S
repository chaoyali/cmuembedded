	@Chaoya Li  <chaoyal@andrew.cmu.edu>
	@Ya Gao		<yag1@andrew.cmu.edu>
	@exit.S

#include "./include/config.h"
	.file	"exit.S"
	.text

	.global exit
exit:
	LDR	r1, =ADDR_SP_SAVE
	LDR	sp, [r1]
	LDMFD	sp!, {r0-r12, lr}
	MOV	pc, lr
