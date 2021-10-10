#include "util.h"

#include <stdio.h>

double CalcSin(int n, int angle, double eps)
{
	double x = angle*M_PI / 180.0;
	double t = x;
	double res = x;
	for (int i = 1; i <= n; ++i)
	{
		t = (t * (-1) * x * x) / (2 * i * (2 * i + 1));
		res += t;
	}
	return res;
}

int GetInt(char* msg)
{
	int res = 0;
	int ret_val = 0;
	while (!ret_val)
	{
		printf("Enter %s: ", msg);
		ret_val = scanf_s("%d", &res);
		if (!ret_val || res < 0)
		{
			printf("Waiting for int\n");
			scanf_s("%*[^\n]");
		}
	}
	scanf_s("%*c");
	return res;
}

double GetDouble(char* msg)
{
	double res = 0;
	int ret_val = 0;
	while (!ret_val)
	{
		printf("Enter %s: ", msg);
		ret_val = scanf_s("%lf", &res);
		if (!ret_val)
		{
			printf("Waiting for double\n");
			scanf_s("%*[^\n]");
		}
	}

	scanf_s("%*c");
	return res;
}
