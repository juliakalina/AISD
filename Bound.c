#include "Bound.h"

#include <stdio.h>
#include <stdlib.h>

bool In(Bound bound, Point point)
{
	return point.x >= (bound.x - bound.hw) && point.x <= (bound.x + bound.hw) &&
		point.y >= (bound.y - bound.hh) && point.y <= (bound.y + bound.hh);
}

bool Equals(Point a, Point b)
{
	return a.x == b.x && a.y == b.y;
}