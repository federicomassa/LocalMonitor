#include "MyLogger.h"
#include <iostream>

MyLogger::MyLogger(std::ostream &os) : out(&os)
{
	currentIndentation = 0;
}

MyLogger::EndLine MyLogger::EndL(const Indent &ind)
{
    //std::cout << "Indentation: " << endl.GetIndentation();
    EndLine endl(ind);
    return endl;
}

MyLogger &MyLogger::operator<< (const std::string &obj)
{
    (*out) << obj;
    return (*this);
}

MyLogger &MyLogger::operator<< (const char *obj)
{
    (*out) << obj;
    return (*this);
}

MyLogger &MyLogger::operator<< (const int &obj)
{
    (*out) << obj;
    return (*this);
}

MyLogger &MyLogger::operator<< (const double &obj)
{
    (*out) << obj;
    return (*this);
}

MyLogger &MyLogger::operator<< (const EndLine &obj)
{
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
