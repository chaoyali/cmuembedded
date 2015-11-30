/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 	<yag1@andrew.cmu.edu>
 */

#include <asm.h>
#include <./bits/swi.h>

	.file "task_create.S"

FUNC(task_create)
task_create:
	SWI CREATE_SWI

