#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include "wrap.h"

#define SERV_ADDR "server.socket"

int main(void)
{
	int listenfd,connfd,len,n;
	char buf[BUFSIZ];
	struct sockaddr_un server_addr,client_addr;
	socklen_t client_addr_len;
	
	//创建一个socket
	listenfd = Socket(AF_UNIX,SOCK_STREAM,0);//这里的第二个和第三个参数是随便填写的
	//初始化server_addr
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path,SERV_ADDR);
	
	//计算出结构体实际占用的大小
	len = offsetof(struct sockaddr_un,sun_path) + strlen(server_addr.sun_path);
	//先删除服务器端的套接字文件
	unlink(SERV_ADDR);
	//绑定服务器套接字文件
	Bind(listenfd,(struct sockaddr*)&server_addr,len);
	//设置同时建立连接数
	Listen(listenfd,64);
	
	while(1)
	{
		client_addr_len = sizeof(client_addr);	
		connfd = Accept(listenfd,(struct sockaddr*)&client_addr,&client_addr_len);
		client_addr_len -= offsetof(struct sockaddr_un,sun_path);
		client_addr.sun_path[client_addr_len] = '\0';
		printf("path : %s client connect success\n",client_addr.sun_path);
		while(1)
		{
			n = read(connfd,buf,sizeof(buf));
			if(n > 0)
			{
				for(int i = 0;i < n;i++)
					buf[i] = toupper(buf[i]);
				write(connfd,buf,n);
				write(STDOUT_FILENO,buf,n);
			}
		}
		close(connfd);
	}
	close(listenfd);
	exit(EXIT_SUCCESS);
}
