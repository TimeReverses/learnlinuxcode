#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void sys_err(char* str)
{
	perror(str);
	exit(1);
}

void do_sig_child(int num)
{
	//回收子进程
	pid_t pid;
	int status;
	while((pid = waitpid(0,&status,WNOHANG))>0)
	{
		if(WIFEXITED(status))
			printf("%d child process exit with %d\n",pid,WEXITSTATUS(status));
		else if(WIFSIGNALED(status))
			printf("%d child process killed by %d\n",pid,WTERMSIG(status));
	}
}

int main(void)
{
	pid_t pid;
	int i ;
	//创建10个子进程
	for(i=0;i<10;i++)
	{
		pid = fork();
		if(pid == -1)
			sys_err("fork error");
		if(pid == 0)
			break;
	}
	if(pid == 0)
	{
		//子进程
		int n = 1;	
		while(n --){
			printf("child %d\n",getpid());
			sleep(1);
		{
		return i+1;
	}
	if(pid > 0)
	{
		//阻塞SIGCHLD信号
		sigset_t newmask,oldmask;
		sigemptyset(&newmask);
		sigaddset(&newmask,SIGCHLD);
		sigprocmask(SIG_BLOCK,&newmask,&oldmask);
		//注册信号捕捉
		struct sigaction act,oldact;
		act.sa_handler = do_sig_child;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGCHLD,&act,&oldact);
		//解放阻塞的信号
		sigprocmask(SIG_SETMASK,&oldmask,NULL);
		
		while(1)
		{
			printf("i am parent proces    %d\n",getpid());
			sleep(1);
		}
	}
	return 0;
}
