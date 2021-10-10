#include "list.h"

#include <stdio.h>
#include <stdlib.h>

void DeallocateList(node_t* head)
{
	if (!head)
		return;
	node_t* ptr = head->next;
	while (ptr)
	{
		node_t* tmp = ptr->next;
		free(ptr);
		ptr = tmp;
	}
	free(head);
	head = NULL;
}

node_t* AddNextNode(node_t* cur)//äîáàâëåíèå ñëåä íîäà
{
	cur->next = (node_t*)calloc(1, sizeof(node_t));
	if (!cur->next)
	return NULL;
	cur->next->next = NULL;
	return cur->next;
}

void InsertNode(node_t* cur, _node_elem_t val)//âñòàâêà â íîä
{
	node_t* next = cur->next;
	node_t* ptr = AddNextNode(cur);
	ptr->value = val;
	ptr->next = next;
}

void RemoveNode(node_t* prev, node_t* cur)
{
	node_t* ptr = cur->next;
	free(prev->next);
	prev->next = ptr;
}
