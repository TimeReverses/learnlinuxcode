#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int i = 0;
	unsigned int ret = alarm(1);
	while(1)
	{
		printf("%d\n",i++);
	}
	return 0;
}
