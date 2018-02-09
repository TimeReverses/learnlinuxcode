#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main(int args,char* argv[])
{
	if(args < 3)
	{
		printf("./a.out filename mode\n");
		exit(1);
	}
	//将字符串转换为一个8进制的数
	long int mod = strtol(argv[2],NULL,8);
	int ret = chmod(argv[1],mod);
	if(ret == -1)
	{
		perror("chmod");
		exit(1);
	}
	return 0;	
}
