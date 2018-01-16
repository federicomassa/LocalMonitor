// Automaton that manages the transitions among discrete states of the system.
// It follows an automaton model that is declared in config file
// It is an abstract base class for particular instances of user-defined automatons

#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "Basic/Agent.h"
#include "SubEvent.h"
#include "Event.h"
#include "Transition.h"
#include "Basic/Maneuver.h"
#include "Basic/StateRegion.h"
#include "Utility/TimedContainer.h"
#include "Utility/EnvironmentParameters.h"
#include "Utility/Properties.h"
#include <set>
#include <map>
#include <string>

class SensorOutput;


// TODO there is no check that automaton works on the maneuvers defined by control model
class Automaton
{
	friend class SimulatorConfiguration;
private:
	std::string name;
	
	// Like controller, also the automaton can store trajectories in memory
	// and use them to determine next maneuver
	TimedContainer<Agent> selfTrajectory;
	TimedContainer<AgentVector> othersTrajectory;
	TimedContainer<EnvironmentParameters> environmentTrajectory;
	
	friend class SimulatorConfiguration;
	Maneuver maneuver;

	std::set<SubEvent> subEvents;
	std::set<Event> events;
	
	// each transition correspond to a pair initDiscrState->finalDiscrState
	std::map<std::pair<std::string, std::string>, Transition> transitions;
		
	std::vector<Maneuver> maneuvers;
	
	//Properties
	Properties properties;
	
protected:
	// Register a sub-event (add it to the set). InteractionFcn is a function returning
	// a boolean that checks a logical condition based on this agent and on one other.
	// AreaFcn is a function
	// that specifies what region of the state space has to be known in order
	// to be able tu surely check the logical condition. It is used during communication
	// to determine what degree of inference can be achieved with the information provided. In this case, mode can either be OR or NOR, depending if the
	// logical conditions are to be evaluated as OR or NOR on each agent measured by
	// the sensors.
	void RegisterSubEvent(const std::string& name, SubEvent::InteractionFcn subEventFcn, SubEvent::AreaFcn areaFcn, const SubEvent::EvalMode& mode = SubEvent::OR, const std::string& description = "");
	
	// Compared to the other version of the function, it links the sub-event to 
	// a function that can be evaluated based only on this agent's state.
	// Here, mode can be either SINGLE or NSINGLE, depending if the logical condition
	// has to be positively or negatively checked (positive --> true verifies condition
	// negative --> false verifies condition).
	void RegisterSubEvent(const std::string& name, SubEvent::SingleEvaluationFcn subEventFcn,
						  const SubEvent::EvalMode& mode = SubEvent::SINGLE, const std::string& description = "");
	
	// An event is a collection of subevents. An event happens when each subevent
	// is verified (logical AND condition of subevents). Subevents are 
	// looked up from the list of registered subevents
	void RegisterEvent(const std::string& name, const std::set<std::string>& listOfSubeventNames, const std::string& description = "");
	
	// Add to the given transition initManeuver->finalManeuver an event.
	// The transition between two discrete states happens when at least one
	// of its events holds true (logical OR condition of events). 
	// Self transition (state->state) is not defined, because it automatically
	// happens when every other transition does not.
	void AddTransition(const std::string& initDiscrState, const std::string& finalDiscrState, const std::set<std::string>& listOfEventNames);
	
	
	const TimedContainer<Agent>& GetSelfTrajectory() const;
	const TimedContainer<AgentVector>& GetOtherAgentsTrajectory() const;
	const TimedContainer<EnvironmentParameters>& GetEnvironmentTrajectory() const;
	
	void SetProperty(const std::string& propertyName, const std::string& propertyValue);
	void UnsetProperty(const std::string& propertyName);
	std::string GetProperty(const std::string& propertyName) const;
	bool IsPropertyAvailable(const std::string& propertyName) const;
	
public:
	Automaton(const std::string&);
	virtual ~Automaton();
	// This method is where the user defines its rules
	virtual void DefineRules() = 0;
	const Maneuver& GetManeuver() const;
	const std::vector<Maneuver>& GetPossibleManeuvers() const;

	void Evolve(const bool& optimize = false);
	
	// Functions called before and after evolution, might be of use for specific automatons
	virtual void PreEvolve();
	virtual void PostEvolve();
	
	const std::string& GetName() const;
	void SetManeuver(const Maneuver&);
	void SetPossibleManeuvers(const std::vector<Maneuver>&);

	// By default, stores only last sensor data. If a different behaviour
	// is desired, please override this function in your controller
	// TODO make other functions to facilitate this task for the user
	virtual void ReceiveSensorOutput(const SensorOutput&, const double& currentTime);
	
};

#endif
