/**
 * @file device.c
 * @author: Chaoya Li <>
 * 		    Ya Gao <yag1@andrew.cmu.edu>
 *
 * @brief Implements simulated devices.
 *
 * @date 2015-12-27
 */

#include <types.h>
#include <assert.h>

#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

#define NULL ((void*)0)

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
	int i = 0;

	for (i = 0; i < NUM_DEVICES; i++) {
		/* initialize the sleep queue */
		devices[i].sleep_queue = NULL;	// ??? how to initialize?
		/* initialize the match value */
		devices[i].next_match = dev_freq[i];
	} 
}

/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned int dev __attribute__((unused)))
{
	/* Disable interrupts */
	disable_interrupts();
	/* Put the tcb of the task (caller) into the queue */
	tcb_t *caller_tcb = get_cur_tcb();	
	tcb_t *cur_queue = devices[dev].sleep_queue;
	if (cur_queue == NULL) {
		devices[dev].sleep_queue = caller_tcb;
	} else {
		while (cur_queue -> sleep_queue != NULL) 
			cur_queue = cur_queue -> sleep_queue;	/* find the end of cur_queue */
		cur_queue -> sleep_queue = caller_tcb;
	}
	caller_tcb -> sleep_queue = NULL;	/* end of the queue */
	/* Enable interrupts */
	enable_interrupts();
	/* Context switch */
	dispatch_sleep();
	// //update match time ??? do we need to change here
	// device[dev].match += dev_freq[dev];
	// //remove the highest prio task ??? do we need to remove here
	// runqueue_remove(highest_prio());
}


/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis __attribute__((unused)))
{	
	disable_interrupts();
	int i = 0;
	tcb_t *tmp_queue;

	/* Check whether the next event for every device has occured */
	for (i = 0; i < NUM_DEVICES; i++) {
		if (devices[i].next_match <= millis) {
			devices[i].next_match = dev_freq[i] + millis;
			/* Make all the tasks on this device's sleep_queue ready to run */
			tcb_t *cur_queue = devices[i].sleep_queue;
			while (cur_queue != NULL) {
				/* !!! runqueue_add() has not yet implemented */
				runqueue_add(cur_queue, cur_queue -> cur_prio);
				tmp_queue = cur_queue -> sleep_queue;
				cur_queue -> sleep_queue = NULL;
				cur_queue = tmp_queue; 
			}
			devices[i].sleep_queue = NULL;
			/* Context switch */
			dispatch_save();
		}
	}
	enable_interrupts();
}
