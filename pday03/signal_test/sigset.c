#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void printpend(sigset_t* pend)
{
	int i;
	for(i = 1;i< 32;i++)
	{
		if(sigismember(pend,i) == 1)
		{
			putchar('1');
		}else
		{
			putchar('0');
		}
	}
	printf("\n");
}

int main(void)
{
	//创建一个集合
	sigset_t myset,oldset;
	//将set集合置0
	sigemptyset(&myset);
	//设置3号信号阻塞
	sigaddset(&myset,SIGQUIT);//ctrl + \
	sigaddset(&myset,SIGKILL);//9号
	sigaddset(&myset,SIGINT);//ctrl +c
	sigaddset(&myset,SIGTSTP);//ctrl + z
	//将自定义的集合设置到阻塞信号集中
	sigprocmask(SIG_BLOCK,&myset,&oldset);
	//打印未决信号集
	sigset_t pend;
	while(1){
		sleep(1);
		sigpending(&pend);
	//打印myset未决信号集
	//for(sigset_t::iterator it = myset.begin();it != myset.end();it++)
	//{
	//	printf("%lu\n",*it);
	//}
		printpend(&pend);
	}
	return 0;
}
