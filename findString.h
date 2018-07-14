#ifndef FINDSTRING
#define FINDSTRING

#include <stdio.h>
#include <string.h>

int findString(char A[], const char *a,unsigned int index)
{
	unsigned int a_size = strlen(a);
	int foundFlg = 0;
	char u = 0;
	char v = 0;
	unsigned long i;
	unsigned long k;

	for (
		i = 0;
			!foundFlg &&
			A[index + i + a_size - 1] != 0 && 
			A[index + i + a_size - 1] != -1;
		i++
		)
	{
		u = A[index + i];
		v = A[index + i + a_size - 1];
		k = 0;

		printf("String : ");
		for (unsigned long l = 0; l < a_size; l++)
		{
			printf("%c", A[index + i + l]);
		}
		printf("\n");

		while (!foundFlg && u == a[k] && v == a[a_size - 1 - k])
		{
			k++;
			u = A[index + i + k];
			v = A[index + i + a_size - 1 - k];
			
			printf("	caught (k = %lu) : ", k);
			for (int l = 0; 2*k < a_size - l; l++)
			{
				printf("%c", A[index + i + k + l]);
			}
			printf("\n");
			if (2 * k > a_size - 1) foundFlg = 1;
		}
	}

	return (foundFlg) ? i - 1 : -1;

}


#endif		/*FINDSTRING*/
