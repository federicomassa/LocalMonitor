#ifndef MYLOGGER_H
#define MYLOGGER_H

#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <set>
#include <map>
#include "LogFunctions.h"

#include <iostream>

#define INDENTATION "   "

class MyLogger
{
    std::ostream * out;
    int currentIndentation;
public:
    enum Indent {CURR, INC, DEC, NOIND};

    struct EndLine {
        Indent indentMode;
        EndLine ( const Indent & );
    };

    MyLogger();
    MyLogger ( std::ostream & );
    void SetOutput ( std::ofstream* );
	
	// Set numerical precision
	void SetPrecision(const int&);

    EndLine EndL ( const Indent &ind = CURR );

    MyLogger &operator<< ( const std::string & );
    MyLogger &operator<< ( const char * );
    MyLogger &operator<< ( const char & );

    MyLogger &operator<< ( const int & );
    MyLogger &operator<< ( const long unsigned int & );

    MyLogger &operator<< ( const double & );
	MyLogger &operator<< ( const bool & );
    MyLogger &operator<< ( const EndLine & );

    template <class T>
    MyLogger &operator<< ( const std::vector<T> & );

    template <class Key, class T>
    MyLogger &operator<< ( const std::map<Key, T> & );


    template <class T, class U>
    MyLogger &operator<< ( const std::pair<T, U> & );

    template <class T>
    MyLogger &operator<< ( const std::set<T> & );
};


template <class T>
MyLogger &MyLogger::operator<< ( const std::vector<T> &v )
{
    LogFunctions::Require ( out != nullptr, "MyLogger::operator<<", "You must set output first." );

    ( *this ) << "{ " << EndL();

    for ( int i = 0; i < v.size() - 1; i++ ) {
        ( *this ) << v.at ( i ) << ", " << EndL();
    }

    ( *this ) << v.at ( v.size()-1 );
    ( *this ) << " }";

    return ( *this );
}

template <class Key, class T>
MyLogger &MyLogger::operator<< ( const std::map<Key, T> & m )
{
    LogFunctions::Require ( out != nullptr, "MyLogger::operator<<", "You must set output first." );

    ( *this ) << "{ " << EndL();

    for ( typename std::map<Key, T>::const_iterator itr = m.begin(); itr != m.end(); itr++ ) {
        ( *this ) << ( *itr ) << EndL();
    }

    ( *this ) << " }" << EndL();

    return ( *this );
}

template <class T, class U>
MyLogger &MyLogger::operator<< ( const std::pair<T, U> &p )
{
    LogFunctions::Require ( out != nullptr, "MyLogger::operator<<", "You must set output first." );

    ( *this ) << "( " << p.first << " , " << p.second << " )";

    return ( *this );
}

template <class T>
MyLogger &MyLogger::operator<< ( const std::set<T> &s )
{
    LogFunctions::Require ( out != nullptr, "MyLogger::operator<<", "You must set output first." );

    ( *this ) << "{ " << EndL();

    for ( typename std::set<T>::const_iterator itr = s.begin(); itr != --s.end(); ++itr ) {
        ( *this ) << *itr << ", " << EndL();
    }

    ( *this ) << * ( --s.end() );
    ( *this ) << " }";

    return ( *this );
}


#endif
