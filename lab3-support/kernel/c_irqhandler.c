/**
 * @file c_irq_handler
 *
 * @handler the irq
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 		<yag1@andrew.cmu.edu>
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

	printf("ok");
}
