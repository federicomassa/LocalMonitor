#ifndef UTILITY_MATH_H
#define UTILITY_MATH_H

#include <string>

namespace Utility
{
	double ToDouble(const std::string& value);
	std::string ToString(const double& value, const int& precision = 5);
	double Sign(const double& x);
}

#endif
