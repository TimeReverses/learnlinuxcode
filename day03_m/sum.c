#include <stdio.h>
#include "head.h"
//#define DEBUG

int main(void)
{
	int a = NUM1;
	int b = NUM2;
	int aa;
	int sum = a + b;
#ifdef DEBUG
	printf("The sum value is : %d + %d = %d\n",a,b,sum);
#endif
	//这是注释,预处理过后你就看不见我了	
	return 0;
}
