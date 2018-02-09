#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <poll.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include "wrap.h"

#define SERV_IP "192.168.206.137"
#define SERV_PORT 9527
#define MAX_NUM 1024

void pri_err(const char* msg)
{
	fprintf(stderr,"%s",msg);
	exit(EXIT_FAILURE);
}

int main(void)
{
	int i,j,n,maxi,nready;
	int listenfd,connfd,sockfd;
	struct sockaddr_in server_addr,client_addr;
	socklen_t client_addr_len;
	struct pollfd client[MAX_NUM];	
	char buf[BUFSIZ],str[INET_ADDRSTRLEN];
	//创建一个socket
	listenfd = Socket(AF_INET,SOCK_STREAM,0);
	//端口复用
	int opt = 1;
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	//绑定IP和port
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,SERV_IP,&server_addr.sin_addr.s_addr);
	Bind(listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	//设置最大同时建立连接数
	Listen(listenfd,64);

	//将第一个listenfd放入到client中
	client[0].fd = listenfd;
	client[0].events = POLLIN;

	for(i = 1;i < MAX_NUM;i++)
		client[i].fd = -1;
	maxi = 0;
	while(1)
	{
		nready = poll(client,maxi+1,-1);
		if(nready < 0)
			sys_err("poll");
		if(client[0].revents & POLLIN)//有连接请求
		{
			client_addr_len = sizeof(client_addr);
			connfd = Accept(listenfd,(struct sockaddr*)&client_addr,&client_addr_len);
			printf("client connect success,ip is %s,port is %d\n",\
					inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,str,sizeof(str)),\
					ntohs(client_addr.sin_port));
			//将接受到的客户端通信的fd添加到client数组中
			for(i = 1;i < MAX_NUM;i++)
			{
				if(client[i].fd < 0)
				{
					client[i].fd = connfd;
					break;
				}
			}
			if(i == MAX_NUM)
				pri_err("too many clients\n");
			client[i].events = POLLIN;
			if(i > maxi)
				maxi = i;
			if(--nready == 0)
				continue;
		}
		//如果还有数据请求
		for(i = 1;i <= maxi;i++)
		{
			if((sockfd = client[i].fd) < 0)
				continue;
			if(client[i].revents & POLLIN)
			{
				if((n = Read(sockfd,buf,sizeof(buf))) < 0)
				{
					if(errno == ECONNRESET)
					{
						printf("client[%d] aborted connection \n",i);
						Close(sockfd);
						sockfd = -1;
					}else
						sys_err("read");
				}
				else if(n == 0) //客户端关闭
				{
					printf("client[%d] closing connection \n",i);
					Close(sockfd);
					sockfd = -1;
				}
				else
				{
					//读写数据
					for(j = 0;j < n;j++)
						buf[j] = toupper(buf[j]);
					write(sockfd,buf,n);
					printf("transfer successful data %d bytes\n",n);
				}
				if(--nready == 0)
					break;
			}
		}
	}
	Close(listenfd);
	exit(EXIT_SUCCESS);
}
