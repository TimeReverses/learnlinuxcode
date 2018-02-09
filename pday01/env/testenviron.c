#include <stdio.h>
//全局环境变量表的声明
extern char** environ;

int main(void)
{
	int i;
	//通过判断environ[i]是否是NULL,来确定是否结束,在环境变量表中NULL表示的是结尾
	for(i = 0;environ[i];i++)
	{
		printf("%s\n",environ[i]);	
	}
	return 0;
}
