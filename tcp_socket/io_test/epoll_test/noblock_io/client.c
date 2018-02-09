#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "wrap.h"

#define SERV_IP "192.168.206.137"
#define SERV_PORT 9527
#define MAXNUM 10
int main(void)
{
	int connfd,i;
	struct sockaddr_in server_addr;
	char ch,buf[MAXNUM];
	ch = 'a';
	connfd = Socket(AF_INET,SOCK_STREAM,0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,SERV_IP,&server_addr.sin_addr.s_addr);
	Connect(connfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	while(1)
	{
		for(i = 0;i < MAXNUM/2 ;i++)
			buf[i] = ch;
		buf[i-1] = '\n';
		ch++;
		for(;i < MAXNUM;i++)
			buf[i] = ch;
		buf[i -1] = '\n';
		ch++;
		write(connfd,buf,MAXNUM);
		sleep(5);
	}
	close(connfd);
	exit(EXIT_SUCCESS);
}
