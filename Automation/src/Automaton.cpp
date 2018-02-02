#include "Automaton.h"
#include "Utility/LogFunctions.h"
#include "SensorOutput.h"

using namespace std;
using namespace LogFunctions;

Automaton::Automaton(const std::string& n)
{
	name = n;
}

Automaton::~Automaton()
{
}

// Default functionality, stores only most current data
void Automaton::ReceiveSensorOutput(const SensorOutput& sensorOutput, const double& currentTime)
{
	const Agent& currentSelf = sensorOutput.RetrieveSelfData();
	const AgentVector& currentOthers = sensorOutput.RetrieveOtherAgentsData();
	const EnvironmentParameters& currentEnv = sensorOutput.RetrieveEnvironmentData();
	
	selfTrajectory.clear();
	othersTrajectory.clear();
	environmentTrajectory.clear();
	
	selfTrajectory.insert(currentTime, currentSelf);
	othersTrajectory.insert(currentTime, currentOthers);
	environmentTrajectory.insert(currentTime, currentEnv);
}

const TimedContainer<Agent>& Automaton::GetSelfTrajectory() const
{
	return selfTrajectory;
}

const TimedContainer<AgentVector>& Automaton::GetOtherAgentsTrajectory() const
{
	return othersTrajectory;
}

const TimedContainer<EnvironmentParameters>& Automaton::GetEnvironmentTrajectory() const
{
	return environmentTrajectory;
}

const std::string & Automaton::GetName() const
{
	return name;
}

void Automaton::Evolve(const bool& optimize)
{
	
	Maneuver automatonOutput = maneuver;
	
	// Evaluates each transition from currentManeuver (as initDiscrState)
	// If more than once is possible, error if optimize==false, 
	// transite to the first possible maneuver if true.
	// Optimization is recommended only if sure that only one transition 
	// evaluates to true at the same time.
	
	//Only useful when not optimized
	bool hasFoundTransition = false;
	Maneuver firstFoundTransition = string("UNKNOWN");
	Maneuver otherFoundTransition = string("UNKNOWN");
	
	for (auto transition = transitions.begin(); transition != transitions.end(); transition++)
	{
		const Maneuver& initDiscrState = transition->first.first;
		
		// We are only interested on transitions from currentManeuver to others
		if (initDiscrState != maneuver)
			continue;
		
		const Maneuver& finalDiscrState = transition->first.second;
		
		// Evaluate transition
		bool currentResult = transition->second.Evaluate(selfTrajectory, othersTrajectory, environmentTrajectory, properties);
		
		// A possible transition was already found
		if (currentResult && hasFoundTransition)
			Error("Automaton::Evolve", string("More than one transition possible in automaton \'") + name + "\' ---  " + initDiscrState.GetName() + "-->" + firstFoundTransition.GetName() + " and " + 
			initDiscrState.GetName() + "-->" + finalDiscrState.GetName());
		
		if (currentResult)
			hasFoundTransition = true;
		
		if (currentResult && optimize)
		{
			automatonOutput = finalDiscrState;
			return;
		}
		else if (currentResult && !optimize)
		{
			firstFoundTransition = finalDiscrState;
		}
		
	}
	
	if (firstFoundTransition != "UNKNOWN")
		automatonOutput = Maneuver(firstFoundTransition);
	// else no transitions found, maneuver doesn't change

	/*for (auto fail = failures.begin(); fail != failures.end(); fail++)
	{
		fail->Run(automatonOutput, maneuver, currentTime);
	}
	*/
	
	maneuver = automatonOutput;
	
}

const Maneuver& Automaton::GetManeuver() const
{
	return maneuver;
}

void Automaton::SetManeuver(const Maneuver& man)
{
	maneuver = man;
}

const std::vector<Maneuver>& Automaton::GetPossibleManeuvers() const
{
	return maneuvers;
}

void Automaton::SetPossibleManeuvers(const std::vector<Maneuver> & mans)
{
	maneuvers = mans;
}

void Automaton::RegisterSubEvent(const std::string& name, SubEvent::InteractionFcn subEventFcn, SubEvent::AreaFcn areaFcn, const SubEvent::EvalMode& mode, const std::string& description)
{		
	subEvents.insert(SubEvent(name, subEventFcn, areaFcn, mode, description));
}

void Automaton::RegisterSubEvent(const std::string& name, SubEvent::SingleEvaluationFcn subEventFcn, const SubEvent::EvalMode& mode, const std::string& description)
{
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

void Automaton::PreEvolve()
{
}

void Automaton::PostEvolve()
{
}

std::string Automaton::GetProperty(const std::string& propertyName) const
{
	std::string property;
	
	try
	{
		property = properties.at(propertyName);
	}
	catch (out_of_range&)
	{
		Error("Automaton::GetProperty", string("Property \'") + propertyName + "\' not set");
	}
	
	return property;
}

void Automaton::SetProperty(const std::string& propertyName, const std::string& propertyValue)
{
	properties.AddEntry(propertyName, propertyValue);
}

void Automaton::UnsetProperty(const std::string& propertyName)
{
	properties.RemoveEntry(propertyName);
}

bool Automaton::IsPropertyAvailable(const std::string& propertyName) const
{	
	try
	{
		properties.at(propertyName);
	}
	catch (out_of_range&)
	{
		return false;
	}
	
	return true;
}
