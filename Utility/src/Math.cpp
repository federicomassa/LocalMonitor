#include "Math.h"
#include <sstream>
#include <math.h>

using namespace std;

double Utility::ToDouble(const string& value)
{
	double d;
	stringstream ss;
	ss << value;
	ss >> d;
	
	return d;
}

string Utility::ToString(const double& value, const int& precision)
{
  /* copy value */
  double x = fabs(value);

  stringstream ss;
  string str;

  if (precision && x > 1E-9)
  {
    double logx = floor(log10(x));

    x = x / pow(10, logx + 1 - precision);
    x = round(x);
    x = x * pow(10, logx + 1 - precision);

    ss << x*value / fabs(value);
  }
  else
    ss << value;

  ss >> str;
  return str;	
}

double Utility::Sign(const double& x)
{
	if (x == 0)
		return 0;
	
	return x/fabs(x);
}
