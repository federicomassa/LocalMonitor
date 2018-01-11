#include "StateCell.h"
#include "StateRegion.h"
#include "Utility/LogFunctions.h"
#include "Utility/MyLogger.h"
#include "Utility/Interval.h"

#include <string>
#include <vector>
#include <algorithm>

using namespace LogFunctions;
using namespace std;

StateCell::StateCell(MyLogger* log) : minState(), maxState()
{
	logger = log;
}

StateCell::StateCell(const State &min, const State &max, MyLogger* log)
{
	logger = log;
	
    CheckConsistency("State::State", min, max);

    for (StateMap::const_iterator minItr = min.begin(); minItr != min.end(); minItr++) {
        StateMap::const_iterator maxItr = max.find(minItr->first);

        if (minItr->second > maxItr->second)
            Error("StateCell::StateCell", "Min state must be smaller than max state");
    }

    minState = min;
    maxState = max;

}

bool StateCell::IsEmpty() const
{
    for (StateMap::const_iterator minItr = minState.begin();
            minItr != minState.end(); minItr++) {
        StateMap::const_iterator maxItr = maxState.find(minItr->first);
        Interval i(minItr->second, maxItr->second);
        if (i.IsEmpty())
            return true;
    }

    return false;
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
    for (int i = 0; i < q.Size(); i++) {
        for (StateMap::const_iterator itr = minState.begin(); itr != minState.end(); itr++) {
            StateMap::const_iterator itr2 = q.find(itr->first);
            if (*itr > *itr2)
                return false;
        }

        for (StateMap::const_iterator itr = maxState.begin(); itr != maxState.end(); itr++) {
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

void StateCell::Weld(const StateCell &cell, const StateName &var)
{
    CheckConsistency("StateCell::Weld", minState, cell.minState);

    try {
        /* computing intersection */
        vector<double> vertex;
        vertex.push_back(minState.at(var));
        vertex.push_back(maxState.at(var));
        vertex.push_back(cell.minState.at(var));
        vertex.push_back(cell.maxState.at(var));
        sort(vertex.begin(), vertex.end());

        minState.at(var) = vertex[0];
        maxState.at(var) = vertex[3];
    } catch (out_of_range &e) {
        Error("StateCell::Weld", string(var) + " is not a valid state variable");
    }
}

bool StateCell::Slice(const StateCell &cell, StateRegion &reg) const
{
    CheckConsistency("StateCell::Slice", minState, cell.minState);
    /* if the rectangles don't have an intersection return an empty area */
    if (!HasIntersection(cell))
        return false;

    /* empty destination list */
    reg.Purge();

    /* computing intersection */
    map<StateName, vector<double> > vertex;
    for (StateConstIterator stateItr = minState.begin(); stateItr != minState.end(); stateItr++) 
	{
        vertex[stateItr->first].push_back(minState.at(stateItr->first));
        vertex[stateItr->first].push_back(maxState.at(stateItr->first));
        vertex[stateItr->first].push_back(cell.minState.at(stateItr->first));
        vertex[stateItr->first].push_back(cell.maxState.at(stateItr->first));

        // For each state variable, vector now contains the ordered points
        sort(vertex[stateItr->first].begin(), vertex[stateItr->first].end());		
    }


    /* building 3^Q_DIM rectangles */
    map<StateName, vector<double> > bounds;
    map<StateName, int> counter;

    for (StateConstIterator stateItr = minState.begin(); stateItr != minState.end(); stateItr++)
        counter[stateItr->first] = 0;


    bool exitCondition = false;
    while (!exitCondition) {		
		
        /* creating rectangle */
        for (StateConstIterator stateItr = minState.begin(); stateItr != minState.end(); stateItr++) 
		{			
            try {
				// Purge from previous iteration
				bounds[stateItr->first].clear();
				
                bounds[stateItr->first].push_back(vertex.at(stateItr->first).at(counter.at(stateItr->first)));

				
                bounds[stateItr->first].push_back(vertex.at(stateItr->first).at(counter.at(stateItr->first) + 1));				
				
				
            } catch (std::out_of_range &e) {
                Error("StateCell::Slice", "Something is wrong with counter/vertex size");
            }
        }


        State newMin, newMax;
        for (StateConstIterator stateItr = minState.begin(); stateItr != minState.end(); stateItr++) {
            newMin[stateItr->first] = bounds.at(stateItr->first)[0];
            newMax[stateItr->first] = bounds.at(stateItr->first)[1];
        }


        StateCell newCell(newMin, newMax);
        /* add rectangle to the new area */
        if (Contains(newCell) || cell.Contains(newCell))
		{
            reg.AddCell(newCell);
		}

        /* checking exit condition and updating dimension counters */
        StateConstIterator nextStateItr;

        // TODO Special case of 1 state variable??
        try {
            for (StateConstIterator stateItr = minState.begin(); stateItr != minState.end(); stateItr++) {
            

                if (stateItr == minState.begin())
                    (counter.at(stateItr->first))++;

                if (counter.at(stateItr->first) == 3) {
                    // if last iteration
                    if (stateItr == (--minState.end())) {
                        exitCondition = true;
                        break;
                    } 
                    else 
					{
						stateItr++;
						nextStateItr = stateItr;
						// set it back for next iteration
						stateItr--;
						
                        counter.at(stateItr->first) = 0;
                        (counter.at(nextStateItr->first)++);
                    }
                }

            }

        } catch (std::out_of_range &e) {
            Error("StateCell::Slice", "Out of range exception in counter update");
        }


    }



    return true;
}


bool StateCell::HasIntersection(const StateCell &cell) const
{
    CheckConsistency("StateCell::HasIntersection", minState, cell.minState);

    for (StateConstIterator minItr1 = minState.begin(); minItr1 != minState.end(); minItr1++) {
        StateConstIterator maxItr1 = maxState.find(minItr1->first);
        StateConstIterator minItr2 = cell.minState.find(minItr1->first);
        StateConstIterator maxItr2 = cell.maxState.find(minItr1->first);

        Interval i1(minItr1->second, maxItr1->second);
        Interval i2(minItr2->second, maxItr2->second);

        if (!i1.HasIntersection(i2))
            return false;

    }

    return true;

}

bool StateCell::HasNonEmptyIntersection(const StateCell &cell) const
{
    CheckConsistency("StateCell::HasIntersection", minState, cell.minState);

    for (StateConstIterator minItr1 = minState.begin(); minItr1 != minState.end(); minItr1++) {
        StateConstIterator maxItr1 = maxState.find(minItr1->first);
        StateConstIterator minItr2 = cell.minState.find(minItr1->first);
        StateConstIterator maxItr2 = cell.maxState.find(minItr1->first);

        Interval i1(minItr1->second, maxItr1->second);
        Interval i2(minItr2->second, maxItr2->second);

        if (!i1.HasNonEmptyIntersection(i2))
            return false;

    }

    return true;
}

bool StateCell::IsAligned(const StateCell &cell, const StateName &var) const
{
    CheckConsistency("StateCell::IsAligned", minState, cell.minState);

    for (StateConstIterator minItr1 = minState.begin(); minItr1 != minState.end(); minItr1++) {
        if (minItr1->first == var)
            continue;

        StateConstIterator maxItr1 = maxState.find(minItr1->first);
        StateConstIterator minItr2 = cell.minState.find(minItr1->first);
        StateConstIterator maxItr2 = cell.maxState.find(minItr1->first);

        if (minItr1->second != minItr2->second ||
                maxItr1->second != maxItr2->second)
            return false;

    }

    return true;
}

StateCell StateCell::operator*(const StateCell &cell) const
{
    CheckConsistency("StateCell::operator*", minState, cell.minState);

    State minIntersection;
    State maxIntersection;

    for (StateMap::const_iterator minItr1 = minState.begin(); minItr1 != minState.end(); minItr1++) {
        StateMap::const_iterator maxItr1 = maxState.find(minItr1->first);
        StateMap::const_iterator minItr2 = cell.minState.find(minItr1->first);
        StateMap::const_iterator maxItr2 = cell.maxState.find(minItr1->first);

        Interval i1(minItr1->second, maxItr1->second);
        Interval i2(minItr2->second, maxItr2->second);

        Interval intersect = i1 * i2;
        minIntersection[minItr1->first] = intersect.min;
        maxIntersection[minItr1->first] = intersect.max;
    }

    return StateCell(minIntersection, maxIntersection);
}

bool StateCell::operator==(const StateCell &cell) const
{
    return (minState == cell.minState && maxState == cell.maxState);
}

MyLogger &operator<<(MyLogger &os, const StateCell &cell)
{
    os << "Cell: " << os.EndL(MyLogger::INC);
    os << cell.minState << os.EndL();
    os << "--->" << os.EndL();
    os << cell.maxState << os.EndL(MyLogger::DEC);
    return os;
}
