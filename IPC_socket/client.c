#include <stdio.h>
#include <sys/un.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "wrap.h"

#define SERV_ADDR "server.socket"
#define CLIE_ADDR "client.socket"

int main(void)
{
	int cfd,len,n;
	struct sockaddr_un server_addr,client_addr;
	char buf[BUFSIZ];
	
	//创建一个socket
	cfd = Socket(AF_UNIX,SOCK_STREAM,0);
	//初始化client_addr
	client_addr.sun_family = AF_UNIX;
	strcpy(client_addr.sun_path,CLIE_ADDR);
	
	len = offsetof(struct sockaddr_un,sun_path) + strlen(client_addr.sun_path);
	//删除旧的套接字	
	unlink(CLIE_ADDR);
	Bind(cfd,(struct sockaddr*)&client_addr,sizeof(client_addr));
	
	//初始化server_addr
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path,SERV_ADDR);
	len = offsetof(struct sockaddr_un,sun_path) + strlen(server_addr.sun_path);
		
	//建立连接
	Connect(cfd,(struct sockaddr*)&server_addr,len);
	while(fgets(buf,sizeof(buf),stdin) != NULL)
	{
		n = write(cfd,buf,strlen(buf));
		n = read(cfd,buf,sizeof(buf));
		write(STDOUT_FILENO,buf,n);
	}
	close(cfd);
	exit(EXIT_SUCCESS);
}
