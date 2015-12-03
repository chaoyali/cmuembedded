/** @file ctx_switch.c
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 	<yag1@andrew.cmu.edu> 
 *
 * @brief C wrappers around assembly context switch routines.
 *
 * @date 2015-12-27
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static __attribute__((unused)) tcb_t* cur_tcb; /* use this if needed */

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
	/* ??? Not quite understand */
	/* Set the current task to idle */
	//cur_tcb = idle;
	/* ??? Need to remove the prio of idle from runqueue? */
	//ctx_switch_half(&(cur_tcb -> context));
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
	tcb_t *new_task;
	tcb_t *old_task = cur_tcb;
	uint8_t prio = highest_prio();
//	printf("save: %d\n", prio);
	/* If the current task is not the highest priority task */
	if (old_task -> cur_prio > prio) {

		new_task = runqueue_remove(prio);
		sched_context_t *old_ctx = &(old_task -> context);
		runqueue_add(old_task, old_task -> cur_prio);
		cur_tcb = new_task;
		/* !!! ctx_switch_full has not yet implemented */
		ctx_switch_full(&(new_task -> context), old_ctx);
	}
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
	/* Get the highest priority */
	uint8_t prio = highest_prio();
	/* Get the corresponding task */
	tcb_t *tcb = runqueue_remove(prio);
	/* Call ctx_switch_half */
	cur_tcb = tcb;
	ctx_switch_half(&(cur_tcb -> context));
}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	/* Similar implementation as dispatch_save but let the current task sleep, and don't 
	 * add it into the runqueue
	 */
	uint8_t prio = highest_prio();

	tcb_t *task = runqueue_remove(prio);
	sched_context_t *cur_ctx = &(cur_tcb -> context);
	cur_tcb = task;
	ctx_switch_full(&(task -> context), cur_ctx);

}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return cur_tcb -> cur_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return cur_tcb; 
}
