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
	char buf[512] = {0};
	int size = readlink(argv[1],buf,sizeof(buf));
	if(size == -1)
	{
		perror("readlink");
		exit(1);
	}
	buf[size] = 0;
	printf("%s's content is %s and size is %d bytes\n",argv[1],buf,size);
	return 0;
}
