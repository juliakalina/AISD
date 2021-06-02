#include "queue.h"
#include "vector.h"

#include <stdio.h>
#include <vld.h>
void AllocateQueue(queue_t* queue)
{
#ifdef _VECTOR_IMPL
	queue->first = 0;
	queue->last = 0;

	queue->cont.n = QUEUE_SIZE;
	AllocateVector(&queue->cont);

#else /* _LIST_IMPL */

	queue->cont.next = NULL;

#endif
}

void DeallocateQueue(queue_t* queue)
{
#ifdef _VECTOR_IMPL

	DeallocateVector(&queue->cont);
	queue->first = 0;
	queue->last = 0;

#else /* _LIST_IMPL */

	DeallocateList(queue->cont.next);

#endif
}

void PushQueue(queue_t* queue, task_t task)
{
#ifdef _VECTOR_IMPL

	if ((queue->last + 1) % QUEUE_SIZE == queue->first) 
	{
		printf("Queue is full\n");
	}

	queue->cont.data[queue->last] = task;
	queue->last = ++queue->last % QUEUE_SIZE;

#else /* _LIST_IMPL */

	node_t* ptr = &queue->cont;
	while (ptr->next)
		ptr = ptr->next;

	InsertNode(ptr, task);

#endif
}

task_t PopQueue(queue_t* queue)
{
#ifdef _VECTOR_IMPL

	if (queue->first != queue->last)
	{
		task_t res = queue->cont.data[queue->first];
		queue->first = ++queue->first % QUEUE_SIZE;

		return res;
	}

#else /* _LIST_IMPL */

	node_t* ptr = queue->cont.next;
	if (ptr)
	{
		task_t res = ptr->value;
		RemoveNode(&queue->cont, ptr);

		return res;
	}

#endif

	printf("Queue is empty\n");
	task_t res = { 0, 0, 0 };
	return res;
}
