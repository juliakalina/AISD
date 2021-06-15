#define _CRT_SECURE_NO_WARNINGS
#include "keyspace.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MIN_INFO_OFFSET

ks_elem_t* _create_elem(const key_t key)
{
	ks_elem_t* elem = (ks_elem_t*)calloc(1LLU, sizeof(ks_elem_t));
	if (!elem)
		return elem;

	elem->key = key;

	return elem;
}

node_t* _create_node(const long info_offset, const release_t release)
{
	node_t* node = (node_t*)calloc(1LLU, sizeof(node_t));
	if (!node)
		return NULL;
	
	node->release = release;
	node->info_offset = info_offset;

	return node;
}

node_t* _remove_node(node_t* node)
{
	if (!node)
		return NULL;

	node_t* next = node->next;
	free(node);

	return next;
}

void _remove_chain(ks_elem_t* elem)
{
	if (!elem)
		return;

	node_t* ptr = elem->node;
	while (ptr)
		ptr = _remove_node(ptr);

	elem->node = NULL;
}

info_t _load_info(FILE* file, const long offset)
{
	info_t res = { 0.0, 0.0, NULL };

	if (!file)
		return res;

	assert(!fseek(file, offset, SEEK_SET));
	assert(1LLU == fread(&res.x, sizeof(float), 1LLU, file));
	assert(1LLU == fread(&res.y, sizeof(float), 1LLU, file));

	int length = 0;
	assert(1LLU == fread(&length, sizeof(size_t), 1LLU, file));

	res.msg = (char*)calloc(length + 1, sizeof(char));
	if (!res.msg)
		return res;

	assert(length == fread(res.msg, sizeof(char), length, file));

	return res;
}

long _save_info(FILE* file, info_t info)
{
	fseek(file, 0L, SEEK_END);
	long offset = ftell(file);

	assert(1LLU == fwrite(&info.x, sizeof(float), 1LLU, file));
	assert(1LLU == fwrite(&info.y, sizeof(float), 1LLU, file));

	int length = strlen(info.msg);
	assert(1LLU == fwrite(&length, sizeof(size_t), 1LLU, file));
	assert(length == fwrite(info.msg, sizeof(char), length, file));

	fflush(file);

	InfoDelete(&info);

	return offset;
}

bool _ks_add(key_space_t* ks, key_t key, const info_t info, const release_t release)
{
	if (!ks)
		return false;

	key %= MSIZE;

	int count = 0U;
	ks_elem_t* ptr = ks->data;
	while (ptr)
	{
		if (ptr->key == key)
		{
			release_t rel = !release ? 1 : release;
			node_t* node_ptr = ptr->node;
			while (node_ptr && node_ptr->next)
			{
				if (!release)
					rel++;
				
				node_ptr = node_ptr->next;
			}

			if (!node_ptr)
				return (ptr->node = _create_node(_save_info(ks->file, info), rel));
			else
				return (node_ptr->next = _create_node(_save_info(ks->file, info), rel + (!release ? 1 : 0)));
		}

		if (++count == MSIZE)
			return false;

		if (!ptr->next)
			break;

		ptr = ptr->next;
	}

	if (!ptr)
		return ((ks->data = _create_elem(key)) && (ks->data->node = _create_node(_save_info(ks->file, info), 1)));

	return ((ptr->next = _create_elem(key)) && (ptr->next->node = _create_node(_save_info(ks->file, info), 1)));
}

void KSDelete(key_space_t* ks)
{
	if (!ks || !ks->data)
		return;

	ks_elem_t* ptr = ks->data;
	while (ptr)
	{
		_remove_chain(ptr);

		ks_elem_t* next = ptr->next;
		free(ptr);

		ptr = next;
	}

	ks->data = NULL;
	ks->file = NULL;
}

bool KSAdd(key_space_t* ks, key_t key, const info_t info)
{
	return _ks_add(ks, key, info, 0);
}

