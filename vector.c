#include "vector.h"

#include <stdlib.h>
#include <stdio.h>

void AllocateVector(vector_t* vec)
{
	if (!vec->n)
	{
		printf("Wrong length of sequence\n");
		return;
	}
	vec->data = (_vec_elem_t*)calloc(vec->n, sizeof(_vec_elem_t));
	if (!vec->data)
	{
		printf("Cannot allocate memory for the sequence\n");
		return 1;
	}
}

void DeallocateVector(vector_t* vec)
{
	free(vec->data);
	vec->n = 0;
	vec->data = NULL;
}