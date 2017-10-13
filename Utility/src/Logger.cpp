#include "Logger.h"
#include <iostream>

Logger::Logger(std::ostream &os) : out(&os)
{
}

Logger::EndLine Logger::EndL(const Indent &ind)
{
    //std::cout << "Indentation: " << endl.GetIndentation();
    EndLine endl(ind);
    return endl;
}

Logger &Logger::operator<< (const std::string &obj)
{
    (*out) << obj;
    return (*this);
}

Logger &Logger::operator<< (const char *obj)
{
    (*out) << obj;
    return (*this);
}

Logger &Logger::operator<< (const int &obj)
{
    (*out) << obj;
    return (*this);
}

Logger &Logger::operator<< (const double &obj)
{
    (*out) << obj;
    return (*this);
}

Logger &Logger::operator<< (const EndLine &obj)
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


Logger::EndLine::EndLine(const Logger::Indent &ind)
{
    indentMode = ind;
}
