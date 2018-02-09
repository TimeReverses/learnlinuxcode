#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

void sys_err(char* str)
{
	perror(str);
	exit(1);
}

void catch_sigalrm(int no)
{
}

unsigned int mysleep(unsigned int seconds)
{
	//注册信号捕捉
	struct sigaction act,oldact;
	act.sa_handler = catch_sigalrm;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGALRM,&act,&oldact);
	//屏蔽sigalrm信号
	sigset_t newmask,oldmask,suspmask;
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGALRM);
	sigprocmask(SIG_BLOCK,&newmask,&oldmask);
	
	sigemptyset(&suspmask);
	suspmask = oldmask;
	sigdelset(&suspmask,SIGALRM);
	//定时
	alarm(seconds);
	sigsuspend(&suspmask);
	
	int ret = alarm(0);
	//恢复
	sigprocmask(SIG_SETMASK,&oldmask,NULL);
	//恢复就得处理方式
	sigaction(SIGALRM,&oldact,NULL);
	return ret;
}

int main(void)
{
	//创建一个子进程,父进程终止
	pid_t pid,sid;
	pid = fork();
	if(pid == -1)
	{
		sys_err("fork error");
	}
	if(pid > 0)
		return 0;
	//子进程创建一个新会话
	sid = setsid();
	if(sid == -1)
		sys_err("setsid error");
	//改变当前进程的工作目录
	int ret = chdir("/home/jylee");
	if(ret == -1)
		sys_err("chdir error");
	//改变文件权限的掩码
	umask(0022);
	//关闭0,1,2三个文件操作符,可以重定向到/dev/null
	close(STDIN_FILENO);
	int fd = open("/dev/null",O_RDWR);//使用读写权限打开/dev/null文件
	fd = open("/home/jylee/out",O_RDWR|O_CREAT|O_TRUNC,0644);
	if(fd == -1)
		sys_err("open error");
	//使用dup2重定向
	dup2(fd,STDOUT_FILENO);
	dup2(fd,STDERR_FILENO);

	//执行守护线程的核心工作
	//没两秒中将当前系统的时间写到out文件中
	while(1)
	{
		sleep(2);
		pid = fork();
		if(pid == 0)
		{
			execlp("date","date",NULL);
			break;
		}
		if(pid > 0)
		{
			wait(NULL);
		}
	}
	return 0;
}
