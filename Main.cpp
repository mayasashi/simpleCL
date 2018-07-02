#include <iostream>

//#define SIMPLECL_NOTIFY_WIN
#include "simpleCL.h"

int print()
{
	printf("-----------------------------------------\n");
	printf("-----------------------------------------\n");
	printf("press any key to continue...\n\n");
	char p[10];
	fgets(p, 10, stdin);
	printf("\n-----------------------------------------\n");
	printf("EXIT\n");
	return 0;
}

int main(int argc, const char **argv)
{
	simpleCL_init();
	return print();
}