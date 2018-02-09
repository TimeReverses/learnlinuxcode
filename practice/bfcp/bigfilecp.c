#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

void sys_err(char* str)
{
	perror(str);
	exit(1);
}

/*
main 函数的参数argv,第一个元素表示执行程序名
	第二个元素是拷贝的文件名(源)
	第三个元素是拷贝到的文件名(目标)
*/

#define NUM 5
int main(int args,char*argv[])
{
	
	if(args != 3)
	{
		sys_err("./a.out src dst");
	}
	//指定创建进程数和辅助变量
	int n = NUM;//进程数
	int i; //辅助变量
	off_t arr[NUM];
	char* srcp = NULL;//源文件映射区的首地址
	char* dstp = NULL;//目标文件映射区的首地址
	pid_t pid,wpid;
	//打开源文件
	int srcfd = open(argv[1],O_RDWR);
	if(srcfd == -1)
		sys_err("srcfile error");
	//获取源文件的大小
	off_t size = lseek(srcfd,0,SEEK_END);
	//将源文件指针指回头
	lseek(srcfd,0,SEEK_SET);
	//打开目标文件,如果不存在创建一个,存在截断为0
	int dstfd = open(argv[2],O_CREAT|O_RDWR|O_TRUNC,0644);
	if(dstfd == -1)
		sys_err("dstfile error");
	//给目标文件拓展空间
	int ret = ftruncate(dstfd,size);
	if(ret == -1)
		sys_err("ftruncate error");
	//分配每一个进程需要下载的文件的大小
	off_t avgsize = size / n;//平均大小
	off_t leftsize = size - n * avgsize;
	for(int j=0;j<n;j++)
	{
		if(j == n-1)
		{//最后一个进程下载的大小
			arr[j] = avgsize + leftsize;
		}
		else
		{
			arr[j] = avgsize;
		}
	}
	//将源文件和目标文件分别创建一个mmap映射区
	srcp = mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,srcfd,0);
	if(srcp == MAP_FAILED)
		sys_err("src mmap error");
	//映射区创建成功,关闭文件
	ret = close(srcfd);
	if(ret == -1)
		sys_err("src file close");
	dstp = mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,dstfd,0);
	if(dstp == MAP_FAILED)
		sys_err("dst mmap error");
	ret = close(dstfd);
	if(ret == -1)
		sys_err("dst file close");
	//创建子进程
	for(i =0;i<n;i++)
	{
		pid = fork();
		if(pid == -1)
			sys_err("fork error");
		if(pid == 0)
		{
			//子进程
			break;
		}
	}
	if(i == n)
	{
		//父进程,回收子进程
		do
		{
			wpid = waitpid(-1,NULL,WNOHANG);
			if(wpid > 0)
				n--;
		}while(n > 0);
	}
	//临时变量
	char* srct = NULL;
	char* dstt = NULL;
	if(i < n)
	{
		//每次将指针重新指向开头
		srct = srcp;
		dstt = dstp;
		//第i个进程指针偏移量
		off_t py = avgsize * i;
		//偏移指针
		dstt += py;
		srct += py;
		memcpy(dstt,srct,arr[i]);
	}
	//关闭映射区
	ret = munmap(dstp,size);
	if(ret == -1)
		sys_err("dst munmap error");
	ret = munmap(srcp,size);
	if(ret == -1)
		sys_err("src munmap error");	
	return 0;
}
