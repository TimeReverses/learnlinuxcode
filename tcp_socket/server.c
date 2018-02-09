#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>

void sys_err(char* msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void err_pri(char* msg)
{
	fprintf(stderr,"%s\n",msg);
	exit(EXIT_FAILURE);
}

int main(void)
{
	//创建一个套接字
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd == -1)
		sys_err("socket");
	//绑定ip地址和端口号
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9527);
	struct in_addr dst;
	const char* ipaddr = "192.168.206.137";
	inet_pton(AF_INET,ipaddr,&dst);
	addr.sin_addr = dst;
	int ret = bind(sfd,(struct sockaddr*)&addr,sizeof(addr));
	if(ret == -1)
		sys_err("bind error");
	//设置同时连接数
	ret = listen(sfd,50);
	if(ret == -1)
		sys_err("listen");
	//接受等待客户端的连接
	struct sockaddr_in clientaddr;
	socklen_t clientlen = sizeof(clientaddr);
	int cfd = accept(sfd,(struct sockaddr*)&clientaddr,&clientlen);
	if(cfd == -1)
		sys_err("accept");
	//读取客户端传来的数据
	char buf[1024] = {0};
	int datasize = 0;//读取的数据的大小
	while((datasize = read(cfd,buf,sizeof(buf))) != 0)
	{
		//将小写的字母转换为大写的字母
		for(int i = 0;i < datasize;i++)
		{
			if(buf[i] >= 'a' && buf[i] <= 'z')
			{
				buf[i] = toupper(buf[i]);
			}
		}
		//将结果会写给客户端
		int len = write(cfd,buf,datasize);
		if(len == datasize)
		{
			printf("success tranfer %d bytes to client\n",len);
		}
	}
	//关闭文件描述符
	ret = close(cfd);
	if(ret == -1)
		sys_err("close");
	ret = close(sfd);
	if(ret == -1)
		sys_err("close");
}
