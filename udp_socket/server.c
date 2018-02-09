#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>

#define SERV_IP "192.168.206.137"
#define SERV_PORT 9527

int main(void)
{
	struct sockaddr_in server_addr,client_addr;
	socklen_t client_addr_len;
	int sockfd,i,n;
	char buf[BUFSIZ],str[INET_ADDRSTRLEN];
	//创建一个socket
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	//绑定IP和端口号
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,SERV_IP,&server_addr.sin_addr.s_addr);
	int ret = bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(ret < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		//接受数据
		client_addr_len = sizeof(client_addr);
		n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&client_addr,&client_addr_len);
		//打印客户端信息
		printf("client connect success,IP is %s,port is %d\n",\
			inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,str,sizeof(str)),\
			ntohs(client_addr.sin_port));
		if(n < -1)
		{
			if(errno == EINTR)
				continue;
			else
			{
				close(sockfd);
				exit(EXIT_FAILURE);
			}
		}
		else if(n > 0)
		{
			//将小写转换为大写
			for(i = 0;i < n;i++)
			{
				buf[i] = toupper(buf[i]);
			}
			//发送给客户端
			n = sendto(sockfd,buf,n,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
		}
	}
	close(sockfd);
	exit(EXIT_SUCCESS);
}
