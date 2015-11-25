/**
 * @file read.c
 *
 * @read system call function
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 		<yag1@andrew.cmu.edu>
 */

#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

#include "./include/bits/swi.h"
#include "./include/bits/errno.h"

ssize_t read(int fd, void *buf, size_t count) {
	unsigned count_read = 0;	
	char *read_buf = (char *)buf;
	
	//check the fd
	if(fd != 0) //not STDIN
		return -EBADF;
	//check the range of buf
	if(((unsigned)&buf < (unsigned)0xa0000000)||(((unsigned)&buf + (unsigned)count) > (unsigned)0xa3ffffff))
		return -EFAULT;	
	
	//read
	while (count_read < count) {
		if(tstc()) {
			read_buf[count_read] = getc();
	
			//EOT
			if (read_buf[count_read] == 4) {	//EOT
				return count_read;
			}
			//check \r  \n
			if (read_buf[count_read] == 10 || read_buf[count_read] == 13) {
				read_buf[count_read] = 10;
				putc('\n');
				count_read ++;
				return count_read;

			}
			//check the backspace and delete
			if (read_buf[count_read] == 8 || read_buf[count_read] == 127) {
				count_read --;
				puts("\b \b");
				continue;

			}
			putc(read_buf[count_read]);
			count_read++;
		}	
	}
	//if buffer full, return here.
	return count_read;
}
