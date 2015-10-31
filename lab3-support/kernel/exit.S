	@Chaoya Li  <chaoyal@andrew.cmu.edu>
	@exit.S

	.file	"exit.S"
	.text

	.global exit
exit:
	MOV	r1, #0xa2000000
	LDR	sp, [r1]
	LDMFD	sp!, {r0-r12, lr}
	MOV	pc, lr
