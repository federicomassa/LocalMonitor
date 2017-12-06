#include "Event.h"
#include "SubEvent.h"

using namespace std;

Event::Event(const string& n, const set<const SubEvent*>& listOfSubevents, const string& descr)
{
	name = n;
	description = descr;
	subEvents = listOfSubevents;
}

Event::Event(const Event& e)
{
	name = e.name;
	description = e.description;
	subEvents = e.subEvents;
} 

const string& Event::GetName() const
{
	return name;
}

bool Event::operator<(const Event& e) const
{
	return (name < e.name);
}

bool Event::operator==(const Event& e) const
{
	bool equal = true;
	equal = equal && (name == e.name);
	
	return equal;
}

bool Event::Evaluate(const TimedContainer<Agent>& self, const TimedContainer<AgentVector>& others, const TimedContainer<EnvironmentParameters>& env) const
{
	// An event evaluates to true if the AND of its subevents evaluation 
	// is true. NB each subevent is evaluated based on its EvalMode (OR, NOR, SINGLE, NSINGLE...)
	
	bool evaluation = true;
	for (auto sub = subEvents.begin(); sub != subEvents.end(); sub++)
	{
		evaluation = evaluation && (*sub)->Evaluate(self, others, env);
	}
	
	return evaluation;
	
}

