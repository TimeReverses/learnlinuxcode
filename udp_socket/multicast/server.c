#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>

#define SERV_PORT 9446
#define GROUP_PORT 9527
#define GROUP_IP "239.9.4.46"

int main(void)
{
	int sockfd;
	struct sockaddr_in server_addr,client_addr;
	struct ip_mreqn group;
	char buf[BUFSIZ];

	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	//绑定IP和端口号
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(ret < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	
	//开放组播权限
	//初始化组播的结构体
	inet_pton(AF_INET,GROUP_IP,&group.imr_multiaddr);
	inet_pton(AF_INET,"0.0.0.0",&group.imr_address);
	group.imr_ifindex = if_nametoindex("ens33");
	setsockopt(sockfd,IPPROTO_IP,IP_MULTICAST_IF,&group,sizeof(group));

	//设置客户端的IP和port
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(GROUP_PORT);
	inet_pton(AF_INET,"0.0.0.0",&client_addr.sin_addr.s_addr);
	while(fgets(buf,sizeof(buf),stdin) != NULL)
	{
		int n = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&client_addr,sizeof(client_addr));
		if(n < 0)
			perror("sendto");
	}
	close(sockfd);
	exit(EXIT_SUCCESS);
}
