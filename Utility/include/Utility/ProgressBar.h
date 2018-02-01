#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <cmath>
#include <iostream>

namespace LM{
	class ProgressBar;
}
std::ostream& operator<< ( std::ostream& os, const LM::ProgressBar& p );

namespace LM
{

class ProgressBar
{
    static int rollIndex;
    bool backspaces;
    int current;
    int total;
    friend std::ostream& ::operator<< ( std::ostream& os, const LM::ProgressBar& p );
public:
    ProgressBar();
    ProgressBar ( const int& c, const int& t );
};

}


#endif
