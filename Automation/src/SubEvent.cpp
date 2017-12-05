#include "SubEvent.h"

using namespace std;

SubEvent::SubEvent(const string& n, SingleEvaluationFcn fcn, const EvalMode& m, const string& descr)
{
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
