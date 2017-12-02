#include "Automaton.h"

const Maneuver& Automaton::GetManeuver() const
{
	return maneuver;
}

void Automaton::SetManeuver(const Maneuver& man)
{
	maneuver = man;
}
