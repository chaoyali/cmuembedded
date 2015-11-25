/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
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

//#include <sched.h>
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
tcb_t tcb[3];
void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
//Your kernel should have logic of how to assign priority, initial context, etc. task_t contains the minimum information necessary for your kernel to initialize tcb. Document your design decision as comments.

	

	runqueue_init();

	tcb[0].native_prio = 2;
	tcb[0].cur_prio = 2;
	//tcb[0].context = (sched_context_t)0;
	tcb[0].holds_lock = 0;
	//tcb[0].sleep_queue = null;

//	run_list[tcb[0].cur_prio] = &tcb[0];
	runqueue_add(&tcb[0], tcb[0].cur_prio);
	//runqueue_add(tcb_t* tcb  __attribute__((unused)), uint8_t prio  __attribute__((unused)))

	tcb[1].native_prio = 3;
	tcb[1].cur_prio = 3;
	//tcb[0].context = (sched_context_t)0;
	tcb[1].holds_lock = 0;
	//tcb[0].sleep_queue = null;
	runqueue_add(&tcb[1], tcb[1].cur_prio);
	//run_list[tcb[1].cur_prio] = &tcb[1];

	/*set up TCB for idle task*/
	tcb[2].native_prio = OS_MAX_TASKS-1;
	tcb[2].cur_prio = OS_MAX_TASKS-1;
	//tcb[0].context = (sched_context_t)0;
	tcb[2].holds_lock = 0;
	//tcb[0].sleep_queue = null;

	/*make the idle task schedulable*/
	runqueue_add(&tcb[2], tcb[2].cur_prio);

	while(1);
	
}
