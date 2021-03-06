/** @file config.h
 *
 * @brief A set of common OS configuration flags.  This is to control overall
 * OS behavior and not behavior of a particular modules.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-28
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define OS_TICKS_PER_SEC        100    /* Set the number of ticks in one second */
#define ADDR_SP_SAVE 			0xa2000004 /*address that save sp*/
#define IRQ_STACK 				0xa2000000 /*irq stack top*/
#define USER_STACK				0Xa3000000 /*user stack top*/
#define USER_BEGIN				0Xa0000000 /*user entry*/
#endif /* _CONFIG_H_ */
