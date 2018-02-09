#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main(void)
{
	int fd = open("test.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
	if(fd == -1)
	{
		perror("file open");
		exit(1);
	}	
	char* str = " -----test1 write----\n";
	write(fd,str,strlen(str));
	
	sleep(20);
	
	//将操作文件的指针指向开头
	lseek(fd,0,SEEK_SET);
	//读文件
	char buf[1024] = {0};
	int len = read(fd,buf,sizeof(buf));
	write(STDOUT_FILENO,buf,len);
	close(fd);
	return 0;
}
