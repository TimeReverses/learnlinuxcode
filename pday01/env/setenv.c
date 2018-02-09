#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char* val;
	const char* name = "ABC";
	//首先获取环境变量的值
	val = getenv(name);
	printf("1, %s = %s \n",name,val);
	//设置环境变量的值,1表示如果有同名的name,覆盖其值
	int ret = setenv(name,"hello,world,linux,c++,c,perl",1);
	//再次获取值
	val = getenv(name);
	printf("2, %s = %s \n",name,val);
#if 1
	//删除一个不存在的name,删除一个不存在的name,返回值是0,删除一个"name="这样的值返回值是-1
	ret = unsetenv("hello=");
	printf("ret = %d\n",ret);
	val = getenv(name);
	printf("3, %s = %s \n",name,val);
#else
	ret = unsetenv(name);
	printf("ret = %d \n",ret);
	val = getenv(name);
	printf("3, %s = %s \n",name,val);
#endif
	return 0;
}
