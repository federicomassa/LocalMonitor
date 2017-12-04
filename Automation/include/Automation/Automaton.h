// Automaton that manages the transitions among discrete states of the system.
// It follows an automaton model that is declared in config file
// It is an abstract base class for particular instances of user-defined automatons

#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "Basic/Agent.h"
#include "Basic/Maneuver.h"
#include "Basic/StateRegion.h"
#include <set>

class Automaton
{
public:
	enum EvalMode {OR, NOR, SINGLE, NSINGLE};
private:
	
	friend class SimulAgent;
	Maneuver maneuver;
	void SetManeuver(const Maneuver&);

	std::set<SubEvent> subEvents;
	std::set<Event> events;
	std::map<Transition>
	
	typedef StateRegion (*AreaFcn) (const Agent& self);
	typedef bool (*InteractionFcn) (const Agent& self, const Agent& other);
	typedef bool (*SingleEvaluationFcn) (const Agent& self);
	
protected:
	// Register a sub-event (add it to the set). InteractionFcn is a function returning
	// a boolean that checks a logical condition based on this agent and on one other.
	// AreaFcn is a function
	// that specifies what region of the state space has to be known in order
	// to be able tu surely check the logical condition. It is used during communication
	// to determine what degree of inference can be achieved with the information provided. In this case, mode can either be OR or NOR, depending if the
	// logical conditions are to be evaluated as OR or NOR on each agent measured by
	// the sensors.
	void RegisterSubEvent(const std::string& name, InteractionFcn subEventFcn, AreaFcn areaFcn, const EvalMode& mode = OR, const std::string& description = "");
	
	// Compared to the other version of the function, it links the sub-event to 
	// a function that can be evaluated based only on this agent's state.
	// Here, mode can be either SINGLE or NSINGLE, depending if the logical condition
	// has to be positively or negatively checked (positive --> true verifies condition
	// negative --> false verifies condition).
	void RegisterSubEvent(const std::string& name, SingleEvaluationFcn subEventFcn,
						  const EvalMode& mode = SINGLE, const std::string& description = "");
	
	// An event is a collection of subevents. An event happens when each subevent
	// is verified (logical AND condition of subevents)
	void RegisterEvent(const std::string& name, std::set<const SubEvent*> listOfSubevents, const std::string& description = "");
	
	// Add to the given transition initManeuver->finalManeuver an event.
	// The transition between two discrete states happens when at least one
	// of its events holds true (logical OR condition of events). 
	// Self transition (state->state) is not defined, because it automatically
	// happens when every other transition does not.
	void AddTransition(const std::string& initDiscrState, const std::string& finalDiscrState, const Event* event);
	
	
public:
	
	const Maneuver& GetManeuver() const;
	void Evolve();
	
};

#endif
