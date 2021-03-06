/**
 * @file init_timer0
 *
 * @initiate theinit_timer0 by writing to OIER, OSMR0.
 *
 * @author Chaoya Li <chaoyal@andrew.cmu.edu>
 * @author Ya Gao    <yag1@andrew.cmu.edu>
 */

#include <exports.h>
#include <arm/reg.h>
#include <arm/timer.h>
#define COUNT_NUM 32500
void init_timer0(void)
{
	//set OSRM0 to 325000 (10ms)
	reg_write(OSTMR_OSMR_ADDR(0), COUNT_NUM);
//    reg_write(OSTMR_OSMR_ADDR(0), reg_read(OSTMR_OSCR_ADDR) + 32500);

	//enable interrupt for OSRM0
    reg_clear(OSTMR_OIER_ADDR, OSTMR_OIER_E1 | OSTMR_OIER_E2 | OSTMR_OIER_E3);
    reg_set(OSTMR_OIER_ADDR, OSTMR_OIER_E0);

	//clear OSCR
	reg_write(OSTMR_OSCR_ADDR, 0);
}
