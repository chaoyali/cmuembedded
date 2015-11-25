/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Authors: Chaoya Li <chaoyal@andrew.cmu.edu>
 *			Ya Gao	  <yag1@andrew.cmu.edu>
 * Links to libc.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	const char* cursor[4] = {"|", "/", "-", "\\"};
	char back[] = "\r";
	//char* cursor[8] = {"|", "\r", "/", "\r", "-", "\r", "\\", "\r"};
	//const char* cursor[4] = {"|", "/", "-", "\\"};
	while (1) {
		int i = 0;

		for (i = 0; i < 4; i++) {
			write(1, cursor[i], 1);
			write(1, back, 1);
			sleep(200);
		}
	}
	return 0;
}

