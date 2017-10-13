#ifndef STATE_CELL_H
#define STATE_CELL_H

#include "State.h"

class StateCell
{
	State minState;
	State maxState;

public:
	StateCell(const State&, const State&);
	const State & GetMin() const;
	const State & GetMax() const;
	bool Contains(const State&) const;
	bool Contains(const StateCell&) const;

};

#endif
