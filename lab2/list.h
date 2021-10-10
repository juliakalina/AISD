#ifndef __LINKEDLIST_H_INCLUDED__
#define __LINKEDLIST_H_INCLUDED__

#include "task.h"
typedef task_t _node_elem_t;

typedef struct _tag_node
{
	_node_elem_t value;
	struct _tag_node* next;
} node_t;

void DeallocateList(node_t* head);
node_t* AddNextNode(node_t* cur);
void InsertNode(node_t* cur, _node_elem_t val);
void RemoveNode(node_t* prev, node_t* cur);

#endif /* __LINKEDLIST_H_INCLUDED__ */
