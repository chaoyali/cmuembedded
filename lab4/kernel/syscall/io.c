/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>

#define EOT_CHAR 0x04
#define DEL_CHAR 0x7f


/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), void *buf __attribute__((unused)), size_t count __attribute__((unused)))
{

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

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd  __attribute__((unused)), const void *buf  __attribute__((unused)), size_t count  __attribute__((unused)))
{

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

