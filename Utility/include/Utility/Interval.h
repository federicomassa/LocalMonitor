#ifndef INTERVAL_H
#define INTERVAL_H

struct Interval
{
	double min;
	double max;
	Interval();
	Interval(const double&, const double&);
	Interval operator*(const Interval&) const;
	bool HasIntersection(const Interval&) const;
	bool HasNonEmptyIntersection(const Interval&) const;
	bool IsEmpty() const;
};

#endif
