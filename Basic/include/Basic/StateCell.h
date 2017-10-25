#ifndef STATE_CELL_H
#define STATE_CELL_H

#include "State.h"

class Logger;
class StateRegion;

class StateCell
{
    State minState;
    State maxState;
	StateCell();
public:
    StateCell ( const State &, const State & );
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

	friend Logger & operator<<(Logger&, const StateCell&);
};

typedef std::vector<StateCell> Region;

Logger& operator<<(Logger&, const StateCell&);

#endif
