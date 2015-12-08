/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 	<yag1@andrew.cmu.edu>
 */

//#define DEBUG 0

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif

const int big_U[64] = {
1000,828,780,757,743,735,729,724,721,718,715,714,712,711,709,708,707,707,
706,705,705,704,704,703,703,702,702,702,701,701,701,701,700,700,700,700,700,
700,699,699,699,699,699,699,699,698,698,698,698,698,698,698,698,698,698,697,
697,697,697,697,697,697,697,697};

void sort(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)));
 
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	size_t i = 0;
	double utilization = 0, uti_block = 0;
	task_t* my_tasks = *tasks;

	sort(my_tasks, num_tasks);

	for (i = 0; i < num_tasks; i++) {
		utilization += my_tasks[i].C/my_tasks[i].T;
		uti_block = utilization + my_tasks[i].B/my_tasks[i].T;
		if ((int)uti_block*1000 > big_U[i])
			return 0;
	}
	return 1;
}
	
/*sort the task by priority.*/
void sort(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
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

