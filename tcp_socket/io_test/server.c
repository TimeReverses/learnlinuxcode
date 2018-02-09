#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <strings.h>
#include <ctype.h>

#include "wrap.h"

#define SERV_IP "192.168.206.137"
#define SERV_PORT 9527

int main(void)
{
	int i,j,n,maxi,nready;
	int listenfd,maxfd,connfd,sockfd;
	fd_set rset,allset;//allset 是用来备份用的,rset是用来传入select参数的
	struct sockaddr_in server_addr,client_addr;
	socklen_t client_addr_len;
	int client[FD_SETSIZE];
	char buf[INET_ADDRSTRLEN];
	char data[BUFSIZ];

	//初始化client数组
	for(j = 0;j<FD_SETSIZE;j++)
	{
		client[j] = -1;
	}
	maxi = -1;
	bzero(&server_addr,sizeof(server_addr));

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
	//设置最大同时建立三次握手的连接数
	Listen(listenfd,64);

	//清空allset
	FD_ZERO(&allset);
	//将listenfd添加到allset中
	FD_SET(listenfd,&allset);
	maxfd = listenfd;
	while(1)
	{
		rset = allset;
		//调用select函数
		nready = select(maxfd + 1,&rset,NULL,NULL,NULL);
		if(nready < 0)
		{
			perror("select");
			exit(EXIT_FAILURE);
		}
		if(FD_ISSET(listenfd,&rset))//有新的客户端向服务器发送连接请求
		{
			//调用accept函数,建立连接
			client_addr_len = sizeof(client_addr);
			connfd = Accept(listenfd,(struct sockaddr*)&client_addr,&client_addr_len);
			//打印客户端信息
			printf("client connect success,IP is %s,port is %d\n",\
					inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,buf,INET_ADDRSTRLEN),
					ntohs(client_addr.sin_port));
			//将该文件描述符,添加到客户端数组中
			for(i = 0;i < FD_SETSIZE;i++)
			{
				if(client[i] < 0)
				{
					client[i] = connfd;
					break;
				}
			}
			if(i == FD_SETSIZE)
			{
				fprintf(stderr,"too many client connect!\n");
				exit(EXIT_FAILURE);
			}
			//将连接到的客户端文件描述符,添加到读事件的集合中
			FD_SET(connfd,&allset);
			if(maxfd < connfd)
				maxfd = connfd;
			if(maxi < i)
				maxi = i;
			if(--nready == 0)
				continue;
		}

		//判断是否有已经建立好连接的客户端,发送数据过来
		for(i = 0;i <= maxi;i++)
		{
			if((sockfd = client[i]) < 0)
				continue;
			if(FD_ISSET(sockfd,&rset))//有数据传输
			{
				n = Read(client[i],data,sizeof(data));
				if(n == 0)//表示客户端写端关闭
				{
					Close(sockfd);
					client[i] = -1;
					FD_CLR(sockfd,&allset);
				}
				else if(n > 0)
				{
					for(j = 0;j <n;j++)
						data[j] = toupper(data[j]);
					write(sockfd,data,n);
					fprintf(stdout,"success tranfer %d bytes\n",n);
				}
				if(--nready == 0)
					break;
			}
		}
	}
	Close(listenfd);
	exit(EXIT_SUCCESS);
}
