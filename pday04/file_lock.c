#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*
	文件锁,用于多进程之间
*/
int main(int args,char* argv[])
{
	if(args != 2)
	{
		printf("./a.out filename\n");
		exit(1);
	}
	int fd;
	fd = open(argv[1],O_RDWR);
	if(fd == -1)
	{
		perror("open error");
		exit(1);
	}
	struct flock lock;
	//lock.l_type = F_RDLCK;//读锁
	lock.l_type = F_WRLCK;//写锁
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;//偏移量
	lock.l_len = 0;//0表示对整个文件加锁
	//设置加锁
	fcntl(fd,F_SETLKW,&lock);
	printf("----get flock -- \n");
	sleep(10);
	lock.l_type = F_UNLCK;
	//解锁
	fcntl(fd,F_SETLKW,&lock);
	printf("---- unblock flock -- \n");
	//关闭文件
	close(fd);
	return 0;
}
