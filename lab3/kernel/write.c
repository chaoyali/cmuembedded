/**
 * @file write.c
 *
 * @write system call function
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 		<yag1@andrew.cmu.edu>
**/
#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

#include "./include/bits/swi.h"
#include "./include/bits/errno.h"

ssize_t write(int fd, const void *buf, size_t count) {

	unsigned i;

	//check the fd	
	if (fd != 1)
		return -EBADF;

	//check whether the buf is in ROM/SDRAM
	if (((unsigned)&buf < (unsigned)0xa0000000) || (((unsigned)&buf + count) > (unsigned)0xa3ffffff) ||
	     (((unsigned)&buf + count) < (unsigned)0x00ffffff))
		return -EFAULT;
	
	//write
	for(i = 0; i < count; i++) {
		if (((char*)buf)[i] == '\0'){
			count++;
			break;
		}
		putc(((char*)buf)[i]);
	}
	return count;
}
