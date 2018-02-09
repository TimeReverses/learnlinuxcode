#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define SERVER_IP "192.168.206.137"
#define SERVER_PORT 9446

#define BROAD_IP "192.168.206.255"
#define BROAD_PORT 9527

int main(void)
{
	int sockfd;
	char buf[BUFSIZ];
	struct sockaddr_in server_addr,client_addr;
	//创建一个socket
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	//绑定服务器端的IP和端口号
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET,SERVER_IP,&server_addr.sin_addr.s_addr);
	int ret = bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(ret < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	//修改socket的属性,使得可以广播
	int flag = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&flag,sizeof(flag));
	
	//配置广播端的IP和端口
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(BROAD_PORT);
	inet_pton(AF_INET,BROAD_IP,&client_addr.sin_addr.s_addr);
	while(fgets(buf,sizeof(buf),stdin) != NULL)
	{
		ret = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&client_addr,sizeof(client_addr));
		if(ret < 0)
		{
			perror("sendto");
			exit(EXIT_FAILURE);
		}
	}
	close(sockfd);
	exit(EXIT_SUCCESS);
}
