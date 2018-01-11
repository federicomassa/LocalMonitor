#ifndef STATE_REGION_H
#define STATE_REGION_H

#include "StateCell.h"
#include <vector>

class MyLogger;

class StateRegion
{
    Region region;
	MyLogger* logger;
	//temporarily public
public:
	void DeleteNullCells();
	void SimplifyByAlign();
	void SimplifyByContain();
	void ExpandByIntersect();
public:
	StateRegion(MyLogger* log = nullptr);
	StateRegion(const StateRegion&);
	StateRegion(const Region&, MyLogger* log);
	Region::const_iterator begin() const;
	Region::const_iterator end() const;
	Region::iterator begin();
	Region::iterator end();
	void Purge();
	int Size() const;
	void Simplify();
	bool Belongs(const StateCell&);
	bool AddCell(const StateCell&);
	Region::const_iterator FindCell(const StateCell&);
	bool Join(const StateRegion&);
	void RemoveCell(const Region::const_iterator&);
	
	bool Contains(const State&) const;
	
	friend MyLogger & operator<<(MyLogger&, const StateRegion&);

};

MyLogger & operator<<(MyLogger&, const StateRegion&);


#endif

