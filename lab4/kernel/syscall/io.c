/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @Chaoya Li	<chaoyal@andrew.cmu.edu>
 * @Ya Gao 	<yag1@andrew.cmu.edu>
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

size_t sdram_start = 0xa0000000;
size_t sdram_end = 0xa3ffffff;
size_t sfrom_start = 0x00000000;
size_t sfrom_end = 0x00ffffff;

/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), void *buf __attribute__((unused)), size_t count __attribute__((unused)))
{

	size_t byte_count = 0;
	char *charBuf = (char *)buf;
	char c;

	if (fd != STDIN_FILENO)	/* U-Boot API can only read from stdin */
		return -EBADF;

	if ((size_t)charBuf < sdram_start || (size_t)charBuf + count > sdram_end)
		return -EFAULT;

	while (byte_count < count) {
		c = getc();
		if (c == 4)	/* EOT character */
			return byte_count;
		else if (c == 8 || c == 127) {	/* backspace or delete */
			byte_count--;
			puts("\b \b");
		} 
		else if (c == 10 || c == 13)	{/* newline or carriage return */
				charBuf[byte_count] = '\n';
				byte_count++;
				putc('\n');
				return byte_count;
		}
		else {
			charBuf[byte_count] = c;
			byte_count++;
			putc(c);

		}
	}

	return byte_count;
	
}

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd  __attribute__((unused)), const void *buf  __attribute__((unused)), size_t count  __attribute__((unused)))
{

	size_t byte_count = 0;
	char *charBuf = (char *)buf;
	char c;

	if (fd != STDOUT_FILENO)
		return -EBADF;

	if ((size_t)charBuf < sfrom_start || 
		((size_t)charBuf + count > sfrom_end && (size_t)charBuf + count < sdram_start) ||
		((size_t)charBuf > sfrom_end && (size_t)charBuf < sdram_start) ||
		(size_t)charBuf + count > sdram_end)
		return -EFAULT;

	while (byte_count < count) {
		c = charBuf[byte_count];
		if (c == '\0')
			return byte_count;
		putc(c);
		byte_count++;
	}

	return byte_count;
	
}

