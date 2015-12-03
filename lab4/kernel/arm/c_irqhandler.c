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
#include <arm/reg.h>
#include <device.h>

extern unsigned long sys_time;
//dispatch the handler
void C_IRQ_handler(){
	/* Check that a time interrupt has occured, if not, ignore it */
	/* Check if interrupt controller irq pending reg. bit 26 is set */
    if((reg_read(OSTMR_OSSR_ADDR) & OSTMR_OSSR_M0) && (reg_read(INT_ICPR_ADDR) >> INT_OSTMR_0)) 
			// Increment time by 10ms
			sys_time += 10;

			//clear the OSCR
			reg_write(OSTMR_OSCR_ADDR, 0);

      		// Writing a 1 to the bit acknowledges the match and hence, clears that bit
			reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
			
			dev_update(sys_time);

	}


}
