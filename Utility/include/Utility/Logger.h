#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <vector>
#include <utility>

#define INDENTATION "   "

class Logger
{
    std::ostream *const out;
    int currentIndentation;
public:
    enum Indent {CURR, INC, DEC, NOIND};

    struct EndLine {
        Indent indentMode;
        EndLine ( const Indent & );
    };

    Logger ( std::ostream & );


    EndLine EndL ( const Indent &ind = CURR );

    Logger &operator<< ( const std::string & );
    Logger &operator<< ( const char * );
    Logger &operator<< ( const int & );
    Logger &operator<< ( const double & );
    Logger &operator<< ( const EndLine & );

    template <class T>
    Logger &operator<< ( const std::vector<T> & );

    template <class T, class U>
    Logger &operator<< ( const std::pair<T, U> & );

};


template <class T>
Logger &Logger::operator<< ( const std::vector<T> &v )
{
    ( *this ) << "{ " << EndL();

    for ( int i = 0; i < v.size() - 1; i++ ) {
        ( *this ) << v.at ( i ) << ", " << EndL();
    }

    ( *this ) << v.at ( v.size()-1 );
    ( *this ) << " }";

    return ( *this );
}

template <class T, class U>
Logger &Logger::operator<< ( const std::pair<T, U> &p )
{
    ( *this ) << "( " << p.first << " , " << p.second << " )";

    return ( *this );
}


#endif
