#ifndef __UTIL_H_INCLUDED__
#define __UTIL_H_INCLUDED__

#define _USE_MATH_DEFINES
#include <math.h>


double CalcSin(int n, int angle, double eps);

double GetDouble(char* msg);
int GetInt(char* msg);

#endif /* !__UTIL_H_INCLUDED__ */
