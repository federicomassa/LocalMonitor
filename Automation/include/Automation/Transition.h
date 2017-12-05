#ifndef TRANSITION_H
#define TRANSITION_H

#include <string>
#include <set>

class Event;

class Transition
{	
	// The transition is between two discrete states: initDiscrState->finalDiscrState
	std::string initDiscrState;
	std::string finalDiscrState;
	
	// A transition is true when at least one event evaluates to true
	std::set<const Event*> events;
	

public:
	Transition(const std::string& init, const std::string& final, const std::set<const Event*>& listOfEvents);
	
	void AddEvent(const Event*);
	
	
};

#endif
