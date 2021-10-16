#ifndef __BOUND_H_INCLUDED__
#define __BOUND_H_INCLUDED__

#include <stdbool.h>

typedef struct _tag_bound
{
	int x;
	int y;
	int hw;
	int hh;
} Bound;

typedef struct _tag_point
{
	int x;
	int y;
} Point;

bool In(Bound bound, Point point);

bool Equals(Point a, Point b);

#endif /* !__BOUND_H_INCLUDED__ */
