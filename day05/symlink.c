#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int args,char* argv[])
{
	if(args < 3)
	{
		printf("./a.out targetPath linkPath\n");
		exit(1);
	}
	int ret = symlink(argv[1],argv[2]);
	if(ret == -1)
	{
		perror("symlink");
		exit(1);
	}
	return 0;
}
