#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include "wrap.h"

#define SERV_IP "192.168.206.137"
#define SERV_PORT 9527
#define MAXCONN 5000

int main(void)
{
	int i,j,n,ret,nready;
	struct sockaddr_in server_addr,client_addr;
	socklen_t client_addr_len;
	char buf[BUFSIZ],str[INET_ADDRSTRLEN];
	int epfd,listenfd,connfd,sockfd;
	struct epoll_event evt,evts[MAXCONN];

	//清零
	bzero(&server_addr,sizeof(server_addr));
	//创建socket
	listenfd = Socket(AF_INET,SOCK_STREAM,0);
	//端口复用
	int opt = 1;
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	//绑定IP和port
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,SERV_IP,&server_addr.sin_addr.s_addr);
	Bind(listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	//设置同时连接的最大数
	Listen(listenfd,64);
	//创建一个红黑树
	epfd = epoll_create(MAXCONN);
	if(epfd == -1)
		sys_err("epoll_create");
	//将listenfd节点添加到树中
	evt.events = EPOLLIN;
	evt.data.fd = listenfd;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&evt);
	if(ret == -1)
		sys_err("epoll_ctl");
	while(1)
	{
		//开始监听
		nready = epoll_wait(epfd,evts,MAXCONN,-1);//evts传入的是空的数组,当epoll_wait函数返回的时候
		//,会将满足的事件结构体放到该数组中
		if(nready == -1)
			sys_err("epoll_wait");
		for(i = 0;i < nready;i++)
		{
			if(!(evts[i].events & EPOLLIN))//如果不是读事件,直接跳过
				continue;
			if(evts[i].data.fd == listenfd)//如果是有新的客户端连接的事件
			{
				client_addr_len = sizeof(client_addr);
				connfd = Accept(listenfd,(struct sockaddr*)&client_addr,&client_addr_len);
				//打印客户端的信息
				printf("client connect success,IP is %s,port is %d\n",\
						inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,str,sizeof(str)),\
						ntohs(client_addr.sin_port));
				//根据文件描述符,创建一个节点,并将其放入到树中
				evt.events = EPOLLIN;
				evt.data.fd = connfd;
				ret = epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&evt);
				if(ret == -1)
					sys_err("epoll_ctl");
			}
			else
			{
				sockfd = evts[i].data.fd;
				//读取数据
				n = Read(sockfd,buf,sizeof(buf));
				if(n == 0)//客户端关闭
				{
					//将这个节点从树中删除
					ret = epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
					if(ret == -1)
						sys_err("epoll_ctl");
					Close(sockfd);
					printf(" %d client closing connection\n",sockfd);
				}
				else if(n < 0)
				{
					//出错
					ret = epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
					if(ret == -1)
						sys_err("epoll_ctl");
					Close(sockfd);
					printf("%d client read error\n",sockfd);
				}
				else
				{
					for(j = 0;j < n;j++)
						buf[j] = toupper(buf[j]);
					Write(sockfd,buf,n);
					Write(STDOUT_FILENO,buf,n);
				}
			}
		}	
	}
}
