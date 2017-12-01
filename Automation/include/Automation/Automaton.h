// Automaton that manages the transitions among discrete states of the system.
// It follows an automaton model that is declared in config file

#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "Basic/Maneuver.h"

class Automaton
{
	Maneuver maneuver;
public:
	const Maneuver& GetManeuver() const;
	void SetManeuver(const Maneuver&);
};

#endif
