#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

//父进程fork3个子进程,三个子进程一个调用ps指令
//一个调用自定义程序1,一个调用自定义指令2(出错程序)
//父进程使用waitpid对齐子进程进行回收

int main(void)
{
	int i;
	int n = 3;//子进程数
	int status;//状态值
	pid_t pid;//进程ID
	pid_t wpid;//回收成功的子进程ID
	//首先创建三个子进程
	for(i = 0;i < n;i++)
	{
		pid = fork();
		if(pid == -1)
		{
			perror("fork");
			exit(1);
		}
		if(pid == 0)
			break;//子进程
	}
	//父进程
	if(i == n)
	{
		//首先休眠iseconds
		sleep(i);
		//开始回收子进程
		do
		{
			wpid = waitpid(-1,&status,WNOHANG);//使用不阻塞的方式回收子进程
			if(wpid > 0)
			{
				//回收成功了一个子进程,获取其状态值
				if(WIFEXITED(status))
					printf("pid is %u,exit with %d\n",wpid,WEXITSTATUS(status));
				if(WIFSIGNALED(status))
					printf("pid is %u,kill by %d signal\n",wpid,WTERMSIG(status));
				n--;
			}
		}while(n > 0);
		printf("i am parent process and my pid is %u\n",getpid());
	}
	//子进程一
	if(i == 0)
	{
		printf("i am %dth child process and my pid is %u\n",i+1,getpid());
		//将ps aux的结果打印到文件中
		//创建一个文件
		int fd = open("out",O_WRONLY | O_CREAT | O_TRUNC,0644);
		if(fd == -1)
		{
			perror("file open");
			exit(1);
		}
		//使用dup2重定向标准输出
		dup2(fd,STDOUT_FILENO);
		//执行ps命令
		execlp("ps","ps","aux",NULL);
		perror("execlp");
		exit(1);
	}
	//子进程二
	if(i == 1)
	{
		printf("i am %dth child process and my pid is %u\n",i+1,getpid());
		//执行正常的程序
		sleep(1);
		execl("./good","good",NULL);
	}
	//子进程三
	if(i == 2)
	{
		printf("i am %dth child process and my pid is %u\n",i+1,getpid());
		//执行错误的程序
		sleep(2);
		execl("./bad","bad",NULL);
	}
	return 0;
}
