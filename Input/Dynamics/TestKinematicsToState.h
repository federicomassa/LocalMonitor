#ifndef TEST_KINEMATIC_TO_STATE_H
#define TEST_KINEMATIC_TO_STATE_H

#include "Basic/Agent.h"
#include <math.h>
#include <iostream>


void TestKinematicsToState(Agent& localState, const Agent& agent)
{
	localState.SetID(agent.GetID());
	AgentParameters pars;
	
	pars["length"] = agent("length");
	pars["desiredV"] = agent("desiredV");
	localState.SetParameters(pars);
	
	localState("x") = 0.5*(agent("xb") + agent("xf"));
	localState("y") = 0.5*(agent("yb") + agent("yf"));
	
	localState("theta") = atan2(agent("yf") - agent("yb"), agent("xf") - agent("xb"));
	localState("v") = agent("v");
}

#endif
