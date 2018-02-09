#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int args,char* argv[])
{
	//判断参数
	if(args < 2)
	{
		printf("./a.out pathname");
		exit(1);
	}
	//改标当前进程的目录	
	int ret = chdir(argv[1]);
	if(ret == -1)
	{
		perror("chdir");
		exit(1);
	}
	//在该目录下创建一个新的文件
	int fd = open("new.txt",O_CREAT | O_RDWR,0644);
	if(fd == -1)
	{
		perror("file open");
		exit(1);
	}
	//关闭文件
	close(fd);
	//获取当前工作路径
	char buf[64] = {0};
	getcwd(buf,sizeof(buf));
	printf("current dir : %s \n",buf);
	return 0;
}
