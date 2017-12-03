#include "Math.h"
#include <sstream>

using namespace std;

double Utility::ToDouble(const string& value)
{
	double d;
	stringstream ss;
	ss << value;
	ss >> d;
	
	return d;
}
