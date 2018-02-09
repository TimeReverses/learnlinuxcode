#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include "wrap.h"

#define SERV_PORT 9527
#define SERV_IP "192.168.206.137"
//捕捉函数
void catch_sigchld(int sig)
{
	int status;
	pid_t pid;
	while((pid = waitpid(0,&status,WNOHANG)) > 0)
	{
		if(WIFEXITED(status))
			printf("%d process exit with %d\n",pid,WEXITSTATUS(status));
		else if(WIFSIGNALED(status))
			printf("%d process signaled by %d\n",pid,WTERMSIG(status));
	}
}

int main(void)
{
	int sfd,cfd;
	struct sockaddr_in server_addr,client_addr;
	socklen_t client_len;
	char client_info[BUFSIZ];
	//注册一个捕捉SIGCHLD信号的捕捉函数
	struct sigaction act;
	sigset_t set;
	pid_t pid;

	sigemptyset(&set);
	act.sa_handler = catch_sigchld;
	act.sa_mask = set;//在捕捉函数执行期间,屏蔽的信号
	act.sa_flags = 0;//在捕捉函数执行期间,自动屏蔽本信号
	sigaction(SIGCHLD,&act,NULL);

	//创建一个socket
	sfd = Socket(AF_INET,SOCK_STREAM,0);
	//端口复用,使用setsockopt()函数
	int opt = 1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	//绑定IP和port	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,SERV_IP,&server_addr.sin_addr.s_addr);
	Bind(sfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	//设置最大同时连接数
	Listen(sfd,64);
	//接受阻塞等待子进程的连接
	while(1)
	{
		client_len = sizeof(client_addr);
		memset(&client_addr,0,client_len);
		cfd = Accept(sfd,(struct sockaddr*)&client_addr,&client_len);
		printf("client connect success,IP is %s,port is %d\n",\
				inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,client_info,sizeof(client_info)),\
				ntohs(client_addr.sin_port));
		//创建一个子进程
		pid = fork();
		if(pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if(pid == 0)
		{
			Close(sfd);
			break;
		}
		if(pid > 0)
		{
			Close(cfd);
		}
	}
	if(pid == 0)
	{
		//子进程
		char buf[BUFSIZ];
		ssize_t datasize;
		/*
		   while((datasize = Read(cfd,buf,sizeof(buf))) !=0)
		   {
		   for(int i =0;i<datasize;i++)
		   buf[i] = toupper(buf[i]);
		   Write(cfd,buf,datasize);
		   }
		 */
		while(1)
		{
			datasize = Read(cfd,buf,sizeof(buf));
			if(datasize == 0)
			{
				//客户端已经关闭	
				Close(cfd);
				exit(EXIT_SUCCESS);
			}
			else if(datasize == -1)
			{
				perror("read");
				exit(EXIT_FAILURE);	
			}
			else
			{
				for(int i =0;i<datasize;i++)
					buf[i] = toupper(buf[i]);
				Write(cfd,buf,datasize);
				fprintf(stdout,"success tranfer %ld bytes\n",datasize);
			}
		}
	}
	else if(pid > 0)
	{
		//父进程
		Close(sfd);
	}
}
