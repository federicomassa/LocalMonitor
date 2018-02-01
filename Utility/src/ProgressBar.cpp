
#include "ProgressBar.h"

using namespace std;

int LM::ProgressBar::rollIndex = 0;

LM::ProgressBar::ProgressBar()
{
    backspaces = false;
    current = 0;
    total = 1;
}

LM::ProgressBar::ProgressBar(const int& c, const int& t)
{
    rollIndex = (rollIndex + 1) % 4;
    backspaces = true;
    current = c;
    total = t;
}

ostream& operator<<(ostream& os, const LM::ProgressBar& p)
{
    char roll = ' ';
    switch(p.rollIndex)
    {
        case 0:
            roll = '|';
            break;
        case 1:
            roll = '/';
            break;
        case 2:
            roll = '-';
            break;
        case 3:
            roll = '\\';
            break;
    }

    const int width = 69;

    if(p.backspaces)
        for(int n = 0; n < width + 9; n++)
            os << '\b';

    cout << roll << ' ' << '[';
    const int prog = (int)round((double)p.current / (double)p.total * width);
    const int perc = (int)round((double)p.current / (double)p.total * 100);
    for(int n = 0; n < prog - 1; n++)
        os << '=';
    if(perc == 100)
        os << '=';
    else if(prog > 0)
        os << '>';
    for(int n = prog; n < width; n++)
        os << ' ';
    os << "] ";
    os.fill(' ');
    os.width(3);
    os << perc << '%';
    os << flush;

    return os;
}
