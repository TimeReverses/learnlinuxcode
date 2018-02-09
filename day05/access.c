#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int args,char* argv[])
{
	if(args <2)
	{
		printf("./a.out filename");
		exit(1);	
	}
	//首先判断是否存在
	int ret = access(argv[1],F_OK);
	if(ret == 0)
	{
		printf("文件存在\n");
		//文件存在
		//判断是否有读权限
		if(access(argv[1],R_OK) == 0)
		{
			printf("有读权限\n");
			if(access(argv[1],W_OK) == 0)
			{
				printf("有写权限\n");
				if(access(argv[1],X_OK) == 0)
				{
					printf("有执行的权限\n");
				}
				else
				{
					printf("没有执行的权限\n");
				}
			}
			else
			{
				printf("没有写权限\n");
			}
		}
		else
		{
			printf("没有读权限\n");
		}
	}	
	else
	{
		printf("文件不存在\n");
	}
	return 0;
}
