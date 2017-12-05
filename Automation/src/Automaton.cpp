#include "Automaton.h"
#include "Utility/LogFunctions.h"

using namespace std;
using namespace LogFunctions;

const Maneuver& Automaton::GetManeuver() const
{
	return maneuver;
}

void Automaton::SetManeuver(const Maneuver& man)
{
	maneuver = man;
}

void Automaton::RegisterSubEvent(const std::string& name, SubEvent::InteractionFcn subEventFcn, SubEvent::AreaFcn areaFcn, const SubEvent::EvalMode& mode, const std::string& description)
{
	if (mode != SubEvent::OR || mode != SubEvent::NOR)
		Error("Automaton::RegisterSubEvent", "Trying to construct an interaction sub-event with mode different from OR or NOR");
	
	Require(subEventFcn != nullptr, "Automaton::RegisterSubEvent", "Cannot register sub-event without logical condition function");
	
	// TODO treat the case of interaction without areaFcn
	if (areaFcn == nullptr)
		Error("Automaton::RegisterSubEvent", "How to treat null area case?");
	
	subEvents.insert(SubEvent(name, subEventFcn, areaFcn, mode, description));
}

void Automaton::RegisterSubEvent(const std::string& name, SubEvent::SingleEvaluationFcn subEventFcn, const SubEvent::EvalMode& mode, const std::string& description)
{
	if (mode != SubEvent::SINGLE || mode != SubEvent::NSINGLE)
		Error("Automaton::RegisterSubEvent", "Trying to construct a single-evaluating sub-event with mode different from SINGLE or NSINGLE");
	
	Require(subEventFcn != nullptr, "Automaton::RegisterSubEvent", "Cannot register sub-event without logical condition function");
	
	subEvents.insert(SubEvent(name, subEventFcn, mode, description));
}

void Automaton::RegisterEvent(const string& name, const set<string>& listOfSubeventNames, const string& description)
{
	set<const SubEvent*> listOfSubEvents;
	
	// Match the list with the registered sub-events
	for (auto subName = listOfSubeventNames.begin(); subName != listOfSubeventNames.end();
		 subName++)
		 {
			bool found = false;
			for (auto registeredSub = subEvents.begin(); 
				 registeredSub != subEvents.end(); registeredSub++)
				 {
					if (registeredSub->GetName() == *subName)
					{
						listOfSubEvents.insert(&(*registeredSub));
						found = true;
						break;
					}
				 }
				 
			if (!found)
				Error("Automaton::RegisterEvent", string("Subevent \'") + *subName + "\' was not registered");
			
				 
		 }
	
	events.insert(Event(name, listOfSubEvents, description));	
}

void Automaton::AddTransition(const std::string& initDiscrState, const std::string& finalDiscrState, const set<string>& listOfEventNames)
{
	// Check if transition already exists
	try
	{
		transitions.at(make_pair(initDiscrState, finalDiscrState));
		Error("Automaton::AddTransition", string("Transition \'") + initDiscrState + "\' --> \'" + finalDiscrState + "\' is already defined.");
		
	}
	catch (out_of_range& e)
	{
		// Not found. That's fine. Continue
	}
	
	set<const Event*> listOfEvents;
	
	// Match the list with the registered sub-events
	for (auto evName = listOfEventNames.begin(); evName != listOfEventNames.end();
		 evName++)
		 {
			bool found = false;
			for (auto registeredEv = events.begin(); 
				 registeredEv != events.end(); registeredEv++)
				 {
					if (registeredEv->GetName() == *evName)
					{
						listOfEvents.insert(&(*registeredEv));
						found = true;
						break;
					}
				 }
				 
			if (!found)
				Error("Automaton::RegisterTransition", string("Event \'") + *evName + "\' was not registered");
			
				 
		 }
	
	transitions[make_pair(initDiscrState, finalDiscrState)] = Transition(initDiscrState, finalDiscrState, listOfEvents);	
}
