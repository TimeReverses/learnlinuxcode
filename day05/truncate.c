#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

//给一个文件拓展空间
int main(int args,char* argv[])
{
	if(args < 2)
	{
		printf("./a.out filename\n");
		exit(1);
	}
	int ret = truncate(argv[1], 300);
	if(ret == -1)
	{
		perror("truncate");
		exit(1);
	}
	return 0;
}
