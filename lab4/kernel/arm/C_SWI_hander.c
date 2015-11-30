#include <types.h>


#include <arm/reg.h>
#include <bits/swi.h>
#include <syscall.h>

extern unsigned long sys_time;
//dispatch the handler
void C_SWI_handler(unsigned swi_num, unsigned *regs){
	unsigned long msecond;
	switch (swi_num) {
//		case EXIT_SWI: 
//			exit(regs[0]);
//			break;
		
		case READ_SWI: 
			regs[0] = read_syscall(regs[0], (void *)regs[1], regs[2]); 
			break;
		case WRITE_SWI:
			write_syscall(regs[0], (void *)regs[1], regs[2]); 
			break;
        case TIME_SWI:
            regs[0] = sys_time;
            break;
        case SLEEP_SWI:
            msecond = regs[0];
            sleep_syscall(msecond);
            break;
       case CREATE_SWI:
   			task_create((task_t *)regs[0], regs[1]);
            break;
       case MUTEX_CREATE:
   			
            break;
       case MUTEX_LOCK:
   			
            break;
       case MUTEX_UNLOCK:
   			
            break;
       case EVENT_WAIT:
   			regs[0] = event_wait(regs[0]);
            break;

		default:;
	}
}
