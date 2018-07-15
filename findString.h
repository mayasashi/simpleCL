#ifndef FINDSTRING
#define FINDSTRING

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

unsigned int findString
(
	const char A[], 
	const char *a, 
	unsigned int index, 
	unsigned int *next_index_ret, 
	void(*customFunc)(char, char, unsigned int)
)
{
	unsigned int a_size = strlen(a);
	int foundFlg = 0;
	char u = 0;
	char v = 0;
	char w = 0;
	unsigned long i;
	unsigned long k;

	/*NULLptr check*/
	if (a == NULL || A == NULL) {
		printf("ERROR_FINDSTRING : A nullptr argument is not allowed.\n");
		return 0;
	}

	/*Checking whether the number of characters in the target string is sufficient.*/
	for (unsigned int l = 0; l < a_size; l++)
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

		if (index + i != 0) w = A[index + i - 1];
		k = 0;

		if (customFunc != NULL) (*customFunc)(u, w, i);     /*Custom function call*/

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
			for (int l = 0; 2 * k < a_size - l; l++)
			{
				printf("%c", A[index + i + k + l]);
			}
			printf("\n");
			if (2 * k > a_size - 1) foundFlg = 1;
		}
	}

	if (next_index_ret != NULL && foundFlg) {
		*next_index_ret = index + i;
	}

	return (foundFlg) ? index + i : 0;

}

unsigned int findTwoStringsWithSpace
(
	const char A[], 
	const char *a1, 
	const char *a2, 
	unsigned int index, 
	unsigned int *next_index_ret, 
	void (*customFunc)(char,char,unsigned int)
)
{
	
	unsigned int a1_index = 0, a2_index = index;
	unsigned int a1_size = strlen(a1);
	unsigned int a2_size = strlen(a2);
	unsigned int post_a1_index;
	unsigned int pre_a2_index;
	bool foundFlg = false;
	bool onlySpaceHTflg = true;
	while (
		!foundFlg &&
		findString(A, a1, a2_index, &a1_index, customFunc) != 0 &&      /*Search a1*/
		findString(A, a2, a1_index, &a2_index, customFunc) != 0         /*Search a2*/
		)
	{
		/*The characters between a1 and a2 should be composed of spaces or HTs.*/

		post_a1_index = a1_index + a1_size - 1;
		pre_a2_index  = a2_index - 2;
		onlySpaceHTflg = true;

		for (unsigned int l = post_a1_index; l <= pre_a2_index && onlySpaceHTflg; l++)
		{
			onlySpaceHTflg *= (A[l] == ' ' || A[l] == '\t');
		}
		
		if (onlySpaceHTflg) {
			printf("NOTE_FINDTWOSTRINGSWITHSPACE : found\n");
			foundFlg == true;
		}
	}
	
	if (foundFlg && next_index_ret != NULL) *next_index_ret = a1_index;

	return (foundFlg) ? a1_index : 0;

}


#endif		/*FINDSTRING*/
