#ifndef FINDSTRING
#define FINDSTRING

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

unsigned int findString(char A[], const char *a,unsigned int index,unsigned int *next_index_ret, void (*customFunc)(char,unsigned int))
{
	unsigned int a_size = strlen(a);
	int foundFlg = 0;
	char u = 0;
	char v = 0;
	unsigned long i;
	unsigned long k;

	/*NULLptr check*/
	if (a == NULL || A == NULL) {
		printf("ERROR_FINDSTRING : A nullptr argument is not allowed.\n");
		return 0;
	}

	/*Checking whether the number of characters in the target string is sufficient.*/
	for (unsigned int l = 0;l < a_size;l++)
	{
		if (A[index + l] == 0 || A[index + l] == -1) {
			printf("NOTE_FINDSTRING : The number of characters in the target string is lower than those in the search term.\n");
			return 0;
		}
	}

	/*Search loop*/
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

		if(customFunc != NULL) (*customFunc)(u,i);

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

	if (next_index_ret != NULL && foundFlg) {
		*next_index_ret = i;
	}

	return (foundFlg) ? i : 0;

}


#endif		/*FINDSTRING*/
