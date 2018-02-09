#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "wrap.h"

#define SERV_IP "192.168.206.137"
#define SERV_PORT 9527
#define MAXNUM 10

int main(void)
{
	int nready,n,ret,epfd,flag;
	struct sockaddr_in server_addr,client_addr;
	socklen_t client_addr_len;
	int listenfd,connfd;
	char str[INET_ADDRSTRLEN],buf[BUFSIZ];
	
	//创建socket
	listenfd = Socket(AF_INET,SOCK_STREAM,0);
	//绑定IP和port
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,SERV_IP,&server_addr.sin_addr.s_addr);
	Bind(listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	//设置最大通知建立连接数
	Listen(listenfd,64);
	
	struct epoll_event evt;
	struct epoll_event evts[10];
	epfd = epoll_create(10);//创建红黑树
	evt.events = EPOLLIN | EPOLLET;//设置为沿边模式
	evt.data.fd = listenfd;
	if(epfd == -1)
		sys_err("epoll_create");
	client_addr_len = sizeof(client_addr);
	connfd = Accept(listenfd,(struct sockaddr*)&client_addr,&client_addr_len);
	printf("client connect success,IP is %s,port is %d\n",\
		inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,str,sizeof(str)),\
		ntohs(client_addr.sin_port));
	//设置connfd为非阻塞状态,使用fcntl
	flag = fcntl(connfd,F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(connfd,F_SETFL,flag);
	
	//将connfd放到红黑树上
	evt.data.fd = connfd;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&evt);
	if(ret == -1)
		sys_err("epoll_ctl");
	while(1)
	{
		nready = epoll_wait(epfd,evts,10,-1);
		if(nready < 0)
			sys_err("epoll_wait");
		printf("have data coming   %d  \n",nready);
		if(evts[0].data.fd == connfd)
		{
			while((n = read(connfd,buf,MAXNUM/2)) > 0)
				write(STDOUT_FILENO,buf,n);
		}
		printf("have data end   %d  \n",nready);
	}
	Close(connfd);
	Close(listenfd);
	Close(epfd);
	exit(EXIT_SUCCESS);
}
