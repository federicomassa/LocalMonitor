#include "StateRegion.h"
#include "Utility/MyLogger.h"
#include "Utility/LogFunctions.h"
#include <vector>
#include <utility>

using namespace LogFunctions;

StateRegion::StateRegion(MyLogger* log) : region()
{
	logger = log;
}

StateRegion::StateRegion(const StateRegion& reg) : region(reg.region)
{}

StateRegion::StateRegion(const Region& reg, MyLogger* log) : region(reg)
{
	logger = log;
}

bool StateRegion::AddCell(const StateCell &cell)
{	
    if (region.size() != 0)
        CheckConsistency("StateRegion::AddCell", region.begin()->GetMin(), cell.GetMin());

	if (FindCell(cell) == end())
	{
		region.push_back(cell);
		return true;
	}
	
	return false;
}

int StateRegion::Size() const
{
	return region.size();
}

Region::const_iterator StateRegion::FindCell(const StateCell& cell)
{
	for (Region::const_iterator cellItr = region.begin(); cellItr != region.end(); cellItr++)
	{
		if (*cellItr == cell)
			return cellItr;
	}
	
	return region.end();
}

bool StateRegion::Belongs(const StateCell& cell)
{
	if (FindCell(cell) == end())
		return false;
	
	return true;
}

bool StateRegion::Contains(const State& q) const
{
	bool contain = false;
	
	for (auto cell = region.begin(); cell != region.end(); cell++)
		contain = contain || cell->Contains(q);
	
	
	return contain;
}

bool StateRegion::Join(const StateRegion& reg)
{
	bool result = false;
	
	for (Region::const_iterator itr = reg.begin(); itr != reg.end(); itr++)
	{
		bool thisResult = AddCell(*itr);
		result = result || thisResult;
	}
	
	return result;
}

void StateRegion::RemoveCell(const Region::const_iterator& itr)
{
	if (itr != region.end())
		region.erase(itr);
	else
		throw std::out_of_range("StateRegion::RemoveCell --- Iterator pointed to the end of region");
	
}

void StateRegion::Purge()
{
	region.clear();
}

void StateRegion::Simplify()
{
    DeleteNullCells();
	SimplifyByAlign();
    SimplifyByContain();
    ExpandByIntersect();
    DeleteNullCells();
    SimplifyByAlign();
    SimplifyByContain();
}

void StateRegion::DeleteNullCells()
{
    for (Region::const_iterator itr = region.begin();
            itr != region.end(); itr++) {
        if (itr->IsEmpty()) {
            region.erase(itr);
            itr--;
        }
    }
}

void StateRegion::SimplifyByAlign()
{
    if (region.size() == 0)
        return;

    // Construct list of var names to iterate on
    std::vector<StateName> vars;
    for (StateConstIterator itr = begin()->GetMin().begin();
            itr != begin()->GetMin().end(); itr++)
        vars.push_back(itr->first);


    for (std::vector<StateName>::const_iterator var = vars.begin(); var != vars.end(); var++) {
        /* create a new rectangle list */
        Region newRegion;
        for (Region::const_iterator cell = begin(); cell != end(); cell++) {
			
			StateCell c1 = *cell;
			region.erase(cell);
			cell--;
			
            bool insert = true;
            
			for (Region::const_iterator newCell = newRegion.begin();
				 newCell != newRegion.end(); newCell++)
			{
				const StateCell& c2 = *newCell;
                /* check if the newList contains a rectangle aligned with r1
                    alogn the i-th dimension */
                if (c1.IsAligned(c2, *var) && c1.HasIntersection(c2)) 
				{
					/* merge r1 with r2 among i-th dimension */
                    c1.Weld(c2, *var);
                    /* remove c2 and insert c1 */
                    newRegion.erase(newCell);
					newRegion.insert(newRegion.begin(), c1);
					
                    insert = false;
                    break;
                }
            }
            if (insert)
                newRegion.insert(newRegion.begin(), c1);
        }
        region = newRegion;
    }
}

void StateRegion::SimplifyByContain()
{
   /* create a new rectangle list */
    Region newRegion;
	for (Region::const_iterator cell = begin(); cell != end(); cell++) 
    {
		StateCell c1 = *cell;
		region.erase(cell);
		cell--;
		
        bool insert = true;
		
    	for (Region::const_iterator newCell = newRegion.begin();
				 newCell != newRegion.end(); newCell++)
		{
			const StateCell& c2 = *newCell;
            if(c1.Contains(c2))
                /* if c1 contains a cell c2 in the new list, c2 has to be
                    deleted */
				{
					newRegion.erase(newCell);
					newCell--;
				}
				
            else if(c2.Contains(c1))
            {
                /* if the new list has a rectangle containing r1, it has not to
                    be inserted in the new list */
                insert = false;
                break;
            }
		}
        if(insert)
            newRegion.insert(newRegion.begin(), c1);
    }
    region = newRegion;
}

void StateRegion::ExpandByIntersect()
{
    bool exitCondition = false;
    while(!exitCondition)
    {
        exitCondition = true;
        
		for (Region::iterator cell = begin(); cell != end(); cell++) 
        {
          	for (Region::const_iterator cell2 = std::next(cell); cell2 != end(); cell2++) 
            {
                if(cell->HasNonEmptyIntersection(*cell2))
                {
                    StateRegion newRegion;
                    cell->Slice(*cell2, newRegion);
					
                    if(!newRegion.Belongs(*cell))
                        RemoveCell(FindCell(*cell));
                    if(!newRegion.Belongs(*cell2))
                        RemoveCell(FindCell(*cell2));
					
					if(Join(newRegion))
                    {
					    exitCondition = false;
                        break;
                    }
                    
                }
			}
            if(!exitCondition)
                break;
        }
    }
    
}

Region::const_iterator StateRegion::begin() const
{
	return region.begin();
}

Region::const_iterator StateRegion::end() const
{
	return region.end();
}

Region::iterator StateRegion::begin()
{
	return region.begin();
}

Region::iterator StateRegion::end()
{
	return region.end();
}

MyLogger & operator<<(MyLogger& os, const StateRegion& r)
{
	os << "StateRegion: " << os.EndL(MyLogger::INC);
	for (Region::const_iterator itr = r.begin(); itr != r.end(); itr++)
	{
		os << *itr;
	}
	
	os << os.EndL(MyLogger::DEC);
	
	return os;
}


