#define _CRT_SECURE_NO_WARNINGS
#include "dialog.h"

#include <stdlib.h>
#include <string.h>

#define BUFFER_LENGTH 128

int dialog(const char* msgs[], int n) {
	char* error = "";
	int choice;
	do {
		fseek(stdin, 0, SEEK_END);
		puts(error);
		error = "You're wrong. Try again!";
		for (int i = 0; i < n; ++i) {
			puts(msgs[i]);
		}
		puts("Make your choice: ");
		choice = getchar() - '0';
		while (getchar() != '\n') {}
	} while (choice < 0 || choice >= n);
	return choice;
}

void _add(key_space_t* ks);
void _find(key_space_t* ks);
void _remove(key_space_t* ks);
void _reorganise(key_space_t* ks);
void _print(key_space_t* ks);


key_t GetKey(void) { return _get_int_param("key"); }

release_t GetRelease(void) { return _get_unsigned_param("release(0 - all, <n> - concrete)"); }

info_t GetInfo(void)
{
	info_t res;
	res.x = _get_float_param("x");
	res.y = _get_float_param("y");
	res.msg = GetStr("msg");

	return res;
}

void _add(key_space_t* ks)
{
	key_t key = GetKey();
	info_t info = GetInfo();
	if (!KSAdd(ks, key, info))
	{
		printf("Failed to add item with " KEY_FMT " key\n", key);

		InfoDelete(&info);
	}
}

void _find(key_space_t* ks)
{
	key_t key = GetKey();
	release_t release = GetRelease();
	key_space_t res = KSFind(ks, key, release);
	
	printf("find result:\n");
	KSPrint(&res);
	KSDelete(&res);
}

void _remove(key_space_t* ks)
{
	key_t key = GetKey();
	release_t release = GetRelease();
	if (!KSRemove(ks, key, release))
		printf("Failed to remove item with " KEY_FMT " key " RELEASE_FMT " release\n", key, release);
}

void _print(key_space_t* ks)
{
	KSPrint(ks);
}


char* GetStr(const char* msg)
{
	char buf[BUFFER_LENGTH] = "";
	int len = 0;

	char* str = (char*)malloc(sizeof(char));
	if (!str)
	{
		printf("failed to allocate memory for str\n");

		return NULL;
	}
	*str = '\0';

	printf("Enter %s:\n> ", msg);
	int n = 0;
	do
	{
		n = scanf("%127[^\n]", buf);
		if (n > 0)
		{
			len += strlen(buf);
			char* tmp_ptr = (char*)realloc(str, len + 1);

			if (!tmp_ptr)
			{
				printf("failed to reallocate memory for str\n");
				free(str);

				return NULL;
			}

			str = tmp_ptr;

			strcat(str, buf);
		}
		else if (n == 0)
			scanf("%*c");
		else
		{
			free(str);

			return NULL;
		}
	} while (n > 0);

	return str;
}

