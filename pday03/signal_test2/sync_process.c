#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

int n = 0,flag = 0;

void sys_err(char * str)
{
	perror(str);
	exit(1);
}

void do_sig_child(int signo)
{
	printf("i am child process and my pid is %d \t %d\n",getpid(),n);
	n += 2;
	flag = 1;//表示数数完成
//	sleep(1);
}

void do_sig_parent(int signo)
{
	printf("i am parent process and my pid is %d \t %d\n",getpid(),n);
	n += 2;
	flag = 1;//表示数数完成
//	sleep(1);
}

int main(void)
{
	//创建一个子进程
	pid_t pid = fork();
	struct sigaction act;
	if(pid == -1)
		sys_err("fork error");
	if(pid > 0)
	{
		//父进程
		//初始化n的值
		n = 1;
		sleep(1);
		//初始化注册捕捉
		act.sa_handler = do_sig_parent;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGUSR2,&act,NULL);//注册捕捉SIGUSR2的函数
		
		//让父进程先启动
		do_sig_parent(0);

		while(1)
		{
			if(flag == 1)
			{
				kill(pid,SIGUSR1);//给子进程发送SIGUSR1的信号
				flag = 0;//表示发送完成
			}
		}
	}
	if(pid == 0)
	{
		//子进程
		n = 2;
		pid_t ppid = getppid();
		//初始化注册捕捉
		act.sa_handler = do_sig_child;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGUSR1,&act,NULL);//注册捕捉SIGUSR1x信号
		while(1)
		{
			if(flag == 1)
			{
				kill(ppid,SIGUSR2);//给父进程发送SIGUSR2的信号
				flag = 0;//表示发送完成
			}
		}
		
	}
	return 0;
}
