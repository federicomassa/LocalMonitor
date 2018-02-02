#include "Transition.h"
#include "Event.h"
#include "Utility/LogFunctions.h"

using namespace std;
using namespace LogFunctions;

Transition::Transition(const Maneuver& init, const Maneuver& final, const std::set<const Event*>& listOfEvents)
{
	Require(init != final, "Transition::Transition", "Cannot declare transition between same maneuver. The automaton automatically stays in the same maneuver when all transitions evaluate to false");
	
	initDiscrState = init;
	finalDiscrState = final;
	events = listOfEvents;
}

Transition::Transition()
{
	initDiscrState = string("UNKNOWN");
	finalDiscrState = string("UNKNOWN");
}

Transition::Transition(const Transition& t)
{
	initDiscrState = t.initDiscrState;
	finalDiscrState = t.finalDiscrState;
	events = t.events;
}

Transition & Transition::operator=(const Transition& t)
{
	initDiscrState = t.initDiscrState;
	finalDiscrState = t.finalDiscrState;
	events = t.events;
	
	return *this;
}



void Transition::AddEvent(const Event* e)
{
	// Check that the event is new
	bool isNew = true;
	for (auto event = events.begin(); event != events.end(); event++)
	{
		if (e->GetName() == (*event)->GetName())
		{
			isNew = false;
			break;
		}
	}
	
	if (isNew)
		events.insert(e);
	else
		Error("Transition::AddEvent", string("Trying to add already present event named \'") + e->GetName() + "\'");
}

bool Transition::Evaluate(const TimedContainer<Agent>& self, const TimedContainer<AgentVector>& others, const TimedContainer<EnvironmentParameters>& env, const Properties& automatonProperties) const
{
	// A transition happens when at least one event evaluates to true
	for (auto event = events.begin(); event != events.end(); event++)
	{
		if ((*event)->Evaluate(self, others, env, automatonProperties))
			return true;
	}
	
	return false;
	
}
