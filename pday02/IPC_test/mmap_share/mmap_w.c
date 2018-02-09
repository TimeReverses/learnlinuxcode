#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

void sys_err(char *str)
{
	perror(str);
	exit(1);
}

typedef struct STU
{
	int age;
	char name[20];
	char sex;
}STU;
int main(int args,char* argv[])
{
	if(args < 2)
		sys_err("./a.out filename");
	int fd = open(argv[1],O_RDWR|O_CREAT|O_TRUNC,0644);
	if(fd == -1)
		sys_err("file open");
	STU student = {11,"桃乃木香奈",'f'};
	STU student2 = {12,"波多野吉衣",'f'};
	STU* mm;
	ftruncate(fd,sizeof(student) * 2);
	//创建一个映射区
	mm = mmap(NULL,sizeof(student),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);//只读
	if(mm == MAP_FAILED)
		sys_err("mmap error");
	close(fd);
	STU *temp = mm;
	/*
	while(1)
	{
		sleep(1);//每隔2秒写数据一次
		memcpy(mm,&student,sizeof(student));
		student.age++;//每次年龄加1
	}
	*/
	for(int i=0;i<2;i++)
	{
		sleep(1);	
		if(i == 0){
			printf("temp = %p\n",temp);
			memcpy(temp,&student,sizeof(student));
		}
		else
		{
			temp ++;
			printf("temp = %p\n",temp);
			memcpy(temp,&student2,sizeof(student2));
		}
	}
	//关闭映射区
	int ret = munmap(mm,sizeof(student));
	if(ret == -1)	
		sys_err("munmap");
	return 0;
}
