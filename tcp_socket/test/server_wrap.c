#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include "wrap.h"

#define SERV_PORT 9527
#define SERV_IP "192.168.2.6.137"
#define MAXNUM 64
int main(void)
{
	int sfd,cfd;
	struct sockaddr_in server_addr,client_addr;
	socklen_t client_len;
	ssize_t datasize;
	char dstaddr[BUFSIZ],buf[BUFSIZ];
	//创建一个socket
	sfd = Socket(AF_INET,SOCK_STREAM,0);
	//绑定IP和port
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,SERV_IP,&server_addr.sin_addr.s_addr);
	Bind(sfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	//设置最大连接数
	Listen(sfd,MAXNUM);
	//阻塞等待客户端连接
	client_len = sizeof(client_addr);
	cfd = Accept(sfd,(struct sockaddr*)&client_addr,&client_len);
	printf("client connect success,IP : %s , port : %d\n",\
		inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,dstaddr,sizeof(dstaddr)),\
		ntohs(client_addr.sin_port));
	//服务器读取客户端的数据
	while((datasize = Read(cfd,buf,sizeof(buf))) != 0)
	{
		for(int i=0;i<datasize;i++)
			buf[i] = toupper(buf[i]);
		Write(cfd,buf,datasize);
		printf("%ld bytes data has been tranfered success\n",datasize);
	}
	Close(cfd);
	Close(sfd);
	exit(EXIT_SUCCESS);
}

