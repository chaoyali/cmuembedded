/** @file hello.c
 *
 * @brief Prints out Hello world using the syscall interface.
 *
 * Links to libc.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-10-29
 */
#include <unistd.h>

int main(int argc, char** argv)
{
	char hello[] = "Hello World\r\n";
	read(STDIN_FILENO, hello, 16);
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	return 0;
}
