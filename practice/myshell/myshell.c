#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXARG 16//最多参数个数
#define MAX 1024//输入最多字节数
#define MAXCMD 8//最多执行的命令

typedef struct cmd
{
	char* args[MAXARG];
	char* in;
	char* out;
}mycmd;

int cmd_format(char* buf,mycmd* cmd)
{
	int cmdnum = 0;	
	char* cur_cmd;
	int j = 0;
	char* temp =  buf;
	//按照'|'分割字符串,返回值是每一条命令
	cmd[cmdnum].args[0] = strtok(temp,"|");
	cmdnum++;
	while(cmd[cmdnum].args[0] = strtok(NULL,"|"))	
	{
		cmdnum++;
	}
	for(int i = 0;i < cmdnum;i++)
	{
		//当前命令
		cur_cmd = cmd[i].args[0];
		cmd[i].in = cmd[i].out = NULL;
		while(*cur_cmd != '\0')
		{
			if(*cur_cmd == ' ')
			{
				*cur_cmd = '\0';
				cur_cmd++;
				continue;
			}
			if(*cur_cmd == '<')
			{
				*cur_cmd = '\0';
				while(*(++cur_cmd) == ' ')
					*cur_cmd = '\0';
				cmd[i].in = cur_cmd;
				continue;
			}
			if(*cur_cmd == '>')
			{
				*cur_cmd = '\0';
				while(*(++cur_cmd) == ' ')
					*cur_cmd = '\0';
				cmd[i].out = cur_cmd;
				continue;
			}
			if(*cur_cmd != ' ' && (cur_cmd == cmd[i].args[0] || *(cur_cmd -1) == '\0'))
			{
				cmd[i].args[j] = cur_cmd;
				j++;
			}
			cur_cmd++;
		}
		cmd[i].args[j] = NULL;
		j = 0;
	}
	return cmdnum;
}

void show_cmd(mycmd *cmd,int cmdnum)
{
	for(int i=0;i<cmdnum;i++)
	{
		int j = 0;
		while(cmd[i].args[j] != NULL)
		{
			printf("%s ",cmd[i].args[j]);
			j++;
		}
		printf(" in : %s ",cmd[i].in);
		printf(" out : %s \n",cmd[i].out);
	}
}

int main(void)
{
	mycmd cmd[MAXCMD];
	char buf[MAX] = {0};
	int cmdnum = 0;
	while(1)
	{
		fgets(buf,sizeof(buf),stdin);
		if(buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = '\0';
		cmdnum = cmd_format(buf,cmd);
		show_cmd(cmd,cmdnum);
	}		
	return 0;
}
