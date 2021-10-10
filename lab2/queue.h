#ifndef __QUEUE_H_INCLUDED__
#define __QUEUE_H_INCLUDED__

#include <stdbool.h>
#include <stddef.h>

#ifdef _VECTOR_IMPL

#include "vector.h"

static const int QUEUE_SIZE = 10;

#else 

#include "list.h"

#endif

typedef struct _tag_queue
{
#ifdef _VECTOR_IMPL

	int first;
	int last;
	vector_t cont;

#else /* _LIST_IMPL */

	node_t cont;

#endif
} queue_t;

void AllocateQueue(queue_t* queue);
void DeallocateQueue(queue_t* queue);
void PushQueue(queue_t* queue, task_t task);
task_t PopQueue(queue_t* queue);

inline bool IsEmptyQueue(queue_t queue)
{
#ifdef _VECTOR_IMPL

	return queue.first == queue.last;

#else /* _LIST_IMPL */

	return queue.cont.next == NULL;

#endif
}

#endif /* !__QUEUE_H_INCLUDED__ */
