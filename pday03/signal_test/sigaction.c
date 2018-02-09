#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void catch2(int signo)
{
	printf("--------signal %d has been catched\n",signo);
	sleep(10);
	printf("-------finish-------\n");
}

//使用sigaction注册一个信号捕捉函数
int main(void)
{
	//定义一个结构体
	struct sigaction sig;
	sig.sa_handler = catch2;
	sigemptyset(&sig.sa_mask);//设置在捕捉函数运行期间,阻塞的信号集
	sigaddset(&sig.sa_mask,SIGQUIT);
	sig.sa_flags = 0;//0位默认值,表示在捕捉函数执行期间,自动屏蔽本信号
	int ret = sigaction(SIGINT,&sig,NULL);	
	if(ret == -1)
	{
		perror("sigaction");
		exit(1);
	}
	while(1);
	return 0;
}
