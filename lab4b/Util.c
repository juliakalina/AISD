#define _CRT_SECURE_NO_WARNINGS 

#include "Info.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Util.h"

#define LENGTH 128

void Free(void** ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

char* GetStr(char* msg) 
{ 
	return 
		GetFStr(stdin, msg);
}

char* GetFStr(FILE* file, char* msg)
{
	char buf[LENGTH] = "";
	size_t len = 0;
	char* string = (char*)malloc(sizeof(char));

	if (!string)
	{
		printf("There's not enough memory!");
		return NULL;
	}

	*string = '\0';

	if (file == stdin)
		printf("Enter %s:\n> ", msg);
	
	int n = 0;
	do
	{
		n = fscanf_s(file, "%127[^\n]", buf, LENGTH);

		if (n > 0)
		{
			len += strlen(buf);
			char* tmp_ptr = (char*)realloc(string, len + 1);

			if (!tmp_ptr)
			{
				free(string);

				return NULL;
			}

			string = tmp_ptr;

			strcat(string, buf);
		}
		else if (n == 0)
			fscanf_s(file, "%*c");
		else if (feof(file))
			break;
		else
		{
			free(string);

			return NULL;
		}
	} while (n > 0);

	return string;
}

int GetInt(char* msg)
{
	int value;
	do
	{
		printf("Enter number %s:\n> ", msg);

		if (scanf("%d", &value) == 1)
			break;

		int rc = scanf("%*s");
	} while (true);

	int rc = scanf("%*c");

	return value;
}

float GetFloat(char* msg)
{
	float value;
	do
	{
		printf("Enter float number %s:\n> ", msg);

		if (scanf("%f", &value) == 1)
			break;

		int rc = scanf("%*s");
	} while (true);

	int rc = scanf("%*c");

	return value;
}

#undef LENGTH
