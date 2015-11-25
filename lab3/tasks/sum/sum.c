/** @file sumup.c
 *
 * @brief Game of sum up numbers
 *
 * Authors: Chaoya Li <chaoyal@andrew.cmu.edu>
 *			Ya Gao	  <yag1@andrew.cmu.edu>
 * Links to libc.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
static unsigned seed[2];

extern unsigned long time(void);
int rand(void);

#define MAX_LENGTH 100

int main(int argc, char** argv)
{
	//time_t t;
	int i = 0;
	int time_limit;
	char input_time[MAX_LENGTH];
	char err_msg[] = "Read error\n";
	char num[1];
	char back[] = "\r";
	int random_num[5];
	int right_ans = 0;
	char input_ans[MAX_LENGTH];
	int user_ans;
	unsigned long start_time, end_time;

	while (1) {
		right_ans = 0;
		printf("How fast can you sum up 5 numbers from 0 to 49 (in milliseconds) ?\n");
		if (read(0, input_time, MAX_LENGTH) <= 0) {
			write(1, err_msg, strlen(err_msg));
			continue;
		}
		//sscanf(input_time, "%d", &time_limit);
		time_limit = atoi(input_time);

		/* Intializes random number generator */
    	//srand((unsigned)time(&t));
		/* Generate 5 random numbers from 0 to 9 */
		for (i = 0; i < 5; i++)  {
			random_num[i] = rand() % 10;
			right_ans += random_num[i];
		}
		/* Print 5 random numbers */
		for (i = 0; i < 5; i++) {
			sprintf(num, "%d", random_num[i]);
			write(1, num, 1);
			//sleep(1);
			sleep(300);
			write(1, back, 1);
		}
		printf("Pls input answer: \n");

		start_time = time();
		if (read(0, input_ans, MAX_LENGTH) <= 0) {
			write(1, err_msg, strlen(err_msg));
			continue;
		}
		end_time = time();
		//sscanf(input_ans, "%d", &user_ans);
		user_ans = atoi(input_ans);

		if (end_time - start_time > (unsigned long)time_limit)
			printf("U lose: time exceeds limit~ Try again!\n");
		else if (user_ans != right_ans)
			printf("U lose: wrong answer~ Try again!\n");
		else
			printf("U win! Try again!\n");

	}
	return 0;
}

int
rand(void)
{
	seed[0] += 0xa859c317;
	seed[0] += (seed[1] << 13) | (seed[1] >> 19);
	seed[1] += seed[0];
	return seed[0] & 0x7FFFFFFF;
}

