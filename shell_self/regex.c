#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <regex.h>

int main(int args,char* argv[])
{
	if(args != 3)
	{
		fprintf(stderr,"%s regexstring and TEXT\n",argv[0]);
		return 1;
	}
	
	char* regex_str = argv[1];
	char* text = argv[2];
	
	//声明一个正则表达式结构体
	regex_t oregex;
	int msglen;
	char msg[1024];
	int ret;
	if((ret = regcomp(&oregex,regex_str,REG_EXTENDED|REG_NOSUB)) == 0)
	{
		if((ret = regexec(&oregex,text,0,NULL,0)) == 0)
		{
			printf("%s match %s\n",text,regex_str);
			regfree(&oregex);
			return 0;
		}
	}
	msglen = regerror(ret,&oregex,msg,sizeof(msg));
	msglen = msglen < sizeof(msg) ? msglen : sizeof(msg) - 1;
	msg[msglen] = '\0';
	printf("REG Error with: %s\n",msg);
	regfree(&oregex);
	return 1;
}
