#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
	//创建一个socket
	int fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	//可以使用bind绑定一个IP和端口,也可以使用隐式绑定,这里就使用"隐式绑定"
	//和服务器建立连接
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9527);//服务器端的端口号
	struct in_addr ip;
	int ret = inet_pton(AF_INET,"192.168.206.137",&ip);
	if(ret == 0)
	{
		fprintf(stderr,"ip error\n");
	}
	else if(ret == -1)
	{
		perror("inet_pton");
		exit(EXIT_FAILURE);
	}
	addr.sin_addr = ip;
	ret = connect(fd,(struct sockaddr*)&addr,sizeof(addr));
	if(ret == -1)
	{
		perror("connect");
		exit(EXIT_FAILURE);
	}
	char buf[1024] = {0};
	int len = 0;
	while(1)
	{
		memset(buf,0,sizeof(buf));
		char* tmp = fgets(buf,sizeof(buf),stdin);
		if(tmp != NULL && strcmp(buf,"exit\n") != 0)
		{
			//将数据写给服务端
			write(fd,tmp,strlen(tmp));
			len = read(fd,buf,sizeof(buf));
			printf("%s",buf);
		}
		else
		{
			break;
		}
	}
	close(fd);
	exit(EXIT_SUCCESS);
}
