#include "LogFunctions.h"

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <math.h>

using namespace std;

string LogFunctions::ClassInfo(const string &className)
{
    return ("In " + className + ": ");
}

void LogFunctions::Error(const string className, const string msg)
{
    cerr << endl << "ERROR - " << className << " : " << msg << endl;
    exit(1);
}

void LogFunctions::Require(const bool &condition, const string &className, const string &msg)
{
    if (!condition) {
        Error(className, msg);
    }
}

string LogFunctions::ToStringWithPrecision(const double &i, const int &precision)
{
    /* copy value */
    double x = fabs(i);

    stringstream ss;
    string str;

    if (precision && x > 1E-9) {
        double logx = floor(log10(x));

        x = x / pow(10, logx + 1 - precision);
        x = round(x);
        x = x * pow(10, logx + 1 - precision);

        ss << x *i / fabs(i);
    } else {
        ss << i;
    }

    ss >> str;
    return str;
}