bool KSRemove(key_space_t* ks, key_t key, const release_t release)
{
	if (!ks)
		return false;

	key %= MSIZE;

	ks_elem_t* prev = NULL;
	ks_elem_t* ptr = ks->data;
	while (ptr)
	{
		if (ptr->key == key)
			if (!release)
			{
				_remove_chain(ptr);
				if (prev)
					prev->next = ptr->next;
				else
					ks->data = ptr->next;

				free(ptr);

				return true;
			}
			else
			{
				node_t* node_prev = NULL;
				node_t* node_ptr = ptr->node;
				while (node_ptr)
				{
					if (node_ptr->release == release)
					{
						if (node_prev)
							node_prev->next = _remove_node(node_ptr);
						else
							ptr->node = _remove_node(node_ptr);

						return true;
					}

					node_prev = node_ptr;
					node_ptr = node_ptr->next;
				}
			}

		prev = ptr;
		ptr = ptr->next;
	}

	return false;
}

void KSReorganise(key_space_t* ks)
{
	if (!ks)
		return;

	ks_elem_t* ptr = ks->data;
	while (ptr)
	{
		while (ptr->node && ptr->node->next)
			ptr->node = _remove_node(ptr->node);

		ptr = ptr->next;
	}
}

key_space_t KSFind(const key_space_t* ks, key_t key, const release_t release)
{
	key_space_t res = { NULL, ks->file };
	if (!ks)
		return res;

	key %= MSIZE;

	ks_elem_t* ptr = ks->data;
	while (ptr)
	{
		if (ptr->key == key)
		{
			node_t* node_ptr = ptr->node;
			while (node_ptr)
			{
				if (!release || node_ptr->release == release)
					_ks_add(&res, key, _load_info(ks->file, node_ptr->info_offset), node_ptr->release);

				node_ptr = node_ptr->next;
			}
		}
		
		ptr = ptr->next;
	}

	return res;
}

void KSPrint(const key_space_t* ks)
{
	if (!ks)
		return;

	ks_elem_t* ptr = ks->data;
	if (!ptr)
	{
		printf("empty\n");

		return;
	}

	while (ptr)
	{
		printf("key: " KEY_FMT "\n", ptr->key);
		node_t* node_ptr = ptr->node;
		while (node_ptr)
		{
			printf("\trel: " RELEASE_FMT " ", node_ptr->release);

			info_t info = _load_info(ks->file, node_ptr->info_offset);
			InfoPrint(&info);
			InfoDelete(&info);

			node_ptr = node_ptr->next;
		}

		printf("\n");

		ptr = ptr->next;
	}
}

void KSLoad(key_space_t* ks)
{
	if (!ks || !ks->file)
		return;

	fseek(ks->file, 0L, SEEK_SET);

	int elements_num = 0U;
	fread(&elements_num, sizeof(int), 1LLU, ks->file);
	for (int i = 0; i < elements_num; ++i)
	{
		key_t key;
		fread(&key, sizeof(key_t), 1LLU, ks->file);

		int nodes_num = 0U;
		fread(&nodes_num, sizeof(int), 1LLU, ks->file);
		for (int j = 0; j < nodes_num; ++j)
		{
			long info_offset = 0L;
			fread(&info_offset, sizeof(long), 1LLU, ks->file);

			long file_offset = ftell(ks->file);
			KSAdd(ks, key, _load_info(ks->file, info_offset));
			fseek(ks->file, file_offset, SEEK_SET);
		}
	}
}

void KSSave(key_space_t* ks)
{
	if (!ks || !ks->file)
		return;

	fseek(ks->file, 0L, SEEK_SET);
	int elements_num = 0U;
	ks_elem_t* elem = ks->data;
	while (elem)
	{
		++elements_num;
		elem = elem->next;
	}
	fwrite(&elements_num, sizeof(int), 1LLU, ks->file);

	elem = ks->data;
	while (elem)
	{
		fwrite(&elem->key, sizeof(key_t), 1LLU, ks->file);

		int nodes_num = 0;
		node_t* node_ptr = elem->node;
		while (node_ptr)
		{
			++nodes_num;
			node_ptr = node_ptr->next;
		}
		fwrite(&nodes_num, sizeof(int), 1LLU, ks->file);

		node_ptr = elem->node;
		while (node_ptr)
		{
			fwrite(&node_ptr->info_offset, sizeof(long), 1LLU, ks->file);
			node_ptr = node_ptr->next;
		}

		elem = elem->next;
	}
}