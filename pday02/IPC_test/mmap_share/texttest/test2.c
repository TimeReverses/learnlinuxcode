#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	int fd = open("test.txt",O_RDWR);
	if(fd == -1)
	{
		perror("file open");
		exit(1);
	}
	sleep(1);
	char buf[1024] = {0};
	int len = read(fd,buf,sizeof(buf));
	write(STDOUT_FILENO,buf,len);
	
	//指针不移动,直接写数据
	char* str = "--------test2 write----------\n";
	write(fd,str,strlen(str));
	close(fd);
	return 0;
}
