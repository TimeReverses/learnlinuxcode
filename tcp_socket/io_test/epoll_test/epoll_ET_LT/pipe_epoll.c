#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#define MAXLINE 10

int main(void)
{
	int i,nready,epfd,n;
	char c ,buf[MAXLINE],str[BUFSIZ];
	pid_t pid;
	int fds[2];
	int ret = pipe(fds);
	if(ret == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if(pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	c = 'a';
	if(pid == 0)
	{//子进程  写数据
		//关闭读端
		close(fds[0]);
		while(1)
		{
			for(i = 0;i < MAXLINE/2;i++)
			{
				buf[i] = c;
			}
			buf[i -1] = '\n';
			c++;
			for(;i < MAXLINE;i++)
				buf[i] = c;
			buf[i - 1] = '\n';
			c++;
			write(fds[1],buf,MAXLINE);
			sleep(5);
		}
		close(fds[1]);
	}
	else if(pid > 0)
	{
		struct epoll_event evt,evts[10];
		//关闭写端
		close(fds[1]);
		//创建一个红黑树
		epfd = epoll_create(10);
		if(epfd == -1)
		{
			perror("epoll_create");
			exit(EXIT_FAILURE);
		}
		//将读端放入到树中
		//evt.events = EPOLLIN;//这个是水平模式,(LT模式,是默认模式)
		evt.events = EPOLLIN|EPOLLET;//这是边沿模式(ET模式)
		evt.data.fd = fds[0];
		ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fds[0],&evt);
		while(1)
		{
			nready = epoll_wait(epfd,evts,10,-1);
			if(evts[i].data.fd == fds[0])
			{
				n = read(evts[i].data.fd,str,MAXLINE/2);
				if(n > 0)	
				{
					write(STDOUT_FILENO,str,n);
				}
				else
				{
					evt.events = EPOLLIN;//这个是水平模式,(LT模式,是默认模式)
					evt.data.fd = fds[0];
					ret = epoll_ctl(epfd,EPOLL_CTL_DEL,fds[0],&evt);
					close(fds[0]);
				}
			}
		}
		close(fds[0]);
	}
	return 0;
}
