#include "Interval.h"
#include "LogFunctions.h"

Interval::Interval()
{
	min = 0;
	max = 0;
}

Interval::Interval(const double& m, const double& M) 
{
	if (m > M)
		LogFunctions::Error("Interval::Interval", "Minimum should be smaller than maximum");
	
	min = m;
	max = M;
}

Interval Interval::operator*(const Interval& i) const
{
	Interval intersection;
	
	// No intersection case
	if (min >= i.max || max <= i.min)
		return intersection;
		
	if (min <= i.min)
		intersection.min = i.min;
	else
		intersection.min = min;
	
	if (max >= i.max)
		intersection.max = i.max;
	else
		intersection.max = max;
	
	return intersection;
}

bool Interval::IsEmpty() const
{
	if (min == max)
		return true;
	
	return false;
}

bool Interval::HasIntersection(const Interval& i) const
{
	Interval intersection;
	
	// No intersection case
	if (min > i.max || max < i.min)
		return false;
		
	return true;
}

bool Interval::HasNonEmptyIntersection(const Interval& i) const
{
	Interval intersection;
	
	// No intersection case
	if (min >= i.max || max <= i.min)
		return false;
		
	return true;
}
