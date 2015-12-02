/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author: Chaoya Li <chaoyal@andrew.cmu.edu>
 * 		    Ya Gao <yag1@andrew.cmu.edu>
 *
 * 
 * @date 2015-11-27
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];

/* Initialize mutexes */
void mutex_init()
{
	int i = 0;
	for (i = 0; i < OS_NUM_MUTEX; i++) {
		gtMutex[i].bAvailable = TRUE;
		gtMutex[i].pHolding_Tcb = NULL;
		gtMutex[i].bLock = FALSE;
		gtMutex[i].pSleep_queue = NULL;
	}
}

int mutex_create(void)
{
	disable_interrupts();
	int i = 0;
	mutex_t *cur_mutex;

	for (i = 0; i < OS_NUM_MUTEX; i++) {
		cur_mutex = &(gtMutex[i]);
		if (cur_mutex -> bAvailable == TRUE) {
			cur_mutex -> bAvailable = FALSE;
			enable_interrupts();
			return i;
		}
	}

	enable_interrupts();
	/* Set errno and return val. indicating error in wrapper */
	return -ENOMEM;
}

int mutex_lock(int mutex  __attribute__((unused)))
{
	mutex_t *cur_mutex;
	bool_e isLocked;
	tcb_t *cur_tcb;

	disable_interrupts();
	/* Invalid mutex index */
	if (mutex < 0 || mutex >= OS_NUM_MUTEX) {
		enable_interrupts();
		return EINVAL;
	}

	cur_mutex = &(gtMutex[mutex]);
	isLocked = cur_mutex -> bLock;
	cur_tcb = get_cur_tcb();
	/* Available mutex or self holding mutex */
	if (cur_mutex -> bAvailable == TRUE) {
		enable_interrupts();
		return EINVAL;
	}
	if (isLocked && cur_mutex -> pHolding_Tcb == cur_tcb) {
		enable_interrupts();
		return EDEADLOCK;
	}

	if (!isLocked) {	/* Free mutex to lock */
		cur_mutex -> pHolding_Tcb = cur_tcb;
		cur_mutex -> bLock = TRUE;
		(cur_tcb -> holds_lock)++;
		/* ??? Need to change the priority ? */
	} else {	/* Add to the sleep queue */
		tcb_t *sleep_tcb = cur_mutex -> pSleep_queue;
		if (sleep_tcb == NULL) {
			cur_mutex -> pSleep_queue = cur_tcb;
			cur_tcb -> sleep_queue = NULL;
		} else {
			while (sleep_tcb -> sleep_queue != NULL)
				sleep_tcb = sleep_tcb -> sleep_queue;
			sleep_tcb -> sleep_queue = cur_tcb;
			cur_tcb -> sleep_queue = NULL;
		}
		/* Context switch */
		dispatch_sleep();
		/* Obtain the lock */
		cur_mutex -> pHolding_Tcb = cur_tcb;
		cur_mutex -> bLock = TRUE;
		(cur_tcb -> holds_lock)++;
	}
	enable_interrupts();
	return 0; // fix this to return the correct value
}

int mutex_unlock(int mutex  __attribute__((unused)))
{
	mutex_t *cur_mutex;
	tcb_t *cur_tcb;

	disable_interrupts();
	/* Invalid mutex index */
	if (mutex < 0 || mutex >= OS_NUM_MUTEX) {
		enable_interrupts();
		return EINVAL;
	}
	cur_mutex = &(gtMutex[mutex]);
	cur_tcb = get_cur_tcb();
	/* Available mutex or unlocked mutex */
	if (cur_mutex -> bAvailable == TRUE || cur_mutex -> bLock == FALSE) {
		enable_interrupts();
		return EINVAL;
	}
	if (cur_mutex -> pHolding_Tcb != cur_tcb) {
		enable_interrupts();
		return EPERM;
	}

	/* Change the state of current mutex */
	cur_mutex -> pHolding_Tcb = NULL;
	cur_mutex -> bLock = FALSE;
	tcb_t *new_tcb = cur_mutex -> pSleep_queue;
	if (new_tcb != NULL) {
		/* Wake up the first task in sleep queue */
		runqueue_add(new_tcb, new_tcb -> cur_prio);
		cur_mutex -> pSleep_queue = new_tcb -> sleep_queue;
		new_tcb -> sleep_queue = NULL;
	}
	(new_tcb -> holds_lock)--;
	enable_interrupts();
	return 0; // fix this to return the correct value
}


