#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	int sockfd,n;
	struct sockaddr_in server_addr;
	char buf[BUFSIZ] = {0};
	//创建一个socket
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9527);
	inet_pton(AF_INET,"192.168.206.137",&server_addr.sin_addr.s_addr);
	while(fgets(buf,sizeof(buf),stdin) != NULL)
	{
		n = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
		if(n < 0)
		{
			perror("sendto");
			close(sockfd);
			exit(EXIT_FAILURE);
		}
		n = recvfrom(sockfd,buf,sizeof(buf),0,NULL,0);
		if(n < 0)
		{
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}
		write(STDOUT_FILENO,buf,n);
	}
	close(sockfd);
	exit(EXIT_SUCCESS);
}
