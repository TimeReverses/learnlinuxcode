#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
/*
 struct itimerval {
               struct timeval it_interval; * Interval for periodic timer 
               struct timeval it_value;    * Time until next expiration 
           };

           struct timeval {
               time_t      tv_sec;         * seconds 
               suseconds_t tv_usec;        * microseconds 
           };
*/
void myfunc(int sig)
{
	printf("hello world\n");
}

int main(void)
{
	struct itimerval it,oldit;
	signal(SIGALRM,myfunc);
	it.it_value.tv_sec = 5;
	it.it_value.tv_usec = 0;
	it.it_interval.tv_sec = 3;
	it.it_interval.tv_usec = 0;
	if(setitimer(ITIMER_REAL,&it,&oldit) == -1)
	{
		perror("setitimer error");
		exit(1);
	}
	while(1){}
	return 0;
}
