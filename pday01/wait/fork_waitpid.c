#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	//首先创建存储pid的变量
	pid_t pid,wpid;
	//指定创建子进程的数量
	int num = 5;
	int i;
	pid_t zpid;
	for(i=0;i< num;i++)
	{
		pid = fork();
		if(pid == -1)
		{
			perror("fork");
			exit(1);
		}
		if(pid == 0)
		{//子进程被成功创建
			break;
		}
		if(i == 2)
		{
			zpid = pid;
			printf("zpid = %u\n",zpid);
		}
	}
	if(i == num)
	{
		//父进程中
		//sleep(1);
		//使用waitpid回收子进程,回收一个指定子进程
		//wpid = waitpid(zpid,NULL,0);//options赋值为0,是阻塞状态
		//使用waitpid回收说有的进程,方式一:类似wait
		/*
		while((wpid = waitpid(-1,NULL,0)) != 4294967295)	
		{
			printf("wpid = %u\n",wpid);
			sleep(1);
		}
		*/
		//使用waitpid不阻塞的方式回收进程
		sleep(1);
		do
		{
			wpid = waitpid(-1,NULL,WNOHANG);
			if(wpid > 0)//回收了一个子进程
				num--;
		}while(num > 0);
		printf("i am parent process, my pid is %u\n",getpid());
		//while(1);
	}
	if(i < num)
	{
		//子进程中
		sleep(i);
		printf("i am %dth child process,my pid is %u,my ppid is %u\n",i+1,getpid(),getppid());
	}
}
