/** @file main.c
 *
 * @brief kernel main
 *
 * @author 
 *	   
 *	   
 * @date   
 */

#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>


extern void init_interrupt(void);
extern void init_timer0(void);
void install_handler(unsigned int *vector, int handler, unsigned *old_instr1, unsigned *old_instr2);
int load_user();
void swi_handler();
void irq_wrapper();

uint32_t global_data;
unsigned long sys_time;
void myprintf() {
	printf("myprint");
}
int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{

	app_startup();
	global_data = table;

	/* add your code up to assert statement */

	unsigned *old_instr1 = 0, *old_instr2 = 0;
	unsigned *old_instr_irq_1 = 0, *old_instr_irq_2 = 0;

	app_startup(); /* bss is valid after this point */
	global_data = table;

	//install the swi custom handler
	install_handler((unsigned int*)0x8, (int)swi_handler, old_instr1, old_instr2);
	//install the irq custom handler
	install_handler((unsigned int*)0x18, (int)irq_wrapper, old_instr_irq_1, old_instr_irq_2);

	//initiate the interrupt and timer 0
	init_interrupt();
	init_timer0();
	
	//load user program	
	load_user(argc, argv);
	myprintf();
		
	assert(0);        /* should never get here */
}

//install the handler
void install_handler(unsigned int *vector, int handler, unsigned *old_instr1, unsigned *old_instr2) {
	//get the instruction in vector
	unsigned instruction = *vector;
	//get the ldr offset	
	unsigned ldr_offset = instruction & 0xfff;
	
	//the address of handler 
	unsigned *swi_addr =(unsigned int *) (*(unsigned int*) ((unsigned)ldr_offset + (unsigned)vector + 0x8));
	
	old_instr1 = (unsigned *)*swi_addr;
	*swi_addr = 0xe51ff004;
	swi_addr++;
	old_instr2 = (unsigned *)*swi_addr;
	*swi_addr = handler;
}


