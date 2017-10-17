#include "StateCell.h"
#include "Utility/LogFunctions.h"
#include <string>

using namespace LogFunctions;
using namespace std;

StateCell::StateCell(const State &min, const State &max)
{
    CheckConsistency("State::State", min, max);

    minState = min;
    maxState = max;
}

const State &StateCell::GetMin() const
{
    return minState;
}
const State &StateCell::GetMax() const
{
    return maxState;
}

bool StateCell::Contains(const State &q) const
{
    CheckConsistency("StateCell::Contains", minState, q);

    for (int i = 0; i < q.Size(); i++) {
        if (minState > q) {
            return false;
        }

        if (maxState < q) {
            return false;
        }
    }

    return true;
}

bool StateCell::Contains(const StateCell &cell) const
{
    CheckConsistency("StateCell::Contains", minState, cell.minState);

    if (minState > cell.minState) {
        return false;
    }

    if (maxState < cell.maxState) {
        return false;
    }

    return true;
}
