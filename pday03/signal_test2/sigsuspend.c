#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

void catch_sigalrm(int signo)
{
}

unsigned int mysleep(unsigned int seconds)
{
	struct sigaction act,oldact;
	sigset_t newmask,oldmask,suspmask;

	act.sa_handler = catch_sigalrm;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	//注册捕捉SIGALRM信号的处理函数
	sigaction(SIGALRM,&act,&oldact);

	//屏蔽SIGALRM信号
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGALRM);
	sigprocmask(SIG_BLOCK,&newmask,&oldmask);
	
	//sigsuspend执行期间屏蔽的信号
	sigemptyset(&suspmask);
	suspmask = oldmask;
	sigdelset(&suspmask,SIGALRM);//再次删除sigalrm信号的屏蔽,防止	

	//定时
	alarm(seconds);
	//挂起
	int msg = sigsuspend(&suspmask);
	if(msg == -1 && errno == EINTR)
	{
		printf("sigsuspend has been interrupted\n");
	} 
	
	//取消定时器
	int ret = alarm(0);
	//恢复旧的处理SIGALRM信号的方式
	sigaction(SIGALRM,&oldact,NULL);
	//恢复旧的屏蔽字
	sigprocmask(SIG_SETMASK,&oldmask,NULL);
	return ret;
}

int main(void)
{
	while(1)
	{
		mysleep(2);
	}
	return 0;
}
