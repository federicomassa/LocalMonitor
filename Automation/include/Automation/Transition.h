#ifndef TRANSITION_H
#define TRANSITION_H

#include <string>
#include <set>
#include <map>

#include "Basic/Maneuver.h"
#include "Basic/Agent.h"
#include "Utility/TimedContainer.h"
#include "Utility/EnvironmentParameters.h"
#include "Utility/Properties.h"

class Event;
class Transition
{	
	// The transition is between two discrete states: initDiscrState->finalDiscrState
	Maneuver initDiscrState;
	Maneuver finalDiscrState;
	
	// A transition is true when at least one event evaluates to true
	std::set<const Event*> events;
	

public:
	Transition();
	Transition(const Maneuver& init, const Maneuver& final, const std::set<const Event*>& listOfEvents);
	
	Transition(const Transition&);
	Transition& operator=(const Transition&);
	
	void AddEvent(const Event*);
	
	bool Evaluate(const TimedContainer<Agent>& self, const TimedContainer<AgentVector>& others, const TimedContainer<EnvironmentParameters>& env, const Properties& automatonProperties) const;
};

#endif
