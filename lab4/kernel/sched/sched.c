/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 	<yag1@andrew.cmu.edu>
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

void sched_init(task_t* main_task  __attribute__((unused)))
{

}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void __attribute__((unused)) idle(void)
{
	 enable_interrupts();
	 while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
/*create TCB, put task in runquene, set up TCB for idle task, make idle scheduable*/

void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
//Your kernel should have logic of how to assign priority, initial context, etc. task_t contains the minimum information necessary for your kernel to initialize tcb. Document your design decision as comments.

	size_t i;
	task_t* my_tasks = *tasks;
	sched_context_t *context;

	runqueue_init();

	for (i = 1; i <= num_tasks; i++) {
		context = &(system_tcb[i].context);
		context->r4 = (uint32_t)my_tasks[i-1].lambda;
		context->r5 = (uint32_t)my_tasks[i-1].data;
		context->r6 = (uint32_t)my_tasks[i-1].stack_pos;
		context->r7 = 0;
    	context->r8 = global_data;
    	context->r9 = 0;
    	context->r10 = 0;
    	context->r11 = 0;
		context->sp = system_tcb[i].kstack_high;
		context->lr = launch_task;

		system_tcb[i].native_prio = i;
		system_tcb[i].cur_prio = i;
		system_tcb[i].holds_lock = 0;
		system_tcb[i].sleep_queue = NULL;
	
		runqueue_add(&system_tcb[i], system_tcb[i].cur_prio);

	}

	/*set up TCB for idle task*/
	context = &(system_tcb[IDLE_PRIO].context);
	context->r4 = (uint32_t)idle;
	context->r5 = 0;//don't need argument
	context->r6 = 0xa2500000;//don't need stack
	context->r7 = 0;
	context->r8 = global_data;
	context->r9 = 0;
	context->r10 = 0;
	context->r11 = 0;
	context->sp = system_tcb[IDLE_PRIO].kstack_high;
	context->lr = launch_task;

	system_tcb[IDLE_PRIO].native_prio = IDLE_PRIO;
	system_tcb[IDLE_PRIO].cur_prio = IDLE_PRIO;
	system_tcb[IDLE_PRIO].holds_lock = 0;
	system_tcb[IDLE_PRIO].sleep_queue = NULL;

	runqueue_add(&system_tcb[IDLE_PRIO], system_tcb[IDLE_PRIO].cur_prio);


	//dispatch_nosave();
	
}
