#include <stdio.h>
#include <stdlib.h>
#include <net/if.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SELF_PORT 9527
#define GROUP_IP "239.9.4.46"

int main(void)
{
	int sockfd;
	struct sockaddr_in self_addr;
	char buf[BUFSIZ];
	struct ip_mreqn group;
	
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	//绑定IP和端口号
	self_addr.sin_family = AF_INET;
	self_addr.sin_port = htons(SELF_PORT);
	inet_pton(AF_INET,"0.0.0.0",&self_addr.sin_addr.s_addr);
	int ret = bind(sockfd,(struct sockaddr*)&self_addr,sizeof(self_addr));
	if(ret < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	//将该主机加入到组播中
	inet_pton(AF_INET,GROUP_IP,&group.imr_multiaddr);
	inet_pton(AF_INET,"0.0.0.0",&group.imr_address);
	group.imr_ifindex = if_nametoindex("ens33");
	setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&group,sizeof(group));
	
	while(1)
	{
		int n = recvfrom(sockfd,buf,sizeof(buf),0,NULL,0);
		write(STDOUT_FILENO,buf,n);
	}
	close(sockfd);
	exit(EXIT_SUCCESS);
}
