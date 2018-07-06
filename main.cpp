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
	simpleCLhandler s = simpleCL_init();
	cl_mem buffer = clCreateBuffer(*(s->mainContext), CL_MEM_READ_WRITE, sizeof(float) * 512, NULL, NULL);
	simpleCL_close(s);
	return print();
}