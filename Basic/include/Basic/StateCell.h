#ifndef STATE_CELL_H
#define STATE_CELL_H

#include "State.h"

class Logger;

class StateCell
{
    State minState;
    State maxState;
	bool empty;
public:
    StateCell ( const State &, const State & );
	StateCell();
    const State &GetMin() const;
    const State &GetMax() const;
	const bool& IsEmpty() const;
    bool Contains ( const State & ) const;
    bool Contains ( const StateCell & ) const;
	bool operator==(const StateCell &) const;
	StateCell operator* (const StateCell &) const;
	friend Logger & operator<<(Logger&, const StateCell&);
};

Logger& operator<<(Logger&, const StateCell&);

#endif
