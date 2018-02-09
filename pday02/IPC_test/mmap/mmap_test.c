#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

int main(void)
{
	int fd;
	char* p = NULL;//存储映射区的首地址
	//创建一个文件
	fd = open("test.txt", O_RDWR);
	if(fd == -1)	
	{
		perror("file error");
		exit(1);
	}
	//拓展文件空间,文件变成一个空洞文件,存储数据时空洞被覆盖
	/*int r = ftruncate(fd,4);
	if(r == -1)
	{
		perror("ftruncate error");
		exit(1);
	} */
	p = mmap(NULL,20,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if(p == MAP_FAILED)
	{//映射区创建失败
		perror("mmap error");
		exit(1);
	}
	//映射区创建成功后,文件可以立即关闭
	close(fd);
	//创建成功,写数据
	strcpy(p," ");
	//释放映射区
	int ret = munmap(p,20);
	if(ret == -1)
	{
		perror("munmap");
		exit(1);
	}
	//关闭文件
	//close(fd);
	return 0;
}
