#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int args,char* argv[])
{
	if(args < 2)
	{
		printf("./a.out filename\n");
		exit(1);
	}
	int ret = chown(argv[1],1001,1000);
	if(ret == -1)
	{
		perror("chown");
		exit(1);
	}
	return 0;
}
