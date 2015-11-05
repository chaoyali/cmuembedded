/**
 * @file c_irq_handler
 *
 * @handler the irq
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 	<yag1@andrew.cmu.edu>
**/
#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

#include "./include/bits/swi.h"
#include "./include/bits/errno.h"

//dispatch the handler
void C_IRQ_handler(){
	/* Check that a time interrupt has occured, if not, ignore it */
	/* Check if interrupt controller irq pending reg. bit 26 is set */
	if (reg_read(INT_ICIP_ADDR) & (1 << 26)) {
		/* Check if OSMR0 = OSCR */
		if (reg_read(OSTMR_OSSR_ADDR) & 0x1) {
			/* Reload the match register */
			
		}
	}
	printf("ok");
}
