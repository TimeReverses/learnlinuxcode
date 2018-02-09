#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	int fd;
	fd = open("test",O_RDWR);
	if(fd == -1)
	{
		perror("open error");
		exit(1);
	}
	char buf[1024] = {0};
	int len = read(fd,buf,sizeof(buf));
	fputs(buf,stdout);
	int flag = fcntl(fd,F_GETFL);
	printf("flag = %d\n",flag);
	//设置追加属性
	//flag |= O_APPEND;
	flag = fcntl(fd,F_SETFL,O_APPEND);
	char* str = "hello world\n";
	write(fd,str,strlen(str));
	printf("flag = %d\n",flag);
	close(fd);
	return 0;
}
