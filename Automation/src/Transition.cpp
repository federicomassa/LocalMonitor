#include "Transition.h"
#include "Event.h"
#include "Utility/LogFunctions.h"

using namespace std;
using namespace LogFunctions;

Transition::Transition(const std::string& init, const std::string& final, const std::set<const Event*>& listOfEvents)
{
	initDiscrState = init;
	finalDiscrState = final;
	events = listOfEvents;
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
