#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int getFileNum(char* root)
{
	//打开目录
	DIR* dir = opendir(root);
	//判断目录是否打开成功
	if(dir == NULL)
	{
		perror("opendir");
		exit(1);
	}
	
	//读目录
	//存放读到文件的信息的结构体
	struct dirent* ptr = NULL;
	//总文件数
	int total = 0;
	//存储目录路径
	char path[512] = {0};
	while((ptr = readdir(dir)) != NULL)
	{
		//过滤. 和 .. 两个目录
		if(strcmp(ptr->d_name,".") == 0 || strcmp(ptr->d_name,"..") == 0)
		{
			//不做任何操作
			continue;
		}
		//判断是否是目录
		if(ptr->d_type == DT_DIR)
		{
			//拼接目录路径
			sprintf(path,"%s/%s",root,ptr->d_name);
			//如果是目录,调用递归函数,计算出里面含有多少文件
			total += getFileNum(path);
		}	
		//判断是否是普通文件
		if(ptr->d_type == DT_REG)
		{
			total++;
		}
	}
	//关闭目录
	closedir(dir);
	//返回数量
	return total;
}

int main(int args,char* argv[])
{
	if(args < 2)
	{
		printf("./a.out dirname");
		exit(1);
	}
	int total = getFileNum(argv[1]);
	printf("%s has file number is %d\n",argv[1],total);
	return 0;
}
