#define _CRT_SECURE_NO_WARNINGS

#include "Util.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 128

char* GetStr(const char* msg)
{
	char buf[LENGTH] = "";
	int len = 0;
	char* string = (char*)malloc(sizeof(char));

	if (!string)
	{
		printf("There's not enough memory!");
		return NULL;
	}

	*string = '\0';

	printf("Enter %s:\n> ", msg);

	int n = 0;
	do
	{
		n = scanf_s("%127[^\n]", buf, LENGTH);

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
			scanf_s("%*c");
		else
		{
			free(string);

			return NULL;
		}
	} while (n > 0);

	return string;
}

int GetInt(const char* msg)
{
	int value;
	do
	{
		printf("Enter number %s:\n> ", msg);

		if (scanf("%d", &value) == 1)
			break;

		int rc = scanf("%*s");
	} while (true);

	return value;
}

int GetUnsigned(char* msg)
{
	int value;
	do
	{
		printf("Enter unsigned number %s:\n> ", msg);

		if (scanf("%d", &value) == 1 && value >= 0)
			break;

		int rc = scanf("%*s");
	} while (true);

	return value;
}

#undef LENGTH