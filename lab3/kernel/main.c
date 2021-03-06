#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>


#include "./include/bits/swi.h"
#include "./include/bits/errno.h"

int exit(int num);
void swi_handler();
void irq_handler();
int load_user();
void C_SWI_handler(unsigned swi_num, unsigned *regs);
extern ssize_t write(int fd, const void *buf, size_t count);
extern ssize_t read(int fd, void *buf, size_t count);
extern void init_interrupt(void);
extern void init_timer0(void);
extern void sleep(unsigned long msecond);
void restore_handler(unsigned int *vector, unsigned *old_instr1, unsigned *old_instr2);
void install_handler(unsigned int *vector, int handler, unsigned *old_instr1, unsigned *old_instr2);

uint32_t global_data;
unsigned long sys_time;

int kmain(int argc, char* argv[], uint32_t table) {
	int exit_num = 0;
	unsigned *old_instr1 = 0, *old_instr2 = 0;
	unsigned *old_instr_irq_1 = 0, *old_instr_irq_2 = 0;

	app_startup(); /* bss is valid after this point */
	global_data = table;

	//install the swi custom handler
	install_handler((unsigned int*)0x8, (int)swi_handler, old_instr1, old_instr2);
	//install the irq custom handler
	install_handler((unsigned int*)0x18, (int)irq_handler, old_instr_irq_1, old_instr_irq_2);

	//initiate the interrupt and timer 0
	init_interrupt();
	init_timer0();
	
	//load user program	
	exit_num = load_user(argc, argv);

	//restore the swi system handler
	restore_handler((unsigned int*)0x8, old_instr1, old_instr2);
	//restore the irq system handler
	restore_handler((unsigned int*)0x18, old_instr_irq_1, old_instr_irq_2);

	return exit_num;

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

//restore the system handler
void restore_handler(unsigned int *vector, unsigned *old_instr1, unsigned *old_instr2) {
	//get the instruction in vector
	unsigned instruction = *vector;
	//get the ldr offset	
	unsigned ldr_offset = instruction & 0xfff;
	//the address of handler 
	unsigned *swi_addr =(unsigned int *) (*(unsigned int*) ((unsigned)ldr_offset + (unsigned)vector + 0x8));
	//restore the first instruction
	*swi_addr = (unsigned int)old_instr1;
	swi_addr++;
	//restore the second instruction
	*swi_addr = (unsigned int)old_instr2;
}

//dispatch the handler
void C_SWI_handler(unsigned swi_num, unsigned *regs){
	unsigned long msecond;
	switch (swi_num) {
		case EXIT_SWI: 
			exit(regs[0]);
			break;
		case READ_SWI: 
			regs[0] = read(regs[0], (void *)regs[1], regs[2]); 
			break;
		case WRITE_SWI:
			write(regs[0], (void *)regs[1], regs[2]); 
			break;
        case TIME_SWI:
            regs[0] = sys_time;
            break;
        case SLEEP_SWI:
            msecond = regs[0];
            sleep(msecond);
            break;

		default:;
	}
}

void myprint(){
	printf("Myprint\n");
}

