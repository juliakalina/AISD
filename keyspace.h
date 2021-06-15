#ifndef __KEYSPACE2_H_INCLUDED__
#define __KEYSPACE2_H_INCLUDED__

#include "info.h"

#include <stdbool.h>
#include <stdio.h>


typedef int key_t;
typedef unsigned release_t;

#define KEY_FMT "%d"
#define RELEASE_FMT "%u"

extern unsigned MSIZE;

typedef struct node_t
{
	release_t release;
	long info_offset;
	struct _tag_node* next;

} node_t;

typedef struct ks_elem_t
{
	key_t key;
	node_t* node;
	struct ks_elem_t* next;
} ks_elem_t;

typedef struct
{
	ks_elem_t* data;
	FILE* file;
} key_space_t;

void KSDelete(key_space_t* ks);
bool KSAdd(key_space_t* ks, key_t key, const info_t info);
bool KSRemove(key_space_t* ks, key_t key, const release_t release);
void KSReorganise(key_space_t* ks);
key_space_t KSFind(const key_space_t* ks, key_t key, const release_t release);
void KSPrint(const key_space_t* ks);
void KSLoad(key_space_t* ks);
void KSSave(key_space_t* ks);

#endif /* !__KEYSPACE2_H_INCLUDED__ */
