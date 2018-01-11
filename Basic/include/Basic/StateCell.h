#ifndef STATE_CELL_H
#define STATE_CELL_H

#include "State.h"

class MyLogger;
class StateRegion;

class StateCell
{
    State minState;
    State maxState;
	StateCell(MyLogger* log = nullptr);
	MyLogger* logger;
public:
    StateCell ( const State &, const State &, MyLogger* log = nullptr );
    const State &GetMin() const;
    const State &GetMax() const;
	bool IsEmpty() const;
    bool Contains ( const State & ) const;
    bool Contains ( const StateCell & ) const;
	bool operator==(const StateCell &) const;
	StateCell operator* (const StateCell &) const;
	bool HasIntersection(const StateCell &) const;
	bool HasNonEmptyIntersection(const StateCell &) const;

	bool IsAligned(const StateCell &, const StateName &) const;
	void Weld(const StateCell&, const StateName &);
	bool Slice(const StateCell&, StateRegion&) const;

	friend MyLogger & operator<<(MyLogger&, const StateCell&);
};

typedef std::vector<StateCell> Region;

MyLogger& operator<<(MyLogger&, const StateCell&);

#endif
