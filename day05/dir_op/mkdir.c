#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int args,char* argv[])
{
	if(args < 2)
	{
		printf("./a.out dirname");
		exit(1);
	}
	int ret = mkdir(argv[1],0755);
	if(ret == -1)
	{
		perror("mkdir")	;
		exit(1);
	}
	return 0;
}
