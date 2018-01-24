#include "SubEvent.h"
#include "Utility/LogFunctions.h"
#include "Basic/StateRegion.h"

using namespace std;
using namespace LogFunctions;

SubEvent::SubEvent(const string& n, SingleEvaluationFcn fcn, const EvalMode& m, const string& descr)
{
	Require(fcn != nullptr, "SubEvent::SubEvent", "Cannot instantiate sub-event without logical condition");
	
	Require(m == SINGLE || m == NSINGLE, "SubEvent::SubEvent", "Single evaluating sub-events must have a SINGLE or NSINGLE evaluation mode");
	
	// It's a single evaluating sub-event
	areaFcn = nullptr;
	interactionFcn = nullptr;
	
	name = n;
	singleEvaluationFcn = fcn;
	mode = m;
	description = descr;
}

SubEvent::SubEvent(const string& n, InteractionFcn fcn, AreaFcn area, const EvalMode& m, const string& descr)
{
	Require(fcn != nullptr, "SubEvent::SubEvent", "Cannot instantiate sub-event without logical condition");
	
	Require(m == OR || m == NOR, "SubEvent::SubEvent", "Interaction sub-events must have a SINGLE or NSINGLE evaluation mode");	
	
	// It's an interaction sub-event
	singleEvaluationFcn = nullptr;
		
	name = n;
	interactionFcn = fcn;
	areaFcn = area;
	mode = m;
	description = descr;
}

SubEvent::SubEvent(const SubEvent& se)
{
	name = se.name;
	singleEvaluationFcn = se.singleEvaluationFcn;
	interactionFcn = se.interactionFcn;
	areaFcn = se.areaFcn;
	mode = se.mode;
	description = se.description;
}


bool SubEvent::operator<(const SubEvent& se) const
{
	return name < se.name;
}

bool SubEvent::operator==(const SubEvent& se) const
{
	bool equal = true;
	
	equal = equal && (name == se.name);
	return equal;
}

const string& SubEvent::GetName() const
{
	return name;
}

bool SubEvent::Evaluate(const TimedContainer<Agent>& self, const TimedContainer<AgentVector>& others, const TimedContainer<EnvironmentParameters>& env, const Properties& automatonProperties) const
{
	// A sub-event evaluates to true based on the logical condition, the sensor data available, the evaluation mode chosen in Automaton::DefineRules for this sub-event.
	
	// mode == OR/NOR means that the logical condition is evaluated with an OR/NOR for each external agent
	// mode == SINGLE/NSINGLE means that the logical condition (its negation in NSINGLE case) is evaluated on 
	// the single agent (usually self during simulation, possibly an observed agent in Observer module) 
	
	EnvironmentParameters currEnv = env.begin().value();
	Agent currSelf = self.begin().value();
	
	bool result;
	
	// ==================== OR/NOR case ===================== //
	if (mode == OR || mode == NOR)
	{
		// TODO For now automaton can store in memory but sub-events can only
		// evaluate based on latest measurement
		
		AgentVector currOthers = others.begin().value();
		
		
		
		// Initialize to neutral element of logical operation
		if (mode == OR)
			result = false;
		else if (mode == NOR)
			result = true;
		
		for (auto currOther = currOthers.begin(); currOther != currOthers.end();
			 currOther++)
			 {
				 if (mode == OR)
				 {
					 // If area fcn was linked to the sub-event, use it to verify that the agent lies inside that area, otherwise just use the logical condition					 
					 if (areaFcn == nullptr)
						result = result || interactionFcn(currSelf, currOther->second, currEnv, automatonProperties);
					 else
						 result = result || (interactionFcn(currSelf, currOther->second, currEnv, automatonProperties) && areaFcn(currSelf).Contains(currOther->second.GetState()));
				 }
				 else if (mode == NOR)
				 {
					// If area fcn was linked to the sub-event, use it to verify that the agent lies inside that area, otherwise just use the logical condition
					 if (areaFcn == nullptr)
						result = result && !interactionFcn(currSelf, currOther->second, currEnv, automatonProperties);
					 else
						 result = result && !(interactionFcn(currSelf, currOther->second, currEnv, automatonProperties) && areaFcn(currSelf).Contains(currOther->second.GetState()));
				 }
			 }
	}
	
	// ==================== SINGLE/NSINGLE case ===================== //
	else if (mode == SINGLE || mode == NSINGLE)
	{
		// TODO For now automaton can store in memory but sub-events can only
		// evaluate based on latest measurement
		
		result = singleEvaluationFcn(currSelf, currEnv, automatonProperties);
		
		if (mode == NSINGLE)
			result = !result;
	}
	
	return result;
}


