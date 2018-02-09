#include <stdio.h>

extern char** environ;

int main(void)
{
	//打印该进程下的所有环境变量
	int i = 0;
	for(i = 0;environ[i] != NULL;i++)
	{
		printf("%s\n",environ[i]);
	}
	return 0;
}
