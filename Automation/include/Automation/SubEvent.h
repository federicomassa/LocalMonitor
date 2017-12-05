#ifndef SUBEVENT_H
#define SUBEVENT_H

#include <string>

class StateRegion;


class SubEvent
{
	friend class Automaton;
	std::string name;
	std::string description;
public:
	enum EvalMode {OR, NOR, SINGLE, NSINGLE};
private:
	EvalMode mode;
	typedef StateRegion (*AreaFcn) (const Agent& self);
	typedef bool (*InteractionFcn) (const Agent& self, const Agent& other);
	typedef bool (*SingleEvaluationFcn) (const Agent& self);
	
	
	// 'single' sub-events, i.e. sub-events that can be evaluated
	// only based on measurements of a single agent, must only
	// define singleEvaluationFcn, 'interaction' sub-events must
	// only define areaFcn and interactionFcn. This corresponds to
	// the two RegisterSubEvent functions in Automaton class
	AreaFcn areaFcn;
	InteractionFcn interactionFcn;
	SingleEvaluationFcn singleEvaluationFcn;
	
public:
	SubEvent(const std::string& n, SingleEvaluationFcn fcn, const EvalMode& m, const std::string& descr = "");
	
	SubEvent(const std::string& n, InteractionFcn fcn, AreaFcn area, const EvalMode& m, const std::string& descr = "");
	
	SubEvent(const SubEvent&);
	
	const std::string& GetName() const;
	
	// Used to create sets. NB two subevents are equal if they have the same name
	bool operator<(const SubEvent&) const;
	bool operator==(const SubEvent&) const;
};

#endif
