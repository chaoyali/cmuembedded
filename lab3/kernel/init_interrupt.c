/**
 * @file init_interrupt
 *
 * @initiate the interrupt by writing to ICMR, ICLR.
 *
 * @author Chaoya Li <chaoyal@andrew.cmu.edu>
 * @author Ya Gao    <yag1@andrew.cmu.edu>
 */

#include <exports.h>
#include <arm/reg.h>
#include <arm/interrupt.h>
void init_interrupt(void)
{
	//set ICLR to 0, which mean all the interrupt will generat IRQ
	reg_write(INT_ICLR_ADDR, 0);
	//cleean ICMR, then only enable the OSTimer 0 interrupt
	reg_clear(INT_ICMR_ADDR, 0);
	reg_write(INT_ICMR_ADDR, 1<<INT_OSTMR_0);
}
