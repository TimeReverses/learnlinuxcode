#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
	int sockfd;
	struct sockaddr_in self_addr;
	int n;
	char buf[BUFSIZ];
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	//绑定端口
	self_addr.sin_family = AF_INET;
	self_addr.sin_port = htons(9527);
	inet_pton(AF_INET,"0.0.0.0",&self_addr.sin_addr.s_addr);
	int ret = bind(sockfd,(struct sockaddr*)&self_addr,sizeof(self_addr));
	if(ret < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		n = recvfrom(sockfd,buf,sizeof(buf),0,NULL,0);
		if(n > 0)	
		{
			write(STDOUT_FILENO,buf,n);
		}
	}
	close(sockfd);
	exit(EXIT_SUCCESS);
}
