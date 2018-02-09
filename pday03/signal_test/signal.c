#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void catchsigint(int signo)
{
	printf("hello world\n");
}

typedef void(*sighandler_t)(int);

int main(void)
{
	sighandler_t handler;
	handler = signal(SIGINT,catchsigint);
	if(handler == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
	while(1)
	{}
	return 0;
}
