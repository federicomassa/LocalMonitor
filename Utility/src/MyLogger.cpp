#include "MyLogger.h"
#include "LogFunctions.h"
#include <iomanip>
#include <iostream>

using namespace LogFunctions;

MyLogger::MyLogger(std::ostream &os) : out(&os)
{
	currentIndentation = 0;
}

MyLogger::MyLogger() : out(nullptr)
{
	currentIndentation = 0;
}

void MyLogger::SetOutput(std::ofstream* o)
{
	out = o;
}

void MyLogger::SetPrecision(const int& p)
{
	(*out) << std::fixed << std::setprecision(p);
}


MyLogger::EndLine MyLogger::EndL(const Indent &ind)
{
    //std::cout << "Indentation: " << endl.GetIndentation();
    EndLine endl(ind);
    return endl;
}

MyLogger &MyLogger::operator<< (const std::string &obj)
{
	Require(out != nullptr, "MyLogger::operator<<", "You must set output first.");
	
    (*out) << obj;
    return (*this);
}

MyLogger &MyLogger::operator<< (const char *obj)
{
	Require(out != nullptr, "MyLogger::operator<<", "You must set output first.");
	
    (*out) << obj;
    return (*this);
}

MyLogger &MyLogger::operator<< (const char &obj)
{
	Require(out != nullptr, "MyLogger::operator<<", "You must set output first.");
	
    (*out) << obj;
    return (*this);
}

MyLogger &MyLogger::operator<< (const int &obj)
{
	Require(out != nullptr, "MyLogger::operator<<", "You must set output first.");
	
    (*out) << obj;
    return (*this);
}

MyLogger &MyLogger::operator<< (const long unsigned int &obj)
{
	Require(out != nullptr, "MyLogger::operator<<", "You must set output first.");
	
    (*out) << obj;
    return (*this);
}


MyLogger &MyLogger::operator<< (const double &obj)
{
	Require(out != nullptr, "MyLogger::operator<<", "You must set output first.");
	
    (*out) << obj;
    return (*this);
}

MyLogger &MyLogger::operator<< (const bool &obj)
{
	Require(out != nullptr, "MyLogger::operator<<", "You must set output first.");
	
	if (obj)
		(*out) << 1;
	else
		(*out) << 0;
	
    return (*this);
}

MyLogger &MyLogger::operator<< (const EndLine &obj)
{
	Require(out != nullptr, "MyLogger::operator<<", "You must set output first.");
	
    (*out) << '\n';

    if (obj.indentMode != NOIND) {
        if (obj.indentMode == INC) {
            currentIndentation++;
        } else if (obj.indentMode == DEC) {
            currentIndentation--;
        }

        if (currentIndentation < 0) {
            currentIndentation = 0;
        }

        for (int i = 0; i < currentIndentation; i++) {
            (*out) << INDENTATION;
        }

    }

    (*out).flush();

    return (*this);
}


MyLogger::EndLine::EndLine(const MyLogger::Indent &ind)
{
    indentMode = ind;
}
