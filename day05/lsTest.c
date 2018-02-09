#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

int main(int argc,char* argv[])
{
	if(argc < 2)
	{
		printf("./a.out filename");
		exit(1);
	}
	//获取文件名
	char* filename = argv[1];
	//创建一个结构体对象,用于存储值
	struct stat st;
	int ret = stat(filename,&st);
	if(ret == -1)
	{
		perror("stat");
		exit(1);
	}
	//存储文件类型和访问权限
	char perm[11] = {0};
	switch(st.st_mode & S_IFMT)
	{
		case S_IFLNK:
			perm[0] = 'l';
			break;
		case S_IFDIR:
			perm[0] = 'd';
			break;
		case S_IFREG:
			perm[0] = '-';
			break;
		case S_IFBLK:
			perm[0] = 'b';
			break;
		case S_IFCHR:
			perm[0] = 'c';
			break;
		case S_IFSOCK:
			perm[0] = 's';
			break;
		case S_IFIFO:
			perm[0] = 'p';
			break;
		default:
			perm[0] = '?';
			break;
	}
	//文件所有者权限
	perm[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';
	perm[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
	perm[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';
	//文件所属组权限
	perm[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';
	perm[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
	perm[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';
	//其他人权限
	perm[7] = (st.st_mode & S_IROTH) ? 'r' : '-';
	perm[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
	perm[9] = (st.st_mode & S_IXOTH) ? 'x' : '-';
	
	//硬连接数
	int linknum = st.st_nlink;
	//文件大小
	int size = st.st_size;
	//文件所属者
	char* fileUser = getpwuid(st.st_uid)->pw_name;
	//文件所属组
	char* fileGrp = getgrgid(st.st_gid)->gr_name;
	//修改时间
	char* time = ctime(&st.st_mtime);
	printf("%s \n",time);
	char mtime[512] = {0};
	strncpy(mtime,time,strlen(time)-1);
	
	char buf[1024];
	sprintf(buf,"%s  %d  %s  %s  %d  %s  %s",perm,linknum,fileUser,fileGrp,size,mtime,argv[1]);
	printf("%s\n",buf);

	return 0;
}
