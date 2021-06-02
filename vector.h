#ifndef __VECTOR_H_INCLUDED__
#define __VECTOR_H_INCLUDED__

#include "task.h"
#include <vld.h>

typedef task_t _vec_elem_t;

#include <stddef.h>

struct task_t;
typedef struct _tag_vector
{
	int n;
	_vec_elem_t* data;
} vector_t;

void AllocateVector(vector_t* vec);
void DeallocateVector(vector_t* vec);

#endif /* !__VECTOR_H_INCLUDED__ */