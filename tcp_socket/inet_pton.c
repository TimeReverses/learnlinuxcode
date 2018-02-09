#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

int main(int args,char* argv[])
{
	//unsigned char buf[sizeof(struct in_addr)];
	struct in_addr addr;
	int domain,s;
	char str[INET_ADDRSTRLEN];

	if(args != 3)
	{
		printf("> %s {i4/i6/other} string\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	domain = (strcmp(argv[1],"i4") == 0)?AF_INET:
		(strcmp(argv[1],"i6") == 0)?AF_INET6:atoi(argv[1]);
	//ip address converts inet
	s = inet_pton(domain,argv[2],(void*)&addr);
	if(s <= 0)
	{
		if(s == 0)
		{
			fprintf(stderr,"Not in presentation format");
			exit(EXIT_FAILURE);
		}
		else
		{
			perror("inet_pton");
			exit(EXIT_FAILURE);
		}
	}	
	printf("%d\n",addr.s_addr);
	//inet converts ip address
	if(inet_ntop(domain,(void*)&addr,str,INET_ADDRSTRLEN) == NULL)
	{
		perror("inet_ntop");
		exit(EXIT_FAILURE);
	}
	printf("%s\n",str);
	exit(EXIT_SUCCESS);
}
