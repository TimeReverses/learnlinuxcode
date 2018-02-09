#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int args,char* argv[])
{
	if(args < 2)
	{
		printf("./a.out filename \n");
		exit(1);
	}
	//创建一个stat结构体对象
	struct stat st;
	//stat和lstat只有在软连接的时候有区别,stat函数有穿透的功能,lstat不穿透
	int ret = lstat(argv[1],&st);
	if(ret == -1)
	{
		perror("stat");
		exit(1);
	}
	//获取文件的大小
	int size = (int)st.st_size;
	printf("%s's size is %d bytes\n",argv[1],size);
	
	return 0;
}
