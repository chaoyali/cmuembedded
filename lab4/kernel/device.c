/**
 * @file device.c
 * @author:  @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 	<yag1@andrew.cmu.edu>
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
#include <bits/errno.h>
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

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000, 4100, 4200, 4300, 4400, 4500, 4600, 4700, 4800, 4900, 5000, 5100, 5200, 5300, 5400, 5500, 5600, 5700, 5800, 5900, 9000};

static dev_t devices[NUM_DEVICES];
/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
	int i = 0;

	for (i = 0; i < NUM_DEVICES; i++) {
		/* initialize the sleep queue */
		devices[i].sleep_queue = NULL;
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

	/* Context switch */
	dispatch_sleep();

	/* Enable interrupts */
	enable_interrupts();
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

	int i = 0;
	tcb_t *tmp_queue;
	int isNotEmpty = 0;

	disable_interrupts();

	/* Check whether the next event for every device has occured */
	for (i = 0; i < NUM_DEVICES; i++) {
		if (devices[i].next_match <= millis) {

			devices[i].next_match += dev_freq[i];
			/* Make all the tasks on this device's sleep_queue ready to run */
			tcb_t *cur_queue = devices[i].sleep_queue;
			while (cur_queue != NULL) {
				isNotEmpty = 1;
				runqueue_add(cur_queue, cur_queue -> cur_prio);
				tmp_queue = cur_queue -> sleep_queue;
				cur_queue -> sleep_queue = NULL;
				cur_queue = tmp_queue; 
			}
			devices[i].sleep_queue = NULL;
		}
	}

	/* Context switch when the sleep_queue is not empty*/
	if (isNotEmpty) {

		dispatch_save();
	}
	enable_interrupts();

}
