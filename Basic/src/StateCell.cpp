#include "StateCell.h"
#include "Utility/LogFunctions.h"
#include "Utility/Logger.h"
#include "Utility/Interval.h"
#include <string>

using namespace LogFunctions;
using namespace std;

StateCell::StateCell() : minState(), maxState()
{
	empty = true;
}

StateCell::StateCell(const State &min, const State &max)
{
    CheckConsistency("State::State", min, max);
	
	empty = true;
	for (StateMap::const_iterator minItr = min.begin(); minItr != min.end(); minItr++)
		{
			StateMap::const_iterator maxItr = max.find(minItr->first);
			if (minItr->second != maxItr->second)
				empty = false;
			
			if (minItr->second > maxItr->second)
				Error("StateCell::StateCell", "Min state must be smaller than max state");
		}
	
    minState = min;
    maxState = max;
	
}

const bool& StateCell::IsEmpty() const
{
	return empty;
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

	// The cell does not contain the point if minimum state is larger than the point or
	// maximum state is smaller
    for (int i = 0; i < q.Size(); i++) 
	{
		for (StateMap::const_iterator itr = minState.begin(); itr != minState.end(); itr++)
		{
			StateMap::const_iterator itr2 = q.find(itr->first);
			if (*itr > *itr2)
				return false;
		}
		
		for (StateMap::const_iterator itr = maxState.begin(); itr != maxState.end(); itr++)
		{
			StateMap::const_iterator itr2 = q.find(itr->first);
			if (*itr < *itr2)
				return false;
		}
		
	}

    return true;
}

bool StateCell::Contains(const StateCell &cell) const
{
	return (Contains(cell.GetMin()) && Contains(cell.GetMax()));
}

StateCell StateCell::operator*(const StateCell & cell) const
{
	CheckConsistency("StateCell::operator*", minState, cell.minState);
	
	State minIntersection;
	State maxIntersection;
	
	for (StateMap::const_iterator minItr1 = minState.begin(); minItr1 != minState.end(); minItr1++) 
		 {
			 StateMap::const_iterator maxItr1 = maxState.find(minItr1->first);
			 StateMap::const_iterator minItr2 = cell.minState.find(minItr1->first);
			 StateMap::const_iterator maxItr2 = cell.maxState.find(minItr1->first);
			 
			 Interval i1(minItr1->second, maxItr1->second);
			 Interval i2(minItr2->second, maxItr2->second);
			 
			 Interval intersect = i1*i2;
			 minIntersection[minItr1->first] = intersect.min;
			 maxIntersection[minItr1->first] = intersect.max;
		 }
		 
	return StateCell(minIntersection, maxIntersection);
}

bool StateCell::operator==(const StateCell & cell) const
{
	return (minState == cell.minState && maxState == cell.maxState);
}

Logger& operator<<(Logger& os, const StateCell& cell)
{
	os << "Cell: " << os.EndL(Logger::INC);
	os << cell.minState << os.EndL();
	os << "--->" << os.EndL();
	os << cell.maxState << os.EndL(Logger::DEC);
	return os;
}
