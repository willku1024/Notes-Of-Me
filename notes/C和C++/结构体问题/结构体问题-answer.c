#include <stdio.h>



union MyUnion
{
	struct MyStruct
	{

		unsigned char s1: 1;
		unsigned char s2: 1;
		unsigned char s3: 1;

	}S;

	unsigned char c;
}U;

int main()
{
	U.c = 100;
	printf("%p\n",&U.S);
	printf("S1:%d\n",sizeof(S1));
	printf("U:%d\n",sizeof(U));
	printf("U.S:%d\n",sizeof(U.S));
	printf("U.S.s3:%d\n",U.S.s3);
	//bit-field 位域概念
	//printf("%d\n",sizeof(U.S.s3));


	return 0;
}






















