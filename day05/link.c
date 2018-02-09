#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int args,char* argv[])
{
	if(args < 3)
	{
		printf("./a.out oldpath newpath\n");
		exit(1);
	}
	//创建一个硬链接
	int ret = link(argv[1],argv[2]);
	if(ret == -1)
	{
		perror("link");
		exit(1);
	}
	return 0;
}
