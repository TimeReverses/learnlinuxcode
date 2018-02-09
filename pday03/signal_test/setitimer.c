/*
struct itimerval {
               struct timeval it_interval; // Interval for periodic timer 
               struct timeval it_value;    // Time until next expiration 
           };

struct timeval {
               time_t      tv_sec;         // seconds 
               suseconds_t tv_usec;        // microseconds 
           };

*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h> 

/*
struct itimerval{
	struct timeval{
		time_t tv_sec;
		suseconds_t tv_usec;
		}it_interval;
	struct timerval{
		time_t tv_sec;
		suseconds_t tv_usec;
		}it_value;
	}it,oldit;
*/


unsigned int my_alarm(unsigned int sec)
{
	struct itimerval it,oldit;
	//设置
	it.it_value.tv_sec = sec;
	it.it_value.tv_usec = 0;
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = 0;
	//设置定时
	int ret = setitimer(ITIMER_REAL,&it,&oldit);
	if(ret == -1)
	{
		perror("setitimer");
		exit(1);
	}
	return oldit.it_value.tv_sec;	
}

int main(void)
{
	int i = 0;
	my_alarm(1);
	while(1)
		printf("%d\n",i++);
	return 0;
}
