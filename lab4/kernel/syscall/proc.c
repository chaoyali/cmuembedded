/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 	<yag1@andrew.cmu.edu>
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>
#include <lock.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

void sort_tasks_by_prio(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	task_t temp;
	size_t i = 0;
	size_t j = 0;
	for (i = 0; i < num_tasks; i++) {
		for (j = i+1; j < num_tasks; j++) {
			if (tasks[i].T > tasks[j].T) {
				temp = tasks[i];
				tasks[i] = tasks[j];
				tasks[j] = temp;
			}
		}	
	}
}

int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{

	if (num_tasks > OS_MAX_TASKS){
		return -EINVAL;
	}
	mutex_init();
    dev_init();

	sort_tasks_by_prio(tasks, num_tasks);

	allocate_tasks(&tasks, num_tasks);
	//sched_init();
	dispatch_nosave();
	return 0;
}

int event_wait(unsigned int dev  __attribute__((unused)))
{
	dev_wait(dev);
	return 1; /* remove this line after adding your code */	
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
