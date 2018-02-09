#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <signal.h>
#include <unistd.h>

void catch_sigalrm(int signum)
{
	;
}

unsigned int mysleep(unsigned int seconds)
{
	alarm(seconds);//定时指定的秒数
	struct sigaction act,oldact;
	act.sa_handler = catch_sigalrm;//捕捉函数
	sigemptyset(&act.sa_mask);//捕捉函数执行期间,屏蔽的信号
	act.sa_flags = 0;//捕捉函数执行期间,自动屏蔽本信号
	int ret = sigaction(SIGALRM,&act,&oldact);
	if(ret == -1)
	{
		perror("sigaction error");
		exit(1);
	}
	ret = pause();
	if(ret == -1 && errno == EINTR)
	{
		printf("pause interrupt\n");
	}
	ret = alarm(0);//取消定时器
	sigaction(SIGALRM,&oldact,NULL);//恢复旧的处理方式
	return ret;
}

int main(void)
{
	while(1)
	{
		mysleep(3);	
	}
	return 0;
}
