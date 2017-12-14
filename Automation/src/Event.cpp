#include "Event.h"
#include "SubEvent.h"
#include "Utility/LogFunctions.h"

using namespace std;
using namespace LogFunctions;

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

bool Event::Evaluate(const TimedContainer<Agent>& self, const TimedContainer<AgentVector>& others, const TimedContainer<EnvironmentParameters>& env, const Properties& automatonProperties) const
{
	// An event evaluates to true if the AND of its subevents evaluation 
	// is true. NB each subevent is evaluated based on its EvalMode (OR, NOR, SINGLE, NSINGLE...)
	
	bool evaluation = true;
	for (auto sub = subEvents.begin(); sub != subEvents.end(); sub++)
	{
		try
		{	
			evaluation = evaluation && (*sub)->Evaluate(self, others, env, automatonProperties);
		}
		catch (out_of_range&)
		{
			Error("Event::Evaluate", string("Out of range exception in sub-event \'" ) + (*sub)->GetName() + "\'...maybe required non measured state variable? Check if all variables required are measured by sensors");
		}
	}
	
	return evaluation;
	
}

