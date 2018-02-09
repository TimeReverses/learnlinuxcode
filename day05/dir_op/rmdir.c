#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int args,char* argv[])
{
	if(args < 2)	
	{
		printf("./a.out dirname");
		exit(1);
	}
	int ret = rmdir(argv[1]);
	if(ret == -1)
	{
		perror("rmdir");
		exit(1);
	}
	return 0;
}
